! This file automatically generated from icnsWallBCCoeff.bf with bpp.










c *wdh* 081214 -- xlf does not like +- --> change O1 to (O1) etc. below









      subroutine icnswallbc(nd,nd1a,nd1b,nd2a,nd2b,nd3a,
     &     nd3b,nd4a,nd4b,coeff,rhs,u,x,aj,rsxy,iprm,rprm,
     &     indexRange,bc, ubvd, bd, bt, nbv, nubv, cfrhs)
c
c     slip ,no-slip, and specified mass flux wall boundary conditions for icns
c
c
c     070927 kkc : Initial Version
c
c     This subroutine applies the various wall boundary condition to the coefficient matrix
c     and the right hand side vector for the implicit CNS method.
c
c     Notes:
c          i) we set the velocity to that specified by the boundary values
c         ii) There are several ways to impose the flux condition.  Right now we assume
c             that viscous fluxes, f^a, satisfy f^a_{i-1/2} = -f^a_{i+1/2} when the real/artificial
c             viscosity is computed. XXX NOTE THIS IS A CONDITION ON icns.
c        iii) For the normal component, we have the condition
c             \rho_{i-1}( a_{11}u + a_{12}v )_{i-1} ) +
c                  2\rho_{i}( a_{11}u + a_{12}v )_{i}  + \rho_{i+1}( a_{11}u + a_{12}v )_{i+1} ) = 4 f.
c             We can either extrapolate \rho and compute the velocity or vice versa.  Right
c             now we choose to extrapolate \rho using a limited extrapolation starting with 2^{nd}
c             order that gets reduced in order until the density is positive.  We also
c             set a dirichlet condition for u and rho on the boundary point (index i above) so
c             we shift the term to the rhs ( resulting on no "i" term in the matrix and 2f on the rhs).
c
c             For the tangential component, we solve for even symmetry of the velocity
c             ( a_{11}u + a_{12}v )_{i-1} )-( a_{11}u + a_{12}v )_{i+1} ) = 0,
c
c             Note the previous discussion applies to the "r" or "i" direction fluxes;
c             the other direction is handled by replacing "i" with "j" and switching the odd/even vector
c             symmetry condition.
c
c         iv) the swirl component (if present) is just extrapolated
c          v) the loops/indices are 0 based 
c
c         vi) cfrhs is a switch,  cfrhs.eq.0 - fill in coefficient array, otherwise fill in rhs
c        vii) XXX THE EQUATION NUMBERS ARE CURRENTLY NOT ALTERED IN THE SPARSE REP!!! 
c                 THIS ROUTINE RELIES ON THE DEFAULT NUMBERING PROVIDED BY THE SPARSE REP FOR MGF
c                 this means that the boundary condition operators for the coeff array should NEVER
c                 be called for the density and velocity components on the boundary and first ghost lines
c                 (because those reset the equation number array)
c       viii) this subroutine consolidates the code in inflowOutflowCoeff.bf and cnsNoSlipWallBC.bf

      implicit none

c     INPUT
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,iprm(0:*)
      double precision coeff(0:iprm(10)*iprm(1)**2-1,
     &                       nd1a:nd1b,nd2a:nd2b,nd3a:nd3b) ! coefficient matrix
      double precision u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b) ! state to linearize about
      double precision rhs(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b) ! right hand side vector
      double precision x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1) ! grid vertices
      double precision aj(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b) ! determinant of grid Jacobian matrix
      double precision rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-
     & 1) ! metric derivatives
      double precision rprm(*)
      integer indexRange(0:1,0:2), bc(0:1,0:2),nbv,nubv
      double precision bd(0:nbv-1,0:1,0:nd-1,0:*)
      double precision ubvd(0:nubv-1,0:1,0:2,0:*)
      integer bt(0:2,0:1,0:2,0:*)
      integer cfrhs

c     OUTPUT
c     u adjusted at boundaries and ghost points

c     LOCAL
      integer i1l,i2l,i3l,eqn,cc
      integer it1,it2,it3,ist(0:2),axp1,axm1
      integer i1,i2,i3,is1,is2,is3,iss(0:2),c,axis,side,irb(0:1,0:2),a,
     & s
      integer i1g,i2g,i3g,i1i,i2i,i3i,e,ncmp,ndim,gmove,neq,teq,ic1,ic2
      integer rc,uc,vc,wc,tc,grid,gridtype,debug,isaxi,radaxis,
     & withswirl
      integer i,j,isten_size,cbnd,width,width3,hwidth,hwidth3
      integer i1gg,i2gg,i3gg
      double precision rr,aa(0:2,0:2),jdet,jdetg,rtmp,aainv(0:2,0:2),
     & den
      double precision ubv(0:30),norm(3),tang(3),grav(3),scale,hlen,
     & sint
      double precision flowsum,flowsum2,drar,jaci,av2,av4
      logical useneumanntemp,iscorner,useneumannvel,extrapponbdy,now
      logical usedirichletrhoinflow,iswall,skipslip,sliponwall,
     & ghostrpos
      double precision ren,prn,man,gam,gm1,gm2,oren,oprn,f43,f13,xr
      double precision coeff_u,coeff_v,fac,len,lur,lus,vect(3),consfac
      double precision rho,uvel,vvel,wvel,temp,mdot,port,pdotn,prhs,
     & pfac
c     LOCAL PARAMETERS
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
      integer noSlipWall,slipWall
      parameter( noslipwall=1,slipWall=4 )
      integer subsonicinflow,subsonicoutflow
      parameter( subsonicinflow=8,subsonicoutflow=10 )
      double precision one
      parameter (one=1d0)
      double precision eps
      parameter (eps=0.1d0)

      integer linearrampinx, axisymmetricsbr
      parameter(linearrampinx=5,axisymmetricsbr=4)! must match enum in userDefinedBoundaryValues.C

c     needed by the macros
      integer d
      double precision alpha,dr(3),dri(3),dri2(3)
      logical usestrik

c     STATEMENT FUNCTIONS
c     in the following extrapolation functions, (i1,i2,i3) refers to the ghost point
c     1st order extrapolation of u, actually neumann if the boundary point is small
c      integer m1,m2,m3,icf
      double precision exo1,exo2,exo3
      double precision rx,xyz
      double precision jac
      include 'icnssfdec.h'
c     1-based indexed arrays
      rx(i1,i2,i3,e,c) = rsxy(i1,i2,i3,e-1,c-1)
      xyz(i1,i2,i3,c) = x(i1,i2,i3,c-1)

      jac(i1,i2,i3) = aj(i1,i2,i3)
      include 'icnssf.h'

      exo1(i1,i2,i3,is1,is2,is3,c) = max( u(i1+is1,i2+is2,i3+is3,c),
     &                                u(i1+2*is1,i2+2*is2,i3+2*is3,c) )
c     2nd order extrapolation of u
      exo2(i1,i2,i3,is1,is2,is3,c) = 2d0*u(i1+is1,i2+is2,i3+is3,c) -
     &                                  u(i1+2*is1,i2+2*is2,i3+2*is3,c)
c     3rd order extrapolation of u
      exo3(i1,i2,i3,is1,is2,is3,c)=3d0*u(i1+  is1,i2+  is2,i3+  is3,c)-
     &                             3d0*u(i1+2*is1,i2+2*is2,i3+2*is3,c)+
     &                                 u(i1+3*is1,i2+3*is2,i3+3*is3,c)

c      icf (m1,m2,m3,e,c) = 
c     &               m1+hwidth+width*(m2+hwidth+width3*(m3+hwidth3)) +
c     &               isten_size*((c)+ncmp*(e)) 
      usestrik = .false.
      useneumannvel = .true.
c      useneumannvel = .false.
      extrapponbdy = .true. !.and. useneumannvel
      usedirichletrhoinflow = .false.
      consfac = 0d0
      if ( .not.useneumannvel ) consfac = 1d0

      off(1)=0
      off(2)=0
      off(3)=0
      occ=0
      oce=0
      ndim = iprm(0)
      ncmp = iprm(1)
      rc   = iprm(2)
      uc   = iprm(3)
      vc   = iprm(4)
      wc   = iprm(5)
      tc   = iprm(6)
      gmove= iprm(7)
      isaxi= iprm(8)
      withswirl= iprm(9)
      isten_size= iprm(10)
      width = iprm(11)
      hwidth= iprm(12)

      width3 = 0
      hwidth3= 0
      cbnd = isten_size*ncmp**2-1
      if ( ndim.eq.3 ) then
         width3 = width
         hwidth3 = hwidth
      endif
      grid = iprm(19)

      debug             =iprm(15)
      radaxis           =iprm(18) ! =axis1 if x=0 is axis of cylindrical symmetry, =axis2 if y=0 is..

      ren = rprm(1)
      prn = rprm(2)
      man = rprm(3)
      gam =rprm(4)
c      impfac=rprm(5)
      dr(1) = rprm(6)
      dr(2) = rprm(7)
      dr(3) = rprm(8)
c      t0    = rprm(9)
c      dt    = rprm(10)
      av2 = rprm(13)
      av4 = rprm(14)
      grav(1) = rprm(15)
      grav(2) = rprm(16)
      grav(3) = rprm(17)
      alpha = rprm(18)
      if ( .not.usestrik ) alpha = 0d0

      f43 = 4d0/3d0
      f13 = 1d0/3d0
      oren = 1d0/ren
      oprn = 1d0/prn
      gm1 = gam-1d0
      gm2 = 1d0/(gam*man*man)
      do d=1,ndim
         dri(d) = 1d0/dr(d)
         dri2(d) = .5d0*dri(d)
      end do

      scale = 1d0
c      print *,oren,oprn
      do a=0,2
         do s=0,1
            irb(s,a)=0
         end do
      end do

      neq = uc
      teq = vc

      do axis=0,nd-1
      do side=0,1

c         print *,"GRID = ",grid," SIDE = ",side," AXIS = ",axis
      axp1 = mod(axis+1,ndim)

      if ( bc(side,axis).eq.noSlipWall .or.
     &     bc(side,axis).eq.slipWall   .or.
     &     bc(side,axis).eq.subsonicinflow .or.
     &     bc(side,axis).eq.subsonicoutflow)  then

         iss(0) = 0
         iss(1) = 0
         iss(2) = 0
         iss(axis) = 1-2*side
         is1=iss(0)
         is2=iss(1)
         is3=iss(2)

         ist(0) = 0
         ist(1) = 0
         ist(2) = 0
         ist(mod(axis+1,ndim)) = 1
         it1=ist(0)
         it2=ist(1)
         it3=ist(2)
         do 100 a=0,nd-1
            do 100 s=0,1
               if ( bc(s,a).ge.0 .or. (s.eq.0) ) then
                  irb(s,a) = indexRange(s,a)
               else if ( s.eq.1 ) then
                  irb(s,a) = indexRange(s,a)-1
               endif
 100     continue ! ha, old school just for fun

         irb(0,axis) = indexRange(side,axis)
         irb(1,axis) = indexRange(side,axis)

c     grab the user defined bc values
c     we get the mass flux per unit area that we want to enforce from
c        rho*uvel and rho*vvel, density and velocity are only specified at the inflow
c        Note that we interpret uvel as the component normal to the boundary and 
c        vvel as the component tangential to it. Positive mass flux means flow is coming 
c        into the domain.
         rho = bd(rc,side,axis,grid)
         uvel= bd(uc,side,axis,grid)
         vvel= bd(vc,side,axis,grid)
c         print *,side,axis,rho,uvel,vvel
         if ( withswirl.gt.0 .or. nd.eq.3 ) wvel= bd(wc,side,axis,grid)
         port= bd(tc,side,axis,grid) ! this is either the pressure or temperature
         useneumanntemp = .true.
         if ( bc(side,axis).ne.slipWall ) then
            if ( bc(side,axis).eq.noslipwall .and. nubv.gt.0 .and.
     &           (bt(0,side,axis,grid).eq.linearrampinx .or.
     &              bt(0,side,axis,grid).eq.axisymmetricsbr)) then
               do c=0, nubv-1
                  ubv(c) = ubvd(c,side,axis,grid)
               end do
               if ( bt(0,side,axis,grid).eq.linearrampinx .or.
     &              bt(0,side,axis,grid).eq.axisymmetricsbr) then
                  if ( ( bt(0,side,axis,grid).eq.linearrampinx .and.
     &                 ubv(3*(max(vc,wc)+1)).gt.0d0 ) .or.
     &                 ( bt(0,side,axis,grid).eq.axisymmetricsbr .and.
     &                 ubv(3).gt.0d0 ) ) then
                     useneumanntemp=.false.
                  endif
               endif
            else if ( bc(side,axis).eq.noslipwall ) then
               useneumanntemp = port.le.0d0
            endif

            if ( bc(side,axis).eq.subsonicinflow .and.
     &           port.gt.0d0) then
               useneumanntemp = .false.
            endif
         endif ! set some wall bc options

         sliponwall = (bc(side,axis).eq.slipWall) .or.
     &             (bc(side,axis).eq.subsonicinflow.and.useneumanntemp)

c         if ( bc(side,axis).eq.subsonicoutflow .or.
c     &        bc(side,axis).eq.subsonicinflow ) then
c         print *,"GRID = ",grid," SIDE = ",side," AXIS = ",axis
c         print *,norm(1),norm(2)
c         print *,"rho,uvel,vvel : ",rho,uvel,vvel
c         print *,"port : ",port,useneumanntemp
c         endif

c         print *,"p vals: ",port,pdotn,prhs
c         print *," rho : ",side,axis,grid,rho
         len = 0d0 ! bdy arc length needed if the mass flux is not zero
         hlen = 0d0
         sint = 0d0 ! discrete approx. of the integral of the weighting function
         do i1=irb(0,0),irb(1,0)-it1
         do i2=irb(0,1),irb(1,1)-it2
         do i3=irb(0,2),irb(1,2)-it3
            len = len + sqrt(
     &           (x(i1+it1,i2+it2,i3+it3,0)-x(i1,i2,i3,0))**2d0 +
     &           (x(i1+it1,i2+it2,i3+it3,1)-x(i1,i2,i3,1))**2d0 )
         end do !i1
         end do !i2
         end do !i3

         flowsum = 0d0
         flowsum2 = 0d0
         do i1l=irb(0,0),irb(1,0)
         do i2l=irb(0,1),irb(1,1)
         do i3l=irb(0,2),irb(1,2)

            i1gg = i1l-2*is1
            i2gg = i2l-2*is2
            i3gg = i3l-2*is3
            i1g = i1l-is1
            i2g = i2l-is2
            i3g = i3l-is3
            i1  = i1l
            i2  = i2l
            i3  = i3l
            i1i = i1l+is1
            i2i = i2l+is2
            i3i = i3l+is3

            jaci = 1d0/jac(i1,i2,i3)

            if ( i1l.ne.irb(0,0) .or. i2l.ne.irb(0,1)
     &           .or. i3l.ne.irb(0,2) ) then
               hlen = hlen + sqrt(
     &              (x(i1-it1,i2-it2,i3+it3,0)-x(i1,i2,i3,0))**2d0+
     & (x(i1-it1,i2-it2,i3+it3,1)-x(i1,i2,i3,1))**2d0 )/len
            endif

            ghostrpos = x(i1g,i2g,i3g,1).gt.0d0
c            if ( hlen.lt.0.5d0 ) then
c               scale = 4d0*hlen 
c            else
c               scale = 4d0*(1d0-hlen)
c            endif

c            scale = (hlen*6d0 - hlen*hlen*6d0)
c            if ( isaxi.gt.0 ) scale = scale/xyz(i1,i2,i3,2)
            iscorner = .false.
            if ( i1.eq.indexRange(0,0) .and. i2.eq.indexRange(0,1))then
               iscorner =
     &              (bc(0,0).eq.noslipwall.or.bc(0,0).eq.slipwall).and.
     &              (bc(0,1).eq.noslipwall.or.bc(0,1).eq.slipwall)
            else if(i1.eq.indexRange(0,0).and.i2.eq.indexRange(1,1))
     & then
               iscorner =
     &              (bc(0,0).eq.noslipwall.or.bc(0,0).eq.slipwall).and.
     &              (bc(1,1).eq.noslipwall.or.bc(1,1).eq.slipwall)
            else if(i1.eq.indexRange(1,0).and.i2.eq.indexRange(0,1))
     & then
               iscorner =
     &              (bc(1,0).eq.noslipwall.or.bc(1,0).eq.slipwall).and.
     &              (bc(0,1).eq.noslipwall.or.bc(0,1).eq.slipwall)
            else if(i1.eq.indexRange(1,0).and.i2.eq.indexRange(1,1))
     & then
               iscorner =
     &              (bc(1,0).eq.noslipwall.or.bc(1,0).eq.slipwall).and.
     &              (bc(1,1).eq.noslipwall.or.bc(1,1).eq.slipwall)
            endif
            skipslip = .false. !iscorner

            if ( .false. ) then
               iscorner = (
     & (i1.eq.indexRange(0,0) .and. i2.eq.indexRange(0,1) .and.
     &           bc(0,0).eq.noslipwall .and. bc(0,1).eq.noslipwall).or.
     & (i1.eq.indexRange(0,0) .and. i2.eq.indexRange(1,1) .and.
     &           bc(0,0).eq.noslipwall .and. bc(1,1).eq.noslipwall).or.
     & (i1.eq.indexRange(1,0) .and. i2.eq.indexRange(0,1) .and.
     &           bc(1,0).eq.noslipwall .and. bc(0,1).eq.noslipwall).or.
     & (i1.eq.indexRange(1,0) .and. i2.eq.indexRange(1,1) .and.
     &           bc(1,0).eq.noslipwall .and. bc(1,1).eq.noslipwall))
            endif

c             iscorner = .false.
c            iscorner = side.eq.0 !.and. bc(side,axis).ne.subsonicinflow
c            iscorner = side.eq.0 .or. iscorner

            a = mod(axis+1,ndim)
            norm(1) = (2d0*a-1d0)*rsxy(i1,i2,i3,a,1)
            norm(2) = (1d0-2d0*a)*rsxy(i1,i2,i3,a,0)
            tang(1) = (2d0*axis-1d0)*rsxy(i1,i2,i3,axis,1)
            tang(2) = (1d0-2d0*axis)*rsxy(i1,i2,i3,axis,0)

            norm(1) = norm(1)/sqrt(norm(1)*norm(1)+norm(2)*norm(2))
            norm(2) = norm(2)/sqrt(norm(1)*norm(1)+norm(2)*norm(2))
            tang(1) = tang(1)/sqrt(tang(1)*tang(1)+tang(2)*tang(2))
            tang(2) = tang(2)/sqrt(tang(1)*tang(1)+tang(2)*tang(2))
c            print *, grid,side,axis,tang(1),tang(2)
c           outward pointing normal
            norm(1) = (2d0*side-1d0)*norm(1)
            norm(2) = (2d0*side-1d0)*norm(2)
            tang(1) = (2d0*side-1d0)*tang(1)
            tang(2) = (2d0*side-1d0)*tang(2)

            vect(1) = norm(1)
            vect(2) = norm(2)
c            vect(1) = tang(1)
c            vect(2) = tang(2)

c         if ( bc(side,axis).eq.subsonicoutflow ) then !.or.
c     &        bc(side,axis).eq.subsonicinflow ) then
c         print *,"GRID = ",grid," SIDE = ",side," AXIS = ",axis
c         print *,norm(1),norm(2)
c         print *,"rho,uvel,vvel : ",rho,uvel,vvel
c         print *,"port : ",port
c         endif

            if ( cfrhs.eq.0 ) then
c           reset coefficient matrix
            do m3=-hwidth3,hwidth3
            do m2=-hwidth, hwidth
            do m1=-hwidth, hwidth
               do c=0,ncmp-1

                  do e=rc,tc
                     coeff(icf(m1,m2,m3,e,c),i1g,i2g,i3g)=0d0
                     coeff(icf(m1,m2,m3,e,c),i1gg,i2gg,i3gg)=0d0
                  end do

                  if ( bc(side,axis).eq.noslipwall .or.
     &                 bc(side,axis).eq.subsonicinflow .or.
     &                 (bc(side,axis).eq.subsonicoutflow.and.
     &                  port.lt.1e-10)) then
                     if ( .not. sliponwall ) then
                     do e=uc,vc!max(vc,wc)
                        coeff(icf(m1,m2,m3,e,c),i1,i2,i3)=0d0
                     end do
                     endif
                  endif
                  if ( (withswirl.gt.0 .or. nd.eq.3) .and.
!     &                    bc(side,axis).ne.slipwall .and.
     &                    bc(side,axis).ne.subsonicoutflow) then
                     coeff(icf(m1,m2,m3,wc,c),i1,i2,i3)=0d0
                  endif

c                  if ( (bc(side,axis).eq.subsonicinflow ) .or.
c     &                 (bc(side,axis).eq.subsonicoutflow .and.
c     &                 port.gt.1e-10 )
c     &                 .or. iscorner ) then
                  if ( .not. sliponwall ) then
                  if ( .not. (iscorner .and. axis.eq.1) ) then
                     coeff(icf(m1,m2,m3,rc,c),i1,i2,i3) = 0d0
                  endif
                  endif

                  if ( (.not. useneumanntemp) .and.
     &                    (bc(side,axis).ne.subsonicoutflow) ) then
                     coeff(icf(m1,m2,m3,tc,c),i1,i2,i3)=0d0
                  endif

               enddo !c
            enddo !m1
            enddo !m2
            enddo !m3
            else
c           reset the rhs
               do e=rc,tc
                  rhs(i1g,i2g,i3g,e) = 0d0
               end do
               rhs(i1gg,i2gg,i3gg,rc) = 0d0

c               if ( bc(side,axis).eq.noslipwall ) then
                  if ( bc(side,axis).eq.noslipwall .or.
     &                 bc(side,axis).eq.subsonicinflow .or.
     &                 (bc(side,axis).eq.subsonicoutflow.and.
     &                  port.lt.1e-10)) then
                     if ( .not. sliponwall ) then
                        do e=uc,vc !max(vc,wc)
                           rhs(i1,i2,i3,e) = 0d0
                        end do
                     endif
                  endif
               if ( (withswirl.gt.0 .or.nd.eq.3).and.
!     &                    bc(side,axis).ne.slipwall.and.
     &                    bc(side,axis).ne.subsonicoutflow) then
                  rhs(i1,i2,i3,wc) = 0d0
               endif
               if ( .not. sliponwall ) then
               if ( .not. (iscorner .and. axis.eq.1) ) then
                  rhs(i1,i2,i3,rc) = 0d0
               endif
               endif
            endif ! if coeff or rhs

            if ( cfrhs.eq.0 ) then
c           SET THE COEFFICIENT MATRIX

c     PRESSURE IS EXTRAPOLATED AT THE GHOST, INFLOW SPECIFIED, MIXED BC ON P FOR SOME OUTFLOW
               eqn = rc

!     &              .not.(bc(side,axis).eq.subsonicoutflow .and.
!     &              port.gt.1e-10)
               if ( .true. ) then
c                 extrapolate the pressure


                  if ( (.not.extrapponbdy).or.
     &                 ( bc(side,axis).eq.subsonicoutflow .and.
     &                 port.gt.1e-10 ).or.
     &                 (bc(side,axis).eq.subsonicinflow
     &                 .and.usedirichletrhoinflow) )then
                            i1 = i1g
                            i2 = i2g
                            i3 = i3g
c                     if ( bc(side,axis).ne.subsonicinflow ) then
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (1d0)*u(i1+(0)+off(1)
     & ,i2+(0)+off(2),i3+(0)+off(3),tc)
                           coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (1d0)*u(i1+(0)+off(1)
     & ,i2+(0)+off(2),i3+(0)+off(3),rc)
c      linearize terms like -2d0 * u(rc)* u(tc) at the grid point offset by is1, is2, is3
                           coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,
     & i3) = coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) + (-2d0)*u(
     & i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),tc)
                           coeff(icf((is1),(is2),(is3),eqn,tc),i1,i2,
     & i3) = coeff(icf((is1),(is2),(is3),eqn,tc),i1,i2,i3) + (-2d0)*u(
     & i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),rc)
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,i3)
     &  + (1d0)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(
     & 3),tc)
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,tc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,tc),i1,i2,i3)
     &  + (1d0)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(
     & 3),rc)
c                     if ( bc(side,axis).eq.subsonicinflow ) then
c     specified density at inflow
c                        coeff(icf(0,0,0,rc,rc),i1,i2,i3) = 1d0
c                     endif
c                     endif
                     if ( bc(side,axis).eq.subsonicinflow .and.
     &                    usedirichletrhoinflow ) then
                               i1 = i1l
                               i2 = i2l
                               i3 = i3l
                        coeff(icf(0,0,0,rc,rc),i1,i2,i3) = 1d0
                     endif
                  else
                            i1 = i1l
                            i2 = i2l
                            i3 = i3l
                     rtmp = exo2(i1g,i2g,i3g,is1,is2,is3,rc)
                     if ( .false. ) then
                        if ( rtmp.gt.0d0 .or. .true.) then
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by -is1, -is2, -is3
                                 coeff(icf((-is1),(-is2),(-is3),eqn,rc)
     & ,i1,i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,rc),i1,i2,i3) +
     &  (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),tc)
                                 coeff(icf((-is1),(-is2),(-is3),eqn,tc)
     & ,i1,i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,tc),i1,i2,i3) +
     &  (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)
c      linearize terms like -2d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                                 coeff(icf((0),(0),(0),eqn,rc),i1,i2,
     & i3) = coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (-2d0)*u(i1+(0)
     & +off(1),i2+(0)+off(2),i3+(0)+off(3),tc)
                                 coeff(icf((0),(0),(0),eqn,tc),i1,i2,
     & i3) = coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (-2d0)*u(i1+(0)
     & +off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by is1, is2, is3
                                 coeff(icf((is1),(is2),(is3),eqn,rc),
     & i1,i2,i3) = coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) + (
     & 1d0)*u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),tc)
                                 coeff(icf((is1),(is2),(is3),eqn,tc),
     & i1,i2,i3) = coeff(icf((is1),(is2),(is3),eqn,tc),i1,i2,i3) + (
     & 1d0)*u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),rc)
                        else
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by -is1, -is2, -is3
                                 coeff(icf((-is1),(-is2),(-is3),eqn,rc)
     & ,i1,i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,rc),i1,i2,i3) +
     &  (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),tc)
                                 coeff(icf((-is1),(-is2),(-is3),eqn,tc)
     & ,i1,i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,tc),i1,i2,i3) +
     &  (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)
c      linearize terms like -1d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                                 coeff(icf((0),(0),(0),eqn,rc),i1,i2,
     & i3) = coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (-1d0)*u(i1+(0)
     & +off(1),i2+(0)+off(2),i3+(0)+off(3),tc)
                                 coeff(icf((0),(0),(0),eqn,tc),i1,i2,
     & i3) = coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (-1d0)*u(i1+(0)
     & +off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                        endif
                     else if ( .not. sliponwall ) then
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by -is1, -is2, -is3
                              coeff(icf((-is1),(-is2),(-is3),eqn,rc),
     & i1,i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,rc),i1,i2,i3) + 
     & (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),tc)
                              coeff(icf((-is1),(-is2),(-is3),eqn,tc),
     & i1,i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,tc),i1,i2,i3) + 
     & (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)
c      linearize terms like -3d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                              coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) 
     & = coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (-3d0)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),tc)
                              coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) 
     & = coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (-3d0)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
c      linearize terms like 3d0 * u(rc)* u(tc) at the grid point offset by is1, is2, is3
                              coeff(icf((is1),(is2),(is3),eqn,rc),i1,
     & i2,i3) = coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) + (3d0)*
     & u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),tc)
                              coeff(icf((is1),(is2),(is3),eqn,tc),i1,
     & i2,i3) = coeff(icf((is1),(is2),(is3),eqn,tc),i1,i2,i3) + (3d0)*
     & u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),rc)
c      linearize terms like -1d0 * u(rc)* u(tc) at the grid point offset by 2*is1, 2*is2, 2*is3
                              coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc)
     & ,i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,
     & i3) + (-1d0)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+
     & off(3),tc)
                              coeff(icf((2*is1),(2*is2),(2*is3),eqn,tc)
     & ,i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,tc),i1,i2,
     & i3) + (-1d0)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+
     & off(3),rc)
                     else

                        coeff(icf(0,0,0,rc,rc),i1g,i2g,i3g) = 1d0
                        coeff(icf(2*is1,2*is2,2*is3,rc,rc),i1g,i2g,i3g)
     & =-1d0

                     endif

                  endif
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l

               endif

               if ( bc(side,axis).eq.subsonicoutflow .and.
     &              port.gt.1e-10 ) then
c              mixed bc on pressure for outflow
                  eqn = rc
                  pfac = bd(tc+ncmp,side,axis,grid)
                  pdotn= bd(tc+ncmp*2,side,axis,grid)
                  prhs= bd(tc,side,axis,grid)
c      linearize terms like pfac * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (pfac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),tc)
                        coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (pfac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),rc)
c     coefficients for a central difference approximation to pdotn*norm(1) * u[rc] u[tc]_X
c                 u[rc]^{n+1} * u[tc]^n_X
                                    coeff(icf(0,0,0,eqn,rc),i1,i2,i3) 
     & = coeff(icf(0,0,0,eqn,rc),i1,i2,i3) + (pdotn*norm(1)) * ux(i1,
     & i2,i3,tc,1)
c                 u[rc]^n * (r_X u[tc]_r^{n+1} + s_X u[tc]_s^{n+1} )
c                 r - part
                                    coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) + (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(1)
                                    coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) - (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(1)
c                 s - part
                                    coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) + (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(2)
                                    coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) - (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(2)
c     coefficients for a central difference approximation to pdotn*norm(1) * u[tc] u[rc]_X
c                 u[tc]^{n+1} * u[rc]^n_X
                                    coeff(icf(0,0,0,eqn,tc),i1,i2,i3) 
     & = coeff(icf(0,0,0,eqn,tc),i1,i2,i3) + (pdotn*norm(1)) * ux(i1,
     & i2,i3,rc,1)
c                 u[tc]^n * (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c                 r - part
                                    coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(1)
                                    coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) - (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(1)
c                 s - part
                                    coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(2)
                                    coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) - (pdotn*norm(1)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(2)
c     coefficients for a central difference approximation to pdotn*norm(2) * u[rc] u[tc]_X
c                 u[rc]^{n+1} * u[tc]^n_X
                                    coeff(icf(0,0,0,eqn,rc),i1,i2,i3) 
     & = coeff(icf(0,0,0,eqn,rc),i1,i2,i3) + (pdotn*norm(2)) * ux(i1,
     & i2,i3,tc,2)
c                 u[rc]^n * (r_X u[tc]_r^{n+1} + s_X u[tc]_s^{n+1} )
c                 r - part
                                    coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) + (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(1)
                                    coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) - (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(1)
c                 s - part
                                    coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) + (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(2)
                                    coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) 
     & =  coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) - (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),rc)*dri2(2)
c     coefficients for a central difference approximation to pdotn*norm(2) * u[tc] u[rc]_X
c                 u[tc]^{n+1} * u[rc]^n_X
                                    coeff(icf(0,0,0,eqn,tc),i1,i2,i3) 
     & = coeff(icf(0,0,0,eqn,tc),i1,i2,i3) + (pdotn*norm(2)) * ux(i1,
     & i2,i3,rc,2)
c                 u[tc]^n * (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c                 r - part
                                    coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(1)
                                    coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) - (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(1)
c                 s - part
                                    coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(2)
                                    coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) - (pdotn*norm(2)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+off(1),i2+off(2),i3+
     & off(3),tc)*dri2(2)
                  if ( .false. ) then
                  coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,tc),i1,i2,i3) +
     &                 pdotn*norm(1)*ux(i1l,i2l,i3l,rc,1) +
     &                 pdotn*norm(2)*ux(i1l,i2l,i3l,rc,2)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 pdotn*norm(1)*ux(i1l,i2l,i3l,tc,1) +
     &                 pdotn*norm(2)*ux(i1l,i2l,i3l,tc,2)
                  endif
c                  print *,i1,i2,pfac,pdotn
               else if ( (.not.sliponwall) .and.
     &.not.(bc(side,axis).eq.subsonicinflow.and.
     &                 usedirichletrhoinflow)) then
c     normal momentum equation at other bcs
                  if ( extrapponbdy .or.
     &                 (bc(side,axis).eq.subsonicinflow.and.
     &                 usedirichletrhoinflow)) then
                  off(axis+1) = iss(axis)
                         i1 = i1g
                         i2 = i2g
                         i3 = i3g
                  else
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l
                  endif

                  eqn = rc
                  do d=uc,uc+ndim-1
c                 convective derivative (will have RHS)
                     if ( .true. ) then
                     do cc=uc, uc+ndim-1
c     coefficients for a central difference approximation to vect(d) * u[cc] u[d]_X
c                 u[cc]^{n+1} * u[d]^n_X
                                          coeff(icf(0,0,0,eqn,cc),i1,
     & i2,i3) = coeff(icf(0,0,0,eqn,cc),i1,i2,i3) + (vect(d)) * ux(i1,
     & i2,i3,d,(cc-uc+1))
c                 u[cc]^n * (r_X u[d]_r^{n+1} + s_X u[d]_s^{n+1} )
c                 r - part
                                          coeff(icf(+1,0,0,eqn,d),i1,
     & i2,i3) =  coeff(icf(+1,0,0,eqn,d),i1,i2,i3) + (vect(d)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,(cc-uc+1))*u(i1+off(1),i2+off(
     & 2),i3+off(3),cc)*dri2(1)
                                          coeff(icf(-1,0,0,eqn,d),i1,
     & i2,i3) =  coeff(icf(-1,0,0,eqn,d),i1,i2,i3) - (vect(d)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),1,(cc-uc+1))*u(i1+off(1),i2+off(
     & 2),i3+off(3),cc)*dri2(1)
c                 s - part
                                          coeff(icf(0,+1,0,eqn,d),i1,
     & i2,i3) =  coeff(icf(0,+1,0,eqn,d),i1,i2,i3) + (vect(d)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,(cc-uc+1))*u(i1+off(1),i2+off(
     & 2),i3+off(3),cc)*dri2(2)
                                          coeff(icf(0,-1,0,eqn,d),i1,
     & i2,i3) =  coeff(icf(0,-1,0,eqn,d),i1,i2,i3) - (vect(d)) * rx(
     & i1+off(1),i2+off(2),i3+off(3),2,(cc-uc+1))*u(i1+off(1),i2+off(
     & 2),i3+off(3),cc)*dri2(2)
                     end do     ! cc (each vel. component)
                     endif

c                 temperature part of pressure gradient
                     if ( .false. .and. side.eq.1
     &                    ) then !usestrik ) then
                        alpha = rprm(18)
c     coefficients for a 3rd order  difference approximation to vect(d)*gm2 * u[tc]_X
c                  (r_X u[tc]_r^{n+1} + s_X u[tc]_s^{n+1} )
c                 r - part
                              coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) + (vect(d)*gm2) * (alpha-
     & .5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,(d-uc+1))*dri(1)
                              coeff(icf( 0,0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf( 0,0,0,eqn,tc),i1,i2,i3) - (vect(d)*gm2) * (3d0*
     & alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,(d-uc+1))*dri(1)
                              coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) + (vect(d)*gm2) * (.5d0+3d0*
     & alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,(d-uc+1))*dri(1)
                              coeff(icf(+2,0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(+2,0,0,eqn,tc),i1,i2,i3) - (vect(d)*gm2) * (alpha)   
     &       *rx(i1+off(1),i2+off(2),i3+off(3),1,(d-uc+1))*dri(1)
c                 s - part
                              coeff(icf(0 ,-1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) + (vect(d)*gm2) * (alpha-
     & .5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,(d-uc+1))*dri(2)
                              coeff(icf(0 , 0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0, 0,0,eqn,tc),i1,i2,i3) - (vect(d)*gm2) * (3d0*
     & alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,(d-uc+1))*dri(2)
                              coeff(icf(0 ,+1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) + (vect(d)*gm2) * (.5d0+3d0*
     & alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,(d-uc+1))*dri(2)
                              coeff(icf(0 ,+2,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0,+2,0,eqn,tc),i1,i2,i3) - (vect(d)*gm2) * (alpha)   
     &       *rx(i1+off(1),i2+off(2),i3+off(3),2,(d-uc+1))*dri(2)
                        alpha = 0d0
                     else
c     coefficients for a  difference approximation to vect(d)*gm2 * u[tc]_X
c     r-part
                              coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) + (vect(d)*gm2)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),1,(d-uc+1))*dri2(1)
                              coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) - (vect(d)*gm2)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),1,(d-uc+1))*dri2(1)
c     s-part
                              coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) + (vect(d)*gm2)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,(d-uc+1))*dri2(2)
                              coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) - (vect(d)*gm2)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,(d-uc+1))*dri2(2)
                     endif

                     if ( .false. ) then
c                 density part of pressure gradient
c     we linearize a discretized version of gm2*u[tc]*log(u[rc])_c
                        lur = (dlog(dabs(u(i1l+1,i2l,i3l,rc)))-dlog(
     & dabs(u(i1l-1,i2l,i3l,rc))))
                        lus = (dlog(dabs(u(i1l,i2l+1,i3l,rc)))-dlog(
     & dabs(u(i1l,i2l-1,i3l,rc))))

c           gm2*u[tc]^{n+1}*log(u[rc]^n)_c
                        coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &                       coeff(icf(0,0,0,eqn,tc),i1,i2,i3) +
     & vect(d)*gm2*( rx(i1l,i2l,i3l,1,d)*dri2(1)*lur +
     &                       rx(i1l,i2l,i3l,2,d)*dri2(2)*lus )

c           the n+1 part of the
c            gm2*u[tc]^{n}*log(u[rc]^{n+1})_c
c           linearization is
c            gm2*u[tc]^{n}*( rx(1,cc)*dri2*( u[rc]^{n+1}_{i+1}/u[rc]^n{i+1} - 
c                                            u[rc]^{n+1}_{i-1}/u[rc]^n{i-1} ) +
c                            rx(2,cc)*dri2*( u[rc]^{n+1}_{j+1}/u[rc]^n{j+1} - 
c                                            u[rc]^{n+1}_{j-1}/u[rc]^n{j-1} ) )
c     
                        coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) =
     &                       coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) +
     & vect(d)*gm2*u(i1l,i2l,i3l,tc)*dri2(1)*rx(i1l,i2l,i3l,1,d)/
     &                       u(i1l+1,i2l,i3l,rc)

                        coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) =
     &                       coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) -
     & vect(d)*gm2*u(i1l,i2l,i3l,tc)*dri2(1)*rx(i1l,i2l,i3l,1,d)/
     &                       u(i1l-1,i2l,i3l,rc)

                        coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) =
     &                       coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) +
     & vect(d)*gm2*u(i1l,i2l,i3l,tc)*dri2(2)*rx(i1l,i2l,i3l,2,d)/
     &                       u(i1l,i2l+1,i3l,rc)

                        coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) =
     &                       coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) -
     & vect(d)*gm2*u(i1l,i2l,i3l,tc)*dri2(2)*rx(i1l,i2l,i3l,2,d)/
     &                       u(i1l,i2l-1,i3l,rc)

                        if ( usestrik ) then
c     strikwerda-like correction term
c     coefficients for a 3rd order  difference approximation to (vect(d)*gm2/u(i1l,i2l,i3l,rc)) * u[tc] u[rc]_X
c                 u[tc]^{n+1} * u[rc]^n_X
                               coeff(icf(0,0,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(0,0,0,eqn,tc),i1,i2,i3) + ((vect(d)*gm2/u(i1l,i2l,
     & i3l,rc))) * ux4m(i1,i2,i3,rc,d)
c                 u[tc]^n * (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c     coefficients for a 3rd order  difference approximation to ((vect(d)*gm2/u(i1l,i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc) * u[rc]_X
c                  (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c                 r - part
                                     coeff(icf(-1,0,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) + (((vect(d)*gm2/u(i1l,
     & i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (alpha-
     & .5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*dri(1)
                                     coeff(icf( 0,0,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) - (((vect(d)*gm2/u(i1l,
     & i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (3d0*
     & alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*dri(1)
                                     coeff(icf(+1,0,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + (((vect(d)*gm2/u(i1l,
     & i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (.5d0+3d0*
     & alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*dri(1)
                                     coeff(icf(+2,0,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) - (((vect(d)*gm2/u(i1l,
     & i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (alpha)   
     &       *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*dri(1)
c                 s - part
                                     coeff(icf(0 ,-1,0,eqn,rc),i1,i2,
     & i3) =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) + (((vect(d)*gm2/u(
     & i1l,i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (
     & alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*dri(2)
                                     coeff(icf(0 , 0,0,eqn,rc),i1,i2,
     & i3) =  coeff(icf(0, 0,0,eqn,rc),i1,i2,i3) - (((vect(d)*gm2/u(
     & i1l,i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (3d0*
     & alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*dri(2)
                                     coeff(icf(0 ,+1,0,eqn,rc),i1,i2,
     & i3) =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + (((vect(d)*gm2/u(
     & i1l,i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (.5d0+
     & 3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*dri(2)
                                     coeff(icf(0 ,+2,0,eqn,rc),i1,i2,
     & i3) =  coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) - (((vect(d)*gm2/u(
     & i1l,i2l,i3l,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (
     & alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*dri(2)
c                 r - part
c      coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) + ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) - ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) - ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c                 s - part
c      coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) + ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,0,0,eqn,rc),i1,i2,i3)  - ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) - ((vect(d)*gm2/u(i1l,i2l,i3l,rc))) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c     print *,i1l,i2l,vect(d),d

                           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                          coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     & vect(d)*gm2*(ux4m(i1l,i2l,i3l,rc,d)-ux(i1l,i2l,i3l,rc,d))*
     & u(i1l,i2l,i3l,tc)/(u(i1l,i2l,i3l,rc)*u(i1l,i2l,i3l,rc))

c     subtract off the part we don't need (done above with log(rho))
c     coefficients for a central difference approximation to (-vect(d)*gm2/u(i1l,i2l,i3l,rc)) * u[tc] u[rc]_X
c                 u[tc]^{n+1} * u[rc]^n_X
                                            coeff(icf(0,0,0,eqn,tc),i1,
     & i2,i3) = coeff(icf(0,0,0,eqn,tc),i1,i2,i3) + ((-vect(d)*gm2/u(
     & i1l,i2l,i3l,rc))) * ux(i1,i2,i3,rc,d)
c                 u[tc]^n * (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c                 r - part
                                            coeff(icf(+1,0,0,eqn,rc),
     & i1,i2,i3) =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + ((-vect(d)*
     & gm2/u(i1l,i2l,i3l,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,d)
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)*dri2(1)
                                            coeff(icf(-1,0,0,eqn,rc),
     & i1,i2,i3) =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) - ((-vect(d)*
     & gm2/u(i1l,i2l,i3l,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,d)
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)*dri2(1)
c                 s - part
                                            coeff(icf(0,+1,0,eqn,rc),
     & i1,i2,i3) =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + ((-vect(d)*
     & gm2/u(i1l,i2l,i3l,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,d)
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)*dri2(2)
                                            coeff(icf(0,-1,0,eqn,rc),
     & i1,i2,i3) =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) - ((-vect(d)*
     & gm2/u(i1l,i2l,i3l,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,d)
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)*dri2(2)

                        endif
                     else
c                        if ( side.eq.1 ) then
c                           alpha = rprm(18)
c                           usestrik = .true.
c                        endif

c     coefficients for a 3rd order  difference approximation to vect(d)*gm2 *  u[tc]*(log(u[rc]))_X
c                 u[tc]^{n+1} * u[rc]^n_X
                              coeff(icf(0,0,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(0,0,0,eqn,tc),i1,i2,i3) + (vect(d)*gm2) * logux4m(i1+
     & off(1),i2+off(2),i3+off(3),rc,d)
c                 r - part
                              coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) + (vect(d)*gm2) * (alpha-
     & .5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+
     & off(2),i3+off(3),tc)*dri(1)/u(i1-1+off(1),i2+off(2),i3+off(3),
     & rc)
                              coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) - (vect(d)*gm2) * (3d0*
     & alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),
     & i2+off(2),i3+off(3),tc)*dri(1)/u(i1  +off(1),  i2+off(2),i3+
     & off(3),rc)
                              coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + (vect(d)*gm2) * (.5d0+3d0*
     & alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+
     & off(2),i3+off(3),tc)*dri(1)/u(i1+1+off(1),i2+off(2),i3+off(3),
     & rc)
                              if ( usestrik ) then
                              coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) - (vect(d)*gm2) * (alpha)   
     &       *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+
     & off(2),i3+off(3),tc)*dri(1)/u(i1+2+off(1),i2+off(2),i3+off(3),
     & rc)
                              endif
c                 s - part
                              coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) + (vect(d)*gm2) * (alpha-
     & .5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+
     & off(2),i3+off(3),tc)*dri(2)/u(i1+off(1),i2-1+off(2),i3+off(3),
     & rc)
                              coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf(0,0,0,eqn,rc),i1,i2,i3)  - (vect(d)*gm2) * (3d0*
     & alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),
     & i2+off(2),i3+off(3),tc)*dri(2)/u(i1+off(1),i2  +off(2),i3+off(
     & 3),rc)
                              coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + (vect(d)*gm2) * (.5d0+3d0*
     & alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+
     & off(2),i3+off(3),tc)*dri(2)/u(i1+off(1),i2+1+off(2),i3+off(3),
     & rc)
                              if ( usestrik ) then
                              coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) =  
     & coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) - (vect(d)*gm2) * (alpha)   
     &       *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+
     & off(2),i3+off(3),tc)*dri(2)/u(i1+off(1),i2+2+off(2),i3+off(3),
     & rc)
                              endif
c                        if ( side.eq.1 ) then
c                           alpha = 0
c                           usestrik = .false.
c                        endif

                     endif

                  end do ! each direction
                  if ( withswirl.gt.0 ) then
c                 -u[wc]^2/r 
c                 will contribute to RHS
                     if ( .true. ) then
                     coeff(icf(0,0,0,eqn,wc),i1,i2,i3) =
     &                coeff(icf(0,0,0,eqn,wc),i1,i2,i3) -
     & vect(2)*2d0 * u(i1l,i2l,i3l,wc)/xyz(i1l,i2l,i3l,2)
                     else
                        do d=1,nd
                              if ( .true. ) then
                              drar = (-vect(2))*dri2(1)*jaci*jac(i1+
     & off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(
     & 3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                    coeff(icf((-1),(0),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                    coeff(icf((-1),(0),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                              drar = (-vect(2))*dri2(1)*jaci*jac(i1+
     & off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                    coeff(icf((+1),(0),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                    coeff(icf((+1),(0),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                              drar = (-vect(2))*dri2(2)*jaci*jac(i1+
     & off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(
     & 3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+
     & off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                    coeff(icf((0),(-1),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                    coeff(icf((0),(-1),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                              drar = (-vect(2))*dri2(2)*jaci*jac(i1+
     & off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                    coeff(icf((0),(+1),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                    coeff(icf((0),(+1),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + (drar)*u(
     & i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                              else if ( .false. ) then
                                 drar = (-vect(2))*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                      coeff(icf((-1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                                      coeff(icf((-1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                      coeff(icf((+1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                                      coeff(icf((+1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                      coeff(icf((0),(-1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)
                                      coeff(icf((0),(-1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                      coeff(icf((0),(+1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),wc)
                                      coeff(icf((0),(+1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + (
     & drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),wc)
                              else
c      linearize terms like ((-vect(2))/(xyz(i1,i2,i3,2))) * u(wc)* u(wc) at the grid point offset by 0, 0, 0
                                       coeff(icf((0),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (((-vect(
     & 2))/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                                       coeff(icf((0),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (((-vect(
     & 2))/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                              endif
                        enddo
                     endif
                  endif
c
c     MOMENTUM EQUATION VISCOUS TERMS
c     
                  if ( .true. ) then
c     u-momentum equation, the terms are
c     (4 u[uc]_xx/3 + u[uc]_yy + u[vc]_xy/3 + axifac*(u[uc]_y/y + u[vc]_x/y/3))/u[rc]/ren
c     
c     UXYC is a bpp macro that fills in the 9 point stencil for the second derivatives (see top of file)

c     u[uc]_xx/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-vect(1)*f43*oren/u(i1l,i2l,i3l,rc)) * u[uc]_{XY}
                              coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,
     & 1,1)*dri2(2))
                              coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                              coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,
     & 1,1)*dri2(1))
                              coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(-vect(1)*f43*
     & oren/u(i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)
     & *rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,1)*dri(2)*dri(2))
                              coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,
     & 1,1)*dri2(1))
                              coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,
     & 1,1)*dri2(2))
                              coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 (-vect(1)*f43*oren*uxx(i1l,i2l,i3l,uc,1,1))/
     &                 (u(i1l,i2l,i3l,rc)**2)

c     u[uc]_yy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-vect(1)*oren/u(i1l,i2l,i3l,rc)) * u[uc]_{XY}
                              coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3)
     & ,1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                              coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,2)
     & *dri2(2))
                              coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3)
     & ,1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                              coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,2)
     & *dri2(1))
                              coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(-vect(1)*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),
     & i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*
     & dri(2)*dri(2))
                              coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,2)
     & *dri2(1))
                              coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                              coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,2)
     & *dri2(2))
                              coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (-vect(1)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 (-vect(1)*oren*uxx(i1l,i2l,i3l,uc,2,2))/
     &                 (u(i1l,i2l,i3l,rc)**2)


c     u[vc]_xy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-vect(1)*f13*oren/u(i1l,i2l,i3l,rc)) * u[vc]_{XY}
                              coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,
     & 1,2)*dri2(2))
                              coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                              coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,
     & 1,2)*dri2(1))
                              coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(-vect(1)*f13*
     & oren/u(i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)
     & *rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,2)*dri(2)*dri(2))
                              coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,
     & 1,2)*dri2(1))
                              coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,
     & 1,2)*dri2(2))
                              coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (-vect(1)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*
     & dri2(2)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 (-vect(1)*f13*oren*uxx(i1l,i2l,i3l,vc,1,2))/
     &                 (u(i1l,i2l,i3l,rc)**2)

                  if ( isaxi.gt.0 .and. xyz(i1l,i2l,i3l,2).gt.0d0 ) 
     & then

c     u[uc]_y/y/u[rc] linearization (will have RHS)
                     coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     & vect(1)*oren*ux(i1l,i2l,i3l,uc,2)/xyz(i1l,i2l,i3l,2)/(u(i1l,
     & i2l,i3l,rc)**2)

c     coefficients for a  difference approximation to (-vect(1)*oren/xyz(i1l,i2l,i3l,2)/u(i1l,i2l,i3l,rc)) * u[uc]_X
c     r-part
                           coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = coeff(
     & icf(+1,0,0,eqn,uc),i1,i2,i3) + ((-vect(1)*oren/xyz(i1l,i2l,i3l,
     & 2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*
     & dri2(1)
                           coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = coeff(
     & icf(-1,0,0,eqn,uc),i1,i2,i3) - ((-vect(1)*oren/xyz(i1l,i2l,i3l,
     & 2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*
     & dri2(1)
c     s-part
                           coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = coeff(
     & icf(0,+1,0,eqn,uc),i1,i2,i3) + ((-vect(1)*oren/xyz(i1l,i2l,i3l,
     & 2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*
     & dri2(2)
                           coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = coeff(
     & icf(0,-1,0,eqn,uc),i1,i2,i3) - ((-vect(1)*oren/xyz(i1l,i2l,i3l,
     & 2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*
     & dri2(2)

c     u[vc]_x/y/uc[rc] linearization (will have RHS)
                     coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     & vect(1)*f13*oren*ux(i1l,i2l,i3l,vc,1)/xyz(i1l,i2l,i3l,2)/(u(
     & i1l,i2l,i3l,rc)**2)

c     coefficients for a  difference approximation to (-vect(1)*f13*oren/xyz(i1l,i2l,i3l,2)/u(i1l,i2l,i3l,rc)) * u[vc]_X
c     r-part
                           coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(+1,0,0,eqn,vc),i1,i2,i3) + ((-vect(1)*f13*oren/xyz(i1l,i2l,
     & i3l,2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,
     & 1)*dri2(1)
                           coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(-1,0,0,eqn,vc),i1,i2,i3) - ((-vect(1)*f13*oren/xyz(i1l,i2l,
     & i3l,2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,
     & 1)*dri2(1)
c     s-part
                           coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(0,+1,0,eqn,vc),i1,i2,i3) + ((-vect(1)*f13*oren/xyz(i1l,i2l,
     & i3l,2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,
     & 1)*dri2(2)
                           coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(0,-1,0,eqn,vc),i1,i2,i3) - ((-vect(1)*f13*oren/xyz(i1l,i2l,
     & i3l,2)/u(i1l,i2l,i3l,rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,
     & 1)*dri2(2)

                  endif

c     v-momentum equation, the terms are
c     (4 u[vc]_yy/3 + u[vc]_xx + u[uc]_xy/3 + axifac*(4/3)*(u[vc]_y/y - u[vc]/y^2))/u[rc]/ren
c     
c     u[vc]_yy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-vect(2)*f43*oren/u(i1l,i2l,i3l,rc)) * u[vc]_{XY}
                              coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,
     & 2,2)*dri2(2))
                              coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                              coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,
     & 2,2)*dri2(1))
                              coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(-vect(2)*f43*
     & oren/u(i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)
     & *rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,2)*dri(2)*dri(2))
                              coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,
     & 2,2)*dri2(1))
                              coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,
     & 2,2)*dri2(2))
                              coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*f43*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 (-vect(2)*f43*oren*uxx(i1l,i2l,i3l,vc,2,2))/
     &                 (u(i1l,i2l,i3l,rc)**2)

c     u[vc]_xx/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-vect(2)*oren/u(i1l,i2l,i3l,rc)) * u[vc]_{XY}
                              coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3)
     & ,1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                              coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,1)
     & *dri2(2))
                              coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3)
     & ,1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                              coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,1)
     & *dri2(1))
                              coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(-vect(2)*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),
     & i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*
     & dri(2)*dri(2))
                              coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,1)
     & *dri2(1))
                              coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                              coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,1)
     & *dri2(2))
                              coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (-vect(2)*oren/u(i1l,
     & i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 (-vect(2)*oren*uxx(i1l,i2l,i3l,vc,1,1))/
     &                 (u(i1l,i2l,i3l,rc)**2)


c     u[uc]_xy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-vect(2)*f13*oren/u(i1l,i2l,i3l,rc)) * u[uc]_{XY}
                              coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,
     & 2,1)*dri2(2))
                              coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                              coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,
     & 2,1)*dri2(1))
                              coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(-vect(2)*f13*
     & oren/u(i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)
     & *rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,1)*dri(2)*dri(2))
                              coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,
     & 2,1)*dri2(1))
                              coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                              coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,
     & 2,1)*dri2(2))
                              coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (-vect(2)*f13*oren/u(
     & i1l,i2l,i3l,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*
     & dri2(2)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 (-vect(2)*f13*oren*uxx(i1l,i2l,i3l,uc,2,1))/
     &                 (u(i1l,i2l,i3l,rc)**2)

                  if ( isaxi.gt.0 .and. xyz(i1l,i2l,i3l,2).gt.0d0 ) 
     & then

c     u[vc]_y/y/u[rc] linearization (will have RHS)
                     coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     & vect(2)*f43*oren*ux(i1l,i2l,i3l,vc,2)/xyz(i1l,i2l,i3l,2)/
     &                    (u(i1l,i2l,i3l,rc)**2)

c     coefficients for a  difference approximation to (-vect(2)*f43*oren/u(i1l,i2l,i3l,rc)/xyz(i1l,i2l,i3l,2)) * u[vc]_X
c     r-part
                           coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(+1,0,0,eqn,vc),i1,i2,i3) + ((-vect(2)*f43*oren/u(i1l,i2l,
     & i3l,rc)/xyz(i1l,i2l,i3l,2)))*rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,2)*dri2(1)
                           coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(-1,0,0,eqn,vc),i1,i2,i3) - ((-vect(2)*f43*oren/u(i1l,i2l,
     & i3l,rc)/xyz(i1l,i2l,i3l,2)))*rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,2)*dri2(1)
c     s-part
                           coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(0,+1,0,eqn,vc),i1,i2,i3) + ((-vect(2)*f43*oren/u(i1l,i2l,
     & i3l,rc)/xyz(i1l,i2l,i3l,2)))*rx(i1+off(1),i2+off(2),i3+off(3),
     & 2,2)*dri2(2)
                           coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(
     & icf(0,-1,0,eqn,vc),i1,i2,i3) - ((-vect(2)*f43*oren/u(i1l,i2l,
     & i3l,rc)/xyz(i1l,i2l,i3l,2)))*rx(i1+off(1),i2+off(2),i3+off(3),
     & 2,2)*dri2(2)

c     -u[vc]/y^2/u[rc] linearization (will have RHS)
                     coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     & vect(2)*f43*oren*u(i1l,i2l,i3l,vc)/(xyz(i1l,i2l,i3l,2)**2)/
     &                    (u(i1l,i2l,i3l,rc)**2)
                     coeff(icf(0,0,0,eqn,vc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,vc),i1,i2,i3) +
     & vect(2)*f43*oren/(xyz(i1l,i2l,i3l,2)**2)/u(i1l,i2l,i3l,rc)

                  endif
                  endif ! add viscous terms

                  off(axis+1)=0
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l

               endif ! what bc to put on the density

               if ( iscorner .and. .false. ) then
c     print *,"corner at ",i1,",  ",i2
                  ic1 = 0
                  ic2 = 0
                  if ( i1.eq.indexRange(0,0) ) then
                     ic1 = 1
                  else if ( i1.eq.indexRange(1,0) )then
                     ic1 = -1
                  endif
                  if ( i2.eq.indexRange(0,1) ) then
                     ic2 = 1
                  else if ( i2.eq.indexRange(1,1) ) then
                     ic2 = -1
                  endif
c     extrapolate the corner point from the interior, diagonally
                  coeff(icf( 0,0,0,rc,rc),i1,i2,i3) = 1d0

                  if ( (2d0*u(i1+ic1,i2+ic2,i3,rc)-
     &                 u(i1+2*ic1,i2+2*ic2,i3,rc)).le.0d0 ) then
                     coeff(icf(ic1,ic2,0,rc,rc),i1,i2,i3) = -1d0
                  else
                     coeff(icf(ic1,ic2,0,rc,rc),i1,i2,i3) = -2d0
                     coeff(icf(2*ic1,2*ic2,0,rc,rc),i1,i2,i3) = 1d0
                  endif
               endif

                      i1 = i1l
                      i2 = i2l
                      i3 = i3l
c           VELOCITY COMPONENTS ON ALL BUT slip and outlflows ARE DIRICHLET
               if (.not.useneumannvel .and.  .not.sliponwall) then
!bc(side,axis).ne.slipwall  ! then 
c     &              ) then
c     &              .and. bc(side,axis).ne.subsonicinflow 
c              no-slip and inflow get dirichlet values on the normal and tangential components
c                 unless the neumann velocity condition is selected
                  do c=uc,uc+nd-1
                   coeff(icf(0,0,0,neq,c),i1,i2,i3) =-norm(c-uc+1) !- because norm points out
                   coeff(icf(0,0,0,teq,c),i1,i2,i3) = tang(c-uc+1)
                  enddo ! each vel component

c              apply the trick that reduces the chance of a zero pivot:
c                let F be the normal flux equation and G the tangential flux equation
c                norm .dot. { F, G }^T gets put into neq
c                tang .dot. { F, G }^T gets put into teq
                                    do m3=-hwidth3,hwidth3
                                    do m2=-hwidth, hwidth
                                    do m1=-hwidth, hwidth
                                       do c=0,ncmp-1
                                          coeff_u = coeff(icf(m1,m2,m3,
     & neq,c),i1,i2,i3)
                                          coeff_v = coeff(icf(m1,m2,m3,
     & teq,c),i1,i2,i3)
                                          coeff(icf(m1,m2,m3,neq,c),i1,
     & i2,i3) =   norm(1)*coeff_u + norm(2)*coeff_v
                                          coeff(icf(m1,m2,m3,teq,c),i1,
     & i2,i3) =   tang(1)*coeff_u + tang(2)*coeff_v
                                       end do
                                    end do
                                    end do
                                    end do

               else if ( sliponwall ) then
!bc(side,axis).eq.slipwall .and. .not. skipslip ) then
!     &                  bc(side,axis).eq.subsonicinflow ) then
c                 project out the normal component from the slipwall bdy equation
                  do m3=-hwidth3,hwidth3
                  do m2=-hwidth, hwidth
                  do m1=-hwidth, hwidth
                     do c=0,ncmp-1

                        coeff_u = coeff(icf(m1,m2,m3,uc,c),i1,i2,i3)
                        coeff_v = coeff(icf(m1,m2,m3,vc,c),i1,i2,i3)
                        coeff(icf(m1,m2,m3,neq,c),i1,i2,i3) = 0d0
                        coeff(icf(m1,m2,m3,teq,c),i1,i2,i3) =
     &                       tang(1)*coeff_u + tang(2)*coeff_v
c     if ( i2.eq.0 ) then
c     print *,i1,i2,c,tang(1)*coeff_u,tang(2)*coeff_v
c     endif
                     end do
                  end do
                  end do
                  end do
c               the following section was uncommented 080110
                  if ( .true. ) then
                  coeff(icf(0,0,0,neq,uc),i1,i2,i3) =
     &                 -norm(1)!- because norm points out
                  coeff(icf(0,0,0,neq,vc),i1,i2,i3) =
     &                 -norm(2)!- because norm points out

c              apply the trick that reduces the chance of a zero pivot:
c                let F be the normal flux equation and G the tangential flux equation
c                norm .dot. { F, G }^T gets put into neq
c                tang .dot. { F, G }^T gets put into teq
                                    do m3=-hwidth3,hwidth3
                                    do m2=-hwidth, hwidth
                                    do m1=-hwidth, hwidth
                                       do c=0,ncmp-1
                                          coeff_u = coeff(icf(m1,m2,m3,
     & neq,c),i1,i2,i3)
                                          coeff_v = coeff(icf(m1,m2,m3,
     & teq,c),i1,i2,i3)
                                          coeff(icf(m1,m2,m3,neq,c),i1,
     & i2,i3) =   norm(1)*coeff_u + norm(2)*coeff_v
                                          coeff(icf(m1,m2,m3,teq,c),i1,
     & i2,i3) =   tang(1)*coeff_u + tang(2)*coeff_v
                                       end do
                                    end do
                                    end do
                                    end do
                  endif
               else if ( .false. .and. port.lt.1e-10 .and. bc(side,
     & axis).eq.subsonicoutflow ) then
c                 outflow condition with specified mass flux
!!! THIS SECTION NEVER GETS CALLED, IS SUBSONICOUTFLOW WITH MASS FLUX BROKEN??!!
                  eqn = neq
c      linearize terms like norm(1) * u(uc)* u(rc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) + (norm(1))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (norm(1))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),uc)
c      linearize terms like norm(2) * u(vc)* u(rc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) + (norm(2))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (norm(2))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),vc)

                  eqn = teq
c      linearize terms like tang(1) * u(uc)* u(rc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) + (tang(1))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (tang(1))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),uc)
c      linearize terms like tang(2) * u(vc)* u(rc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) + (tang(2))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (tang(2))*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),vc)

c              apply the trick that reduces the chance of a zero pivot:
c                let F be the normal flux equation and G the tangential flux equation
c                norm .dot. { F, G }^T gets put into neq
c                tang .dot. { F, G }^T gets put into teq
                                    do m3=-hwidth3,hwidth3
                                    do m2=-hwidth, hwidth
                                    do m1=-hwidth, hwidth
                                       do c=0,ncmp-1
                                          coeff_u = coeff(icf(m1,m2,m3,
     & neq,c),i1,i2,i3)
                                          coeff_v = coeff(icf(m1,m2,m3,
     & teq,c),i1,i2,i3)
                                          coeff(icf(m1,m2,m3,neq,c),i1,
     & i2,i3) =   norm(1)*coeff_u + norm(2)*coeff_v
                                          coeff(icf(m1,m2,m3,teq,c),i1,
     & i2,i3) =   tang(1)*coeff_u + tang(2)*coeff_v
                                       end do
                                    end do
                                    end do
                                    end do

               endif ! which kind of wall

c           NORMAL AND TANGENTIAL GHOST EQS. SET BY FLUX BC AND SYMMETRY RESPECTIVELY
               if ( (.not.
     &              ( bc(side,axis).eq.subsonicoutflow .and.
     &                port.gt.1e-10 )) ) then
c              we have a mass flux type bc

                  if ( useneumannvel ) then
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l
                  off(axis+1) = -iss(axis) ! the code was originally written for the ghost points, shift the equation to the boundary
                  else
                         i1 = i1g
                         i2 = i2g
                         i3 = i3g
                  endif         ! useneumannvel

c                  if ( axis.eq.1 .and. side.eq.0 ) then
c                     print *,i1,i2,off(axis+1)
c                  endif

                  if (  .not.sliponwall .or..not.useneumannvel) then
!bc(side,axis).ne.slipWall) then

c              SET UP THE NORMAL FLUX EQUATION 
                  eqn = neq
                  now = .true.
c     ghost point
                  rr = consfac  !1d0 !2d0*side-1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0) * u(rc)* u(uc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,0))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),uc)
                        coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,0))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1) * u(rc)* u(vc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)
                        coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)
c     boundary point 
                  rr = 2d0      !2d0*side-1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0) * u(rc)* u(uc) at the grid point offset by is1, is2, is3
                        coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) 
     & = coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) + (rr*aj(i1l,
     & i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0))*u(i1+(is1)+off(1),i2+(is2)+
     & off(2),i3+(is3)+off(3),uc)
                        coeff(icf((is1),(is2),(is3),eqn,uc),i1,i2,i3) 
     & = coeff(icf((is1),(is2),(is3),eqn,uc),i1,i2,i3) + (rr*aj(i1l,
     & i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0))*u(i1+(is1)+off(1),i2+(is2)+
     & off(2),i3+(is3)+off(3),rc)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1) * u(rc)* u(vc) at the grid point offset by is1, is2, is3
                        coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) 
     & = coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) + (rr*aj(i1l,
     & i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1))*u(i1+(is1)+off(1),i2+(is2)+
     & off(2),i3+(is3)+off(3),vc)
                        coeff(icf((is1),(is2),(is3),eqn,vc),i1,i2,i3) 
     & = coeff(icf((is1),(is2),(is3),eqn,vc),i1,i2,i3) + (rr*aj(i1l,
     & i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1))*u(i1+(is1)+off(1),i2+(is2)+
     & off(2),i3+(is3)+off(3),rc)

c     interior point
                  rr = consfac  !1d0 !2d0*side-1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0) * u(rc)* u(uc) at the grid point offset by 2*is1, 2*is2, 2*is3
                        coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,
     & i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,i3) + 
     & (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),uc)
                        coeff(icf((2*is1),(2*is2),(2*is3),eqn,uc),i1,
     & i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,uc),i1,i2,i3) + 
     & (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1) * u(rc)* u(vc) at the grid point offset by 2*is1, 2*is2, 2*is3
                        coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,
     & i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,i3) + 
     & (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),vc)
                        coeff(icf((2*is1),(2*is2),(2*is3),eqn,vc),i1,
     & i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,vc),i1,i2,i3) + 
     & (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)
                  now = .false.
c     strikwerda term additions
                  if ( .false. .and. side.eq.0 ) then
c     ghost ghost point
                     rr = 2d0*alpha*(1d0-2d0*side)
                     if ( isaxi.gt.0 ) then
                        rr = rr*x(i1g-is1,i2g-is2,i3-is3,radaxis)
                     endif
c      linearize terms like rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,0) * u(rc)* u(uc) at the grid point offset by -is1, -is2, -is3
                           coeff(icf((-is1),(-is2),(-is3),eqn,rc),i1,
     & i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,rc),i1,i2,i3) + (
     & rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,
     & 0))*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),uc)
                           coeff(icf((-is1),(-is2),(-is3),eqn,uc),i1,
     & i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,uc),i1,i2,i3) + (
     & rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,
     & 0))*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)
c      linearize terms like rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,1) * u(rc)* u(vc) at the grid point offset by -is1, -is2, -is3
                           coeff(icf((-is1),(-is2),(-is3),eqn,rc),i1,
     & i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,rc),i1,i2,i3) + (
     & rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,
     & 1))*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),vc)
                           coeff(icf((-is1),(-is2),(-is3),eqn,vc),i1,
     & i2,i3) = coeff(icf((-is1),(-is2),(-is3),eqn,vc),i1,i2,i3) + (
     & rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,
     & 1))*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)
c     ghost point
                     rr = -2d0*alpha*(1d0-2d0*side) !(2d0*side-1d0)*alpha
                     if ( isaxi.gt.0 ) then
                        rr = rr*x(i1g,i2g,i3g,radaxis)
                     endif
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0) * u(rc)* u(uc) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,0))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),uc)
                           coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,0))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1) * u(rc)* u(vc) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)
                           coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,axis,1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)
c     boundary point 

                     rr = -2d0*alpha*(1d0-2d0*side) !2d0*side-1d0
                     if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0) * u(rc)* u(uc) at the grid point offset by is1, is2, is3
                           coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,
     & i3) = coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) + (rr*aj(
     & i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0))*u(i1+(is1)+off(1),i2+(
     & is2)+off(2),i3+(is3)+off(3),uc)
                           coeff(icf((is1),(is2),(is3),eqn,uc),i1,i2,
     & i3) = coeff(icf((is1),(is2),(is3),eqn,uc),i1,i2,i3) + (rr*aj(
     & i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0))*u(i1+(is1)+off(1),i2+(
     & is2)+off(2),i3+(is3)+off(3),rc)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1) * u(rc)* u(vc) at the grid point offset by is1, is2, is3
                           coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,
     & i3) = coeff(icf((is1),(is2),(is3),eqn,rc),i1,i2,i3) + (rr*aj(
     & i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1))*u(i1+(is1)+off(1),i2+(
     & is2)+off(2),i3+(is3)+off(3),vc)
                           coeff(icf((is1),(is2),(is3),eqn,vc),i1,i2,
     & i3) = coeff(icf((is1),(is2),(is3),eqn,vc),i1,i2,i3) + (rr*aj(
     & i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1))*u(i1+(is1)+off(1),i2+(
     & is2)+off(2),i3+(is3)+off(3),rc)

c     interior point
                     rr = 2d0*alpha*(1d0-2d0*side) !(2d0*side-1d0)*alpha
                     if ( isaxi.gt.0 ) then
                        rr = rr*x(i1i,i2i,i3i,radaxis)
                     endif
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0) * u(rc)* u(uc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0))*u(i1+(2*is1)+
     & off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),uc)
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,uc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,uc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0))*u(i1+(2*is1)+
     & off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1) * u(rc)* u(vc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1))*u(i1+(2*is1)+
     & off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),vc)
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,vc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,vc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1))*u(i1+(2*is1)+
     & off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)

                  endif ! side==0

                  endif ! not a slip wall, set boundary point equation

                  if ( useneumannvel ) then
                            i1 = i1g
                            i2 = i2g
                            i3 = i3g
                     off(axis+1) = 0
                  else
                            i1 = i1g
                            i2 = i2g
                            i3 = i3g
                  endif         ! useneumannvel

c     SET UP THE TANGENTIAL FLUX EQUATION 
                  eqn = teq
                  a = mod(axis+1,ndim)
                  rr = 1d0      !2d0*side-1d0
c     ghost point
c               if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
c               UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0),rc,uc,COEFF)
c               UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1),rc,vc,COEFF)
c               rr = 1d0 !2d0*side-1d0
c                if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)               
c               UVCF( 2*is1, 2*is2, 2*is3,-rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0),rc,uc,COEFF)
c               UVCF( 2*is1, 2*is2, 2*is3,-rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1),rc,vc,COEFF)
                  coeff(icf(0,0,0,teq,uc),i1,i2,i3) = tang(1)
                  coeff(icf(0,0,0,teq,vc),i1,i2,i3) = tang(2)
                  coeff(icf(2*is1,2*is2,2*is3,teq,uc),i1,i2,i3)=-tang(
     & 1)
                  coeff(icf(2*is1,2*is2,2*is3,teq,vc),i1,i2,i3)=-tang(
     & 2)

c     NEUMANN CONDITION ON THE NORMAL COMPONENT IF ASKED FOR
                  if ( useneumannvel ) then
                     eqn = neq
                     a = axis
                     rr = 1d0   !2d0*side-1d0
c     ghost point
c                     coeff(icf(0,0,0,neq,uc),i1,i2,i3) = norm(1)
c                     coeff(icf(0,0,0,neq,vc),i1,i2,i3) = norm(2)

c                  coeff(icf(2*is1,2*is2,2*is3,neq,uc),i1,i2,i3)=-norm(1)
c                  coeff(icf(2*is1,2*is2,2*is3,neq,vc),i1,i2,i3)=-norm(2)
c     !!! 080125 try a vector symmetry!
c                   coeff(icf(2*is1,2*is2,2*is3,neq,uc),i1,i2,i3)=norm(1)
c                   coeff(icf(2*is1,2*is2,2*is3,neq,vc),i1,i2,i3)=norm(2)


                     if ( isaxi.gt.0 .and. ghostrpos )
     &                    rr = rr*x(i1g,i2g,i3g,radaxis)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0) * u(rc)* u(uc) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,a,0))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),uc)
                           coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,uc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,a,0))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1) * u(rc)* u(vc) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,a,1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)
                           coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) + (rr*aj(i1g,i2g,i3g)*
     & rsxy(i1g,i2g,i3g,a,1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)
                     if ( bc(side,axis).eq.slipWall
     &                    .or. bc(side,axis).eq.noSlipWall ) then
c                      conservative wall bc
                        rr = 1d0
                     else
c                      neumann bc                        
                        rr = -1d0
                     endif
                     if ( isaxi.gt.0 .and. ghostrpos )
     &                    rr = rr*x(i1i,i2i,i3i,radaxis)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0) * u(rc)* u(uc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),uc)
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,uc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,uc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1) * u(rc)* u(vc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,rc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),vc)
                           coeff(icf((2*is1),(2*is2),(2*is3),eqn,vc),
     & i1,i2,i3) = coeff(icf((2*is1),(2*is2),(2*is3),eqn,vc),i1,i2,i3)
     &  + (rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1))*u(i1+(2*is1)+off(
     & 1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)
                  endif

c              apply the trick that reduces the chance of a zero pivot:
c                let F be the normal flux equation and G the tangential flux equation
c                norm .dot. { F, G }^T gets put into neq
c                tang .dot. { F, G }^T gets put into teq
                                    do m3=-hwidth3,hwidth3
                                    do m2=-hwidth, hwidth
                                    do m1=-hwidth, hwidth
                                       do c=0,ncmp-1
                                          coeff_u = coeff(icf(m1,m2,m3,
     & neq,c),i1,i2,i3)
                                          coeff_v = coeff(icf(m1,m2,m3,
     & teq,c),i1,i2,i3)
                                          coeff(icf(m1,m2,m3,neq,c),i1,
     & i2,i3) =   norm(1)*coeff_u + norm(2)*coeff_v
                                          coeff(icf(m1,m2,m3,teq,c),i1,
     & i2,i3) =   tang(1)*coeff_u + tang(2)*coeff_v
                                       end do
                                    end do
                                    end do
                                    end do

c              reset the indices for future macros and assignments
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l
                  if ( useneumannvel .and. .not.sliponwall) then
!bc(side,axis).ne.slipwall 
!     &                 .and.
!     &                 bc(side,axis).ne.subsonicinflow) then
c     &                 ) then
                     do c=uc,uc+nd-1
                        coeff(icf(0,0,0,teq,c),i1,i2,i3) = tang(c-uc+1)
                     enddo      ! each vel component

c              apply the trick that reduces the chance of a zero pivot:
c                let F be the normal flux equation and G the tangential flux equation
c                norm .dot. { F, G }^T gets put into neq
c                tang .dot. { F, G }^T gets put into teq
                                       do m3=-hwidth3,hwidth3
                                       do m2=-hwidth, hwidth
                                       do m1=-hwidth, hwidth
                                          do c=0,ncmp-1
                                             coeff_u = coeff(icf(m1,m2,
     & m3,neq,c),i1,i2,i3)
                                             coeff_v = coeff(icf(m1,m2,
     & m3,teq,c),i1,i2,i3)
                                             coeff(icf(m1,m2,m3,neq,c),
     & i1,i2,i3) =   norm(1)*coeff_u + norm(2)*coeff_v
                                             coeff(icf(m1,m2,m3,teq,c),
     & i1,i2,i3) =   tang(1)*coeff_u + tang(2)*coeff_v
                                          end do
                                       end do
                                       end do
                                       end do
                  endif

               else
c     we have a non-mass flux (ie pressure) boundary condition, extrapolate the velocity
                  do e=uc,max(vc,wc)
                     coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
                     coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
                     coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=1
c     neumann     coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=-1
                  end do
               endif            ! is mass-flux type boundary

c     SWIRL IS EXTRAPOLATED OR SPECIFIED
               if ( withswirl.gt.0 ) then
c     always extrapolate the ghost point
                  coeff(icf(0,0,0,wc,wc),i1g,i2g,i3g) = 1d0
                  coeff(icf(is1,is2,is3,wc,wc),i1g,i2g,i3g) = -2d0
                  coeff(icf(2*is1,2*is2,2*is3,wc,wc),i1g,i2g,i3g) =+1d0

                  if ( !bc(side,axis).ne.slipwall .and.
     &                 bc(side,axis).ne.subsonicoutflow ) then
                     coeff(icf(0,0,0,wc,wc),i1,i2,i3) = 1d0
                  endif ! specify the swirl velocity
               endif ! withswirl

c           TEMPERATURE IS SPECIFIED, NEUMANN, OR EXTRAPOLATED
               if ( useneumanntemp ) then

                  if ( .false. ) then
                     coeff(icf(0,0,0,tc,tc),i1g,i2g,i3g) = 1d0
                    coeff(icf(2*is1,2*is2,2*is3,tc,tc),i1g,i2g,i3g)=-
     & 1d0
                  else
                     off(axis+1) = iss(axis)
                            i1 = i1g
                            i2 = i2g
                            i3 = i3g
                     eqn = tc
                     do d=1,ndim
c     coefficients for a  difference approximation to norm(d) * u[tc]_X
c     r-part
                              coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) + (norm(d))*rx(i1+off(1),i2+
     & off(2),i3+off(3),1,d)*dri2(1)
                              coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) - (norm(d))*rx(i1+off(1),i2+
     & off(2),i3+off(3),1,d)*dri2(1)
c     s-part
                              coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) + (norm(d))*rx(i1+off(1),i2+
     & off(2),i3+off(3),2,d)*dri2(2)
                              coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) - (norm(d))*rx(i1+off(1),i2+
     & off(2),i3+off(3),2,d)*dri2(2)
                     enddo
                     off(axis+1) = 0d0
                            i1 = i1l
                            i2 = i2l
                            i3 = i3l
                  endif
               else
                  coeff(icf(0,0,0,tc,tc),i1g,i2g,i3g) = 1d0
                  coeff(icf(is1,is2,is3,tc,tc),i1g,i2g,i3g) = -2d0
                  coeff(icf(2*is1,2*is2,2*is3,tc,tc),i1g,i2g,i3g)=1
                  if ( ( bc(side,axis).eq.noslipwall ).or.
     &                 (bc(side,axis).eq.subsonicinflow .and.
     &                 port.gt.1e-10) ) then
                     coeff(icf(0,0,0,tc,tc),i1,i2,i3) = 1d0
                  endif         ! dirichlet bc on temp?
               endif            ! use neumann
CCCCCCCC END OF COEFFICIENT MATRIX PART

            else
c           SET THE RHS

c           DENSITY IS EXTRAPOLATED AT THE GHOST, INFLOW SPECIFIED OR MIXED ON P

c               rhs(i1g,i2g,i3g,rc) = 0d0 !ghost
c                 extrapolate the pressure
               if ( .true. ) then
                  eqn = rc

                  if ( .not.extrapponbdy .or.
     &                 ( bc(side,axis).eq.subsonicoutflow .and.
     &                 port.gt.1e-10 ).or.
     &                 (bc(side,axis).eq.subsonicinflow
     &                 .and.usedirichletrhoinflow) )then
                            i1 = i1g
                            i2 = i2g
                            i3 = i3g
c                     if ( bc(side,axis).ne.subsonicinflow ) then

c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & 1d0)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),tc)
c      linearize terms like -2d0 * u(rc)* u(tc) at the grid point offset by is1, is2, is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-
     & 2d0)*u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),rc)*u(
     & i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),tc)
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & 1d0)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),
     & rc)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),tc)
c                     endif
                     if ( bc(side,axis).eq.subsonicinflow .and.
     &                    usedirichletrhoinflow ) then
                               i1 = i1l
                               i2 = i2l
                               i3 = i3l
                        rhs(i1,i2,i3,rc) = rho
c                        print *,i1,i2,i3,rho
                     endif
                  else
                            i1 = i1l
                            i2 = i2l
                            i3 = i3l
                     rtmp = exo2(i1g,i2g,i3g,is1,is2,is3,rc)
                     if ( .false. ) then
                        if ( rtmp.gt.0d0 .or. .true.) then
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by -is1, -is2, -is3
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),
     & rc)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),tc)
c      linearize terms like -2d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (-2d0)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(
     & 0)+off(1),i2+(0)+off(2),i3+(0)+off(3),tc)
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by is1, is2, is3
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (1d0)*u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),rc)*
     & u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),tc)
                        else
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by -is1, -is2, -is3
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),
     & rc)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),tc)
c      linearize terms like -1d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (-1d0)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(
     & 0)+off(1),i2+(0)+off(2),i3+(0)+off(3),tc)
                        endif
                     else if ( .not. sliponwall ) then
c      linearize terms like 1d0 * u(rc)* u(tc) at the grid point offset by -is1, -is2, -is3
                              rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & 1d0)*u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)*
     & u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),tc)
c      linearize terms like -3d0 * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                              rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & -3d0)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),tc)
c      linearize terms like 3d0 * u(rc)* u(tc) at the grid point offset by is1, is2, is3
                              rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & 3d0)*u(i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),rc)*u(
     & i1+(is1)+off(1),i2+(is2)+off(2),i3+(is3)+off(3),tc)
c      linearize terms like -1d0 * u(rc)* u(tc) at the grid point offset by 2*is1, 2*is2, 2*is3
                              rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & -1d0)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),
     & rc)*u(i1+(2*is1)+off(1),i2+(2*is2)+off(2),i3+(2*is3)+off(3),tc)
                     else
                        rhs(i1g,i2g,i3g,rc) = 0
                     endif
                  endif
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l

               endif

c               if ( iscorner ) rhs(i1,i2,i3,rc) = 0d0 !.and. .false. ) rhs(i1,i2,i3,rc) = 0d0 

               if ( .not.useneumannvel .and.
     &              bc(side,axis).eq.subsonicinflow ) then
c                 dirichlet on rho
                   rhs(i1,i2,i3,rc) = rho
               else if ( bc(side,axis).eq.subsonicoutflow
     &                 .and. port.gt.1e-10 ) then
c                 mixed condition on p
                  eqn = rc
                  pfac = bd(tc+ncmp,side,axis,grid)
                  pdotn= bd(tc+ncmp*2,side,axis,grid)
                  prhs= bd(tc,side,axis,grid)

c      linearize terms like pfac * u(rc)* u(tc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (pfac)*
     & u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1)
     & ,i2+(0)+off(2),i3+(0)+off(3),tc)
                  rhs(i1,i2,i3,rc) =
     &                 pfac*u(i1,i2,i3,rc)*u(i1,i2,i3,tc)+prhs+
     & u(i1l,i2l,i3l,tc)*(pdotn*norm(1)*ux(i1l,i2l,i3l,rc,1)+
     &                 pdotn*norm(2)*ux(i1l,i2l,i3l,rc,2))+
     & u(i1l,i2l,i3l,rc)*(pdotn*norm(1)*ux(i1l,i2l,i3l,tc,1)+
     &                 pdotn*norm(2)*ux(i1l,i2l,i3l,tc,2))
               else if ( (.not. sliponwall) .and.
     &.not. (bc(side,axis).eq.subsonicinflow.and.
     &                 usedirichletrhoinflow)) then
c     normal momentum equation at other bcs
                  eqn = rc

                  if ( extrapponbdy .or.
     &                 (bc(side,axis).eq.subsonicinflow.and.
     &                 usedirichletrhoinflow)) then
                         i1 = i1g
                         i2 = i2g
                         i3 = i3g
                  off(axis+1)=iss(axis)
                  else
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l
                  endif

                  if ( .true. ) then
                  do d=uc,uc+ndim-1
                     do cc=uc, uc+ndim-1
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
     & vect(d)*u(i1l,i2l,i3l,cc)*ux(i1l,i2l,i3l,d,cc)
                     end do     ! cc (each vel. component)
                  end do
                  endif

                  if ( withswirl.gt.0 ) then

                     if ( .true. ) then
                     rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) -
     & vect(2)*u(i1l,i2l,i3l,wc)**2/xyz(i1l,i2l,i3l,2)
                     else
                        do d=1,nd
                              if ( .true. ) then
                              drar = (-vect(2))*dri2(1)*jaci*jac(i1+
     & off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(
     & 3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                    rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)*
     & u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                              drar = (-vect(2))*dri2(1)*jaci*jac(i1+
     & off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                    rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)*
     & u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                              drar = (-vect(2))*dri2(2)*jaci*jac(i1+
     & off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(
     & 3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+
     & off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                    rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)*
     & u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                              drar = (-vect(2))*dri2(2)*jaci*jac(i1+
     & off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                    rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)*
     & u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                              else if ( .false. ) then
                                 drar = (-vect(2))*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                      rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),
     & i3+(0)+off(3),wc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),
     & wc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                      rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),
     & i3+(0)+off(3),wc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),
     & wc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                      rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),
     & i3+(0)+off(3),wc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),
     & wc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                      rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),
     & i3+(0)+off(3),wc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),
     & wc)
                              else
c      linearize terms like ((-vect(2))/(xyz(i1,i2,i3,2))) * u(wc)* u(wc) at the grid point offset by 0, 0, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (((-vect(2))/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),wc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)
                              endif
                        enddo
                     endif
                  endif

                  rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
     &                 vect(1)*grav(1)+vect(2)*grav(2)
c     
c     MOMENTUM EQUATION VISCOUS TERMS
c     
                  if ( .true.) then
c     u-momentum equation, the terms are
c     (4 u[uc]_xx/3 + u[uc]_yy + u[vc]_xy/3 + axifac*(u[uc]_y/y + u[vc]_x/y/3))/u[rc]/ren
                     rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
     & vect(1)*oren*( f43*uxx(i1l,i2l,i3l,uc,1,1) + uxx(i1l,i2l,i3l,
     & uc,2,2) +
     & f13*uxx(i1l,i2l,i3l,vc,1,2) )/u(i1l,i2l,i3l,rc)

                     if ( isaxi.gt.0 .and. xyz(i1l,i2l,i3l,2).gt.0d0 ) 
     & then
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
     & vect(1)*oren*(ux(i1l,i2l,i3l,uc,2) + f13*ux(i1l,i2l,i3l,vc,1))/
     &                       (u(i1l,i2l,i3l,rc)*xyz(i1l,i2l,i3l,2))
                     end if

c     v-momentum equation, the terms are
c     (4 u[vc]_yy/3 + u[vc]_xx + u[uc]_xy/3 + axifac*(4/3)*(u[vc]_y/y - u[vc]/y^2))/u[rc]/ren
                     rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
     & vect(2)*oren*( f43*uxx(i1l,i2l,i3l,vc,2,2) + uxx(i1l,i2l,i3l,
     & vc,1,1) +
     & f13*uxx(i1l,i2l,i3l,uc,2,1) )/u(i1l,i2l,i3l,rc)

                     if ( isaxi.gt.0 .and. xyz(i1l,i2l,i3l,2).gt.0d0)
     & then
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
     & vect(2)*oren*(f43*(ux(i1l,i2l,i3l,vc,2)-u(i1l,i2l,i3l,vc)/xyz(
     & i1l,i2l,i3l,2))/
     &                       (u(i1l,i2l,i3l,rc)*xyz(i1l,i2l,i3l,2)))

                     endif
                  endif         !  use viscous terms
                  off(axis+1) = 0
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l
               endif            ! what to do on bdy for rho

c           VELOCITY COMPONENTS ON ALL BUT slip and outlflows ARE DIRICHLET
               if ( .not.useneumannvel .and. .not.sliponwall
!bc(side,axis).ne.slipwall 
!     &              .and. bc(side,axis).ne.subsonicinflow ) then
     &              ) then
c              no-slip and inflow get dirichlet values on the normal and tangential components
c                  do c=uc,max(vc,wc)
                   rhs(i1,i2,i3,neq) = scale*uvel
                   rhs(i1,i2,i3,teq) = vvel
c                  enddo ! each vel component
c              apply the trick that reduces the chance of a zero pivot (to match coeff array)
                                     coeff_u = rhs(i1,i2,i3,neq)
                                     coeff_v = rhs(i1,i2,i3,teq)
                                     rhs(i1,i2,i3,neq) = norm(1)*
     & coeff_u + norm(2)*coeff_v
                                     rhs(i1,i2,i3,teq) = tang(1)*
     & coeff_u + tang(2)*coeff_v
               else if ( sliponwall ) then
!bc(side,axis).eq.slipwall .and. .not.skipslip ) then !.or.
!     &                 bc(side,axis).eq.subsonicinflow ) then
c                 project out the normal component from the slipwall bdy equation
                  if ( .true. ) then ! 080114
c                     print *,i1,i2,rhs(i1,i2,i3,uc),rhs(i1,i2,i3,vc)
                     rhs(i1,i2,i3,teq) =
     & tang(1)*rhs(i1,i2,i3,uc)+tang(2)*rhs(i1,i2,i3,vc)
                     rhs(i1,i2,i3,neq) = scale*uvel
                                       coeff_u = rhs(i1,i2,i3,neq)
                                       coeff_v = rhs(i1,i2,i3,teq)
                                       rhs(i1,i2,i3,neq) = norm(1)*
     & coeff_u + norm(2)*coeff_v
                                       rhs(i1,i2,i3,teq) = tang(1)*
     & coeff_u + tang(2)*coeff_v
                  endif
               else if ( port.lt.1e-10 .and. bc(side,axis)
     & .eq.subsonicoutflow) then
c                 outflow condition with specified mass flux
c                  print *,i1,i2,norm(1),norm(2)
                  eqn = neq
c      linearize terms like norm(1) * u(uc)* u(rc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (norm(
     & 1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),uc)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
c      linearize terms like norm(2) * u(vc)* u(rc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (norm(
     & 2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)

                  eqn = teq
c      linearize terms like tang(1) * u(uc)* u(rc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (tang(
     & 1))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),uc)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
c      linearize terms like tang(2) * u(vc)* u(rc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (tang(
     & 2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)

                  rhs(i1,i2,i3,neq) = rhs(i1,i2,i3,neq) + scale*uvel*
     & rho
                  rhs(i1,i2,i3,teq) = rhs(i1,i2,i3,teq) + vvel*rho
                                    coeff_u = rhs(i1,i2,i3,neq)
                                    coeff_v = rhs(i1,i2,i3,teq)
                                    rhs(i1,i2,i3,neq) = norm(1)*
     & coeff_u + norm(2)*coeff_v
                                    rhs(i1,i2,i3,teq) = tang(1)*
     & coeff_u + tang(2)*coeff_v
               endif ! which kind of wall

c           NORMAL AND TANGENTIAL EQS. SET BY FLUX BC AND SYMMETRY RESPECTIVELY

               if ( ( .not.
     &              ( bc(side,axis).eq.subsonicoutflow .and.
     &                port.gt.1e-10 )) ) then

c              SET UP THE NORMAL FLUX EQUATION 
                  if ( useneumannvel ) then
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l
                  off(axis+1) = -iss(axis)
                  else
                         i1 = i1g
                         i2 = i2g
                         i3 = i3g
                  endif         ! useneumannvel

                  if (  .not.sliponwall .or..not.useneumannvel) then
!bc(side,axis).ne.slipWall) then
                  eqn = neq
                  rr = consfac !1d0!2d0*side-1d0
c     ghost point
                  if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0) * u(rc)* u(uc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*aj(
     & i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0))*u(i1+(0)+off(1),i2+(0)+
     & off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),uc)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1) * u(rc)* u(vc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*aj(
     & i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1))*u(i1+(0)+off(1),i2+(0)+
     & off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)
c     boundary point 
                  rr = 2d0      !2d0*side-1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0) * u(rc)* u(uc) at the grid point offset by is1, is2, is3
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*aj(
     & i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0))*u(i1+(is1)+off(1),i2+(
     & is2)+off(2),i3+(is3)+off(3),rc)*u(i1+(is1)+off(1),i2+(is2)+off(
     & 2),i3+(is3)+off(3),uc)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1) * u(rc)* u(vc) at the grid point offset by is1, is2, is3
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*aj(
     & i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1))*u(i1+(is1)+off(1),i2+(
     & is2)+off(2),i3+(is3)+off(3),rc)*u(i1+(is1)+off(1),i2+(is2)+off(
     & 2),i3+(is3)+off(3),vc)
                  fac = 1d0
                  if ( bc(side,axis).eq.subsonicoutflow )!.and. side.eq.1)
     &                 fac = -1d0

c                  if ((bc(side,axis).eq.subsonicinflow .and. side.eq.1)
c     &                 .or. 
c     &                 (bc(side,axis).eq.subsonicoutflow.and.side.eq.0)) 
c     &                 fac = -1d0

                  rr = fac*(2d0 + 2d0*consfac)*(1d0-2d0*side)
c                  if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
                  rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn)+
     &              rr*scale*rho*uvel*len*dri(mod(axis+1,ndim)+1)/
     &              (irb(1,mod(axis+1,ndim))-irb(0,mod(axis+1,ndim)))
c     interior point
                  rr = consfac !1d0!2d0*side-1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0) * u(rc)* u(uc) at the grid point offset by 2*is1, 2*is2, 2*is3
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*aj(
     & i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0))*u(i1+(2*is1)+off(1),i2+(
     & 2*is2)+off(2),i3+(2*is3)+off(3),rc)*u(i1+(2*is1)+off(1),i2+(2*
     & is2)+off(2),i3+(2*is3)+off(3),uc)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1) * u(rc)* u(vc) at the grid point offset by 2*is1, 2*is2, 2*is3
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*aj(
     & i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1))*u(i1+(2*is1)+off(1),i2+(
     & 2*is2)+off(2),i3+(2*is3)+off(3),rc)*u(i1+(2*is1)+off(1),i2+(2*
     & is2)+off(2),i3+(2*is3)+off(3),vc)

c                 strikwerda fix
                  if (.false. .and. side.eq.0) then
c     ghost ghost point
                     rr = 2d0*alpha*(1d0-2d0*side)!(2d0*side-1d0)*alpha
                     if ( isaxi.gt.0 ) then
                        rr = rr*x(i1g-is1,i2g-is2,i3-is3,radaxis)
                     endif
c      linearize terms like rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,0) * u(rc)* u(uc) at the grid point offset by -is1, -is2, -is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,0))
     & *u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)*u(i1+
     & (-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),uc)
c      linearize terms like rr*aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,1) * u(rc)* u(vc) at the grid point offset by -is1, -is2, -is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1g-is1,i2g-is2,i3-is3)*rsxy(i1g-is1,i2g-is2,i3-is3,axis,1))
     & *u(i1+(-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),rc)*u(i1+
     & (-is1)+off(1),i2+(-is2)+off(2),i3+(-is3)+off(3),vc)
c     ghost point
                     rr = -2d0*alpha*(1d0-2d0*side) !(2d0*side-1d0)*alpha
                     if ( isaxi.gt.0 ) then
                        rr = rr*x(i1g,i2g,i3g,radaxis)
                     endif
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0) * u(rc)* u(uc) at the grid point offset by 0, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0))*u(i1+(0)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),uc)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1) * u(rc)* u(vc) at the grid point offset by 0, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1))*u(i1+(0)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),vc)
c     boundary point 
                     rr = -2d0*alpha*(1d0-2d0*side) !2d0*side-1d0
                     if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0) * u(rc)* u(uc) at the grid point offset by is1, is2, is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,0))*u(i1+(is1)+off(1),i2+
     & (is2)+off(2),i3+(is3)+off(3),rc)*u(i1+(is1)+off(1),i2+(is2)+
     & off(2),i3+(is3)+off(3),uc)
c      linearize terms like rr*aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1) * u(rc)* u(vc) at the grid point offset by is1, is2, is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1l,i2l,i3i)*rsxy(i1l,i2l,i3l,axis,1))*u(i1+(is1)+off(1),i2+
     & (is2)+off(2),i3+(is3)+off(3),rc)*u(i1+(is1)+off(1),i2+(is2)+
     & off(2),i3+(is3)+off(3),vc)
c     interior point
                     rr = 2d0*alpha*(1d0-2d0*side)!(2d0*side-1d0)*alpha
                     if ( isaxi.gt.0 ) then
                        rr = rr*x(i1i,i2i,i3i,radaxis)
                     endif
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0) * u(rc)* u(uc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0))*u(i1+(2*is1)+off(1),
     & i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)*u(i1+(2*is1)+off(1),i2+
     & (2*is2)+off(2),i3+(2*is3)+off(3),uc)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1) * u(rc)* u(vc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1))*u(i1+(2*is1)+off(1),
     & i2+(2*is2)+off(2),i3+(2*is3)+off(3),rc)*u(i1+(2*is1)+off(1),i2+
     & (2*is2)+off(2),i3+(2*is3)+off(3),vc)
                  endif
                  endif !not a slipwall

                  if ( useneumannvel ) then
                            i1 = i1g
                            i2 = i2g
                            i3 = i3g
                     off(axis+1) = 0
                  else
                            i1 = i1g
                            i2 = i2g
                            i3 = i3g
                  endif         ! useneumannvel

c     SET UP THE TANGENTIAL FLUX EQUATION 
                  eqn = teq
                  a = mod(axis+1,ndim)
                  rr = 1d0!2d0*side-1d0
c     ghost point
c                  if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
c                  UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0),rc,uc,RHS)
c                  UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1),rc,vc,RHS)
c                  rr = 1d0!2d0*side-1d0
c                  if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)               
c                  UVCF( 2*is1, 2*is2, 2*is3,-rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0),rc,uc,RHS)
c                  UVCF( 2*is1, 2*is2, 2*is3,-rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1),rc,vc,RHS)

c     NEUMANN CONDITION ON THE NORMAL COMPONENT IF ASKED FOR
                  if ( useneumannvel  ) then
!.and.bc(side,axis).ne.slipwall) then
                     eqn = neq
                     a = axis
                     rr = 1d0   !2d0*side-1d0
c     ghost point
c                     rhs(i1,i2,i3,neq) = 0
                     if ( isaxi.gt.0 .and.ghostrpos )
     &                    rr = rr*x(i1g,i2g,i3g,radaxis)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0) * u(rc)* u(uc) at the grid point offset by 0, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0))*u(i1+(0)+off(1),i2+(0)+
     & off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),uc)
c      linearize terms like rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1) * u(rc)* u(vc) at the grid point offset by 0, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1))*u(i1+(0)+off(1),i2+(0)+
     & off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)
                     if ( bc(side,axis).eq.slipWall
     &                    .or. bc(side,axis).eq.noSlipWall ) then
                        rr = 1d0
                     else
                        rr = -1d0
                     endif
                     if ( isaxi.gt.0 .and. ghostrpos )
     &                    rr = rr*x(i1i,i2i,i3i,radaxis)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0) * u(rc)* u(uc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0))*u(i1+(2*is1)+off(1),i2+(
     & 2*is2)+off(2),i3+(2*is3)+off(3),rc)*u(i1+(2*is1)+off(1),i2+(2*
     & is2)+off(2),i3+(2*is3)+off(3),uc)
c      linearize terms like rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1) * u(rc)* u(vc) at the grid point offset by 2*is1, 2*is2, 2*is3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (rr*
     & aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1))*u(i1+(2*is1)+off(1),i2+(
     & 2*is2)+off(2),i3+(2*is3)+off(3),rc)*u(i1+(2*is1)+off(1),i2+(2*
     & is2)+off(2),i3+(2*is3)+off(3),vc)
                  endif

                                    coeff_u = rhs(i1,i2,i3,neq)
                                    coeff_v = rhs(i1,i2,i3,teq)
                                    rhs(i1,i2,i3,neq) = norm(1)*
     & coeff_u + norm(2)*coeff_v
                                    rhs(i1,i2,i3,teq) = tang(1)*
     & coeff_u + tang(2)*coeff_v

c              reset the indices for future macros and assignments
                         i1 = i1l
                         i2 = i2l
                         i3 = i3l
                  if ( useneumannvel .and. .not.sliponwall )then
!     &                 bc(side,axis).ne.slipwall  ) then
c     &                 .and.
c    &                 bc(side,axis).ne.subsonicinflow ) then
                     rhs(i1,i2,i3,teq) = vvel
c     enddo ! each vel component
c     apply the trick that reduces the chance of a zero pivot (to match coeff array)
                                       coeff_u = rhs(i1,i2,i3,neq)
                                       coeff_v = rhs(i1,i2,i3,teq)
                                       rhs(i1,i2,i3,neq) = norm(1)*
     & coeff_u + norm(2)*coeff_v
                                       rhs(i1,i2,i3,teq) = tang(1)*
     & coeff_u + tang(2)*coeff_v
                  endif

               endif !  mass flux type bc

               if ( withswirl.eq.1 ) then
c           SWIRL IS EXTRAPOLATED OR SET BY INFLOW/NOSLIP
                  if ( bc(side,axis).eq.noslipwall) then
                     if ( bt(0,side,axis,grid).eq. axisymmetricsbr) 
     & then
                        rhs(i1,i2,i3,wc) = ubv(0)*x(i1,i2,i3,1)
                     else if (bt(0,side,axis,grid).eq.linearrampinx)
     & then
                        rhs(i1,i2,i3,wc) = ubv( 3*wc ) +
     &                       ubv( 3*wc +1 )*x(i1,i2,i3,0)
c                        print *,i1,i2,rhs(i1,i2,i3,wc)
                     else
                        rhs(i1,i2,i3,wc) = 0d0
                     endif
                  else if ( bc(side,axis).eq.subsonicinflow ) then
                     rhs(i1,i2,i3,wc) = wvel
                  else if ( bc(side,axis).eq.slipwall ) then
                     rhs(i1,i2,i3,wc) = u(i1,i2,i3,wc)
                  endif
                  rhs(i1g,i2g,i3g,wc) = 0d0
               endif

c           TEMPERATURE IS SPECIFIED, NEUMANN, OR EXTRAPOLATED
               rhs(i1g,i2g,i3g,tc) = 0d0 !ghost
               if ( .not.useneumanntemp ) then
                  if ( bc(side,axis).eq.noslipwall ) then
                     if ( bt(0,side,axis,grid).eq.axisymmetricsbr) then
                        rhs(i1,i2,i3,tc) = ubv(3)
                     else if (bt(0,side,axis,grid).eq.linearrampinx)
     & then
                        rhs(i1,i2,i3,tc) = ubv( 3*(max(vc,wc)+1) ) +
     &                       ubv( 3*(max(vc,wc)+1) +1 )*x(i1,i2,i3,0)
                     else if ( port.gt.1e-10 ) then
                        rhs(i1,i2,i3,tc) = port
                     endif      !bdy type
                  else if ( port.gt.1e-10 ) then
                     rhs(i1,i2,i3,tc) = port
                  endif         !noslip wall
               endif            ! .not. useneumanntemp

c               if ( bc(side,axis).eq.subsonicinflow ) then
c               print *,"RHS FOR TC = ",rhs(i1g,i2g,i3g,tc)
c               print *,"RHS FOR VC = ",rhs(i1,i2,i3,vc)
c               endif
               if ( .false. .and. bc(side,axis).eq.subsonicoutflow) 
     & then
                  write(*,1000) grid,i1i,i2i,
     &                 rhs(i1i,i2i,i3,rc),rhs(i1i,i2i,i3,uc),
     &                 rhs(i1i,i2i,i3,vc),rhs(i1i,i2i,i3,tc)
                  write(*,1000) grid,i1,i2,
     &                 rhs(i1,i2,i3,rc),rhs(i1,i2,i3,uc),
     &                 rhs(i1,i2,i3,vc),rhs(i1,i2,i3,tc)
                  write(*,1000) grid,i1g,i2g,
     &                 rhs(i1g,i2g,i3,rc),rhs(i1g,i2g,i3,uc),
     &                 rhs(i1g,i2g,i3,vc),rhs(i1g,i2,i3,tc)
               endif

c     compute a boundary flux diagnostic
               if ( .false. ) then
               rr = 1d0
               xr = rr/dri(mod(axis+1,ndim)+1)/
     &              rx(i1,i2,i3,mod(axis+1,ndim)+1,mod(axis+1,ndim)+1)
               if ( isaxi.gt.0 ) rr = xr*x(i1,i2,i3,1)
               if (
     &              (i1l.eq.irb(0,0) .and. i2l.eq.irb(0,1)
     &              .and. i3l.eq.irb(0,2)) .or.
     &              (i1l.eq.irb(1,0) .and. i2l.eq.irb(1,1)
     &              .and. i3l.eq.irb(1,2))  ) then
                  rr = .5*rr
               endif
               flowsum =flowsum+
     &              (rr*u(i1,i2,i3,rc)*(norm(1)*u(i1,i2,i3,uc)+
     &              norm(2)*u(i1,i2,i3,vc))*2d0)

c               print *,i1,i2,u(i1,i2,i3,uc),u(i1,i2,i3,vc)
c               print *,norm(1),norm(2)
               flowsum2 =flowsum2+
     &              (rr*u(i1,i2,i3,rc)*(norm(1)*u(i1,i2,i3,uc)+
     &              norm(2)*u(i1,i2,i3,vc)))

               if ( isaxi .gt. 0) then
                  rr = xr*x(i1g,i2g,i3g,1)
               endif
               flowsum = flowsum +
     &              rr*u(i1g,i2g,i3g,rc)*(norm(1)*u(i1g,i2g,i3g,uc)+
     &              norm(2)*u(i1g,i2g,i3g,vc))
               if ( isaxi .gt. 0) then
                  rr = xr*x(i1i,i2i,i3i,1)
               endif
               flowsum = flowsum +
     &              rr*u(i1i,i2i,i3i,rc)*(norm(1)*u(i1i,i2i,i3i,uc)+
     &              norm(2)*u(i1i,i2i,i3i,vc))
            endif ! compute flux diagnostic

c            if ( grid.eq.1 ) then!bc(side,axis).eq.subsonicinflow) then
c               print *,i1,i2,rhs(i1,i2,i3,rc)
c            endif
            endif               ! set coefficients or rhs

c            print *,"t",i1,i2,rhs(i1,i2,i3,teq)
         enddo !i1l
         enddo !i2l
         enddo !i3l
         if ( .false. .and. cfrhs.eq.1 ) then
            write(*,2000) grid,side,axis,flowsum/4d0,flowsum2
         endif

      endif ! bc is some kind of wall
      enddo ! side
      enddo ! axis

 1000 format (i2,1x,i3,1x,i3,1x,4(e12.5,1x))
 2000 format ("bc flux : ",3(1x,i3),1x,2(e12.5,1x))
      return
      end
