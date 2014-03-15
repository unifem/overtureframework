! This file automatically generated from cnsNoSlipWallBC.bf with bpp.










c *wdh* 081214 -- xlf does not like +- --> change O1 to (O1) etc. below







      subroutine cnsNoSlipWallBC(nd,nd1a,nd1b,nd2a,nd2b,nd3a,
     &                           nd3b,nd4a,nd4b,u,x,aj,rsxy,ipar,rpar,
     &                           indexRange,bc)
c
c     no-slip wall, zero mass flux boundary conditions for cnsdu (Jameson)
c
c
c     051221 kkc : Initial Version
c
c     This subroutine applies the no-slip wall boundary condition to u where appropriate
c
c     Notes:
c          i) we set velocity = 0 on the boundary
c         ii) zero mass flux implies f_{i-1/2}+f_{i+1/2}=0  where i is on the boundary and
c             f is the total flux (convective and artificial viscosity) for the continuity
c             equation.
c         iv) There are several ways to impose the flux condition.  Right now we assume
c             that viscous fluxes, f^a, satisfy f^a_{i-1/2} = -f^a_{i+1/2} when the artificial
c             viscosity is computed in cnsdu. XXX NOTE THIS IS A CONDITION ON cnsdu22, cnsdu22a
c             THAT IS CURRENTLY ONLY SATISFIED IN cnsdu22a.
c          v) Given (iv) and the simple flux averagin in cnsdu,  we now have the condition
c             \rho_{i-1}( a_{11}u + a_{12}v )_{i-1} ) = -\rho_{i+1}( a_{11}u + a_{12}v )_{i+1} ) .
c             We can either extrapolate \rho and compute the velocity or vice versa.  Right
c             now we choose to extrapolate \rho using a limited extrapolation starting with 3^{rd}
c             order that gets reduced in order until the density is positive.  The
c             remaining condition looks (kind-of) like a scaled vector symmetry; the velocity
c             components are computed by solving:
c
c             [ a_{11}  a_{12} ]        { u }          { -\alpha(a_{11}u + a_{12}v) }
c             [                ]        {   } =        {                            }
c             [ a_{21}  a_{22} ]_{i-1}  { v }          {         a_{21}u + a_{22}v  }_{i+1}
c
c             where \alpha = \rho_{i+1} / \rho{i-1}.             
c
c             Note the previous discussion applies to the "r" or "i" direction fluxes;
c             the other direction is handled by replacing "i" with "j" and switching the odd/even vector
c             symmetry condition.
c
c         vi) Boundary conditions for T (temperature) must be done outside this function
c
c        vii) the swirl component (if present) is just extrapolated
c       viii) the loops/indices are 0 based 
c
      implicit none

c     INPUT
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ipar(0:*)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real aj(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real rpar(0:*)
      integer indexRange(0:1,0:2), bc(0:1,0:2)
      integer cfrhs

c     OUTPUT
c     u adjusted at boundaries and ghost points

c     LOCAL
      integer i1,i2,i3,is1,is2,is3,iss(0:2),c,axis,side,irb(0:1,0:2),a,
     & s
      integer rc,uc,vc,wc,tc,grid,gridtype,debug,isaxi,radaxis,
     & withswirl
      integer i,j
      real alpha,aa(0:2,0:2),jdet,jdetg,rtmp,aainv(0:2,0:2),den,rhs(
     & 0:2)
      real rr

c     LOCAL PARAMETERS
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
      integer noSlipWall,slipWall
      parameter( noslipwall=1,slipWall=4 )
      real one
      parameter (one=1d0)
      real eps
      parameter (eps=0.1d0)

c     STATEMENT FUNCTIONS
c     in the following extrapolation functions, (i1,i2,i3) refers to the ghost point
c     1st order extrapolation of u, actually neumann if the boundary point is small
      real exo1,exo2,exo3
      exo1(i1,i2,i3,is1,is2,is3,c) = max( u(i1+is1,i2+is2,i3+is3,c),
     &                                u(i1+2*is1,i2+2*is2,i3+2*is3,c) )
c     2nd order extrapolation of u
      exo2(i1,i2,i3,is1,is2,is3,c) = 2d0*u(i1+is1,i2+is2,i3+is3,c) -
     & u(i1+2*is1,i2+2*is2,i3+2*is3,c)
c     3rd order extrapolation of u
      exo3(i1,i2,i3,is1,is2,is3,c) =3d0*u(i1+  is1,i2+  is2,i3+  is3,c)
     & -
     & 3d0*u(i1+2*is1,i2+2*is2,i3+2*is3,c)+
     &                                  u(i1+3*is1,i2+3*is2,i3+3*is3,c)

c      print *,"INSIDE NO SLIP WALL"
      rc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      tc                =ipar(4)

      grid              =ipar(7)
      gridType          =ipar(8)
      isaxi             =ipar(12)

      debug             =ipar(15)
      radaxis           =ipar(18) ! =axis1 if x=0 is axis of cylindrical symmetry, =axis2 if y=0 is..
      withswirl         =ipar(19)

      do a=0,2
         do s=0,1
            irb(s,a)=0
         end do
      end do

      do axis=0,nd-1
      do side=0,1

      if ( bc(side,axis).eq.noSlipWall .or.
     &     bc(side,axis).eq.slipWall ) then

         iss(0) = 0
         iss(1) = 0
         iss(2) = 0
         iss(axis) = 1-2*side
         is1=iss(0)
         is2=iss(1)
         is3=iss(2)

         do 100 a=0,nd-1
            do 100 s=0,1
               irb(s,a) = indexRange(s,a)
 100     continue ! ha, old school just for fun

         irb(0,axis) = indexRange(side,axis)
         irb(1,axis) = indexRange(side,axis)

         do i1=irb(0,0),irb(1,0)
         do i2=irb(0,1),irb(1,1)
         do i3=irb(0,2),irb(1,2)

            rtmp = exo3(i1-is1,i2-is2,i3-is3,is1,is2,is3,rc)
c 060110 machine epsilon is too tight a tolerance, esp when extrapolating near zero, 
c        instead limit the extrapolation to keep the density from changing more than, say,
c        one or two orders of magnitude
c     
c            if ( (1d0+rtmp/u(i1+is1,i2+is2,i3+is3,rc)).le.one ) then
            if ( .true. .or. !i2.eq.irb(0,1).or.
     &           (((rtmp/u(i1+is1,i2+is2,i3+is3,rc)).lt.eps) .or.
     &           ((u(i1+is1,i2+is2,i3+is3,rc)/rtmp).lt.eps)) ) then
c               print *,"small density, using exo2 ",i1,i2,rtmp
               rtmp = exo2(i1-is1,i2-is2,i3-is3,is1,is2,is3,rc)
c               if ( (1d0+rtmp/u(i1+is1,i2+is2,i3+is3,rc)).le.one ) then
               if ( !i2.eq.irb(0,1).or.
     &              (((rtmp/u(i1+is1,i2+is2,i3+is3,rc)).lt.eps).or.
     &              ((u(i1+is1,i2+is2,i3+is3,rc)/rtmp).lt.eps) )) then
c                  print *,"small density, using exo1 ",i1,i2,rtmp
c                  if ( i2.eq.irb(0,1) ) then
c                     rtmp = u(i1+is1,i2+is2,i3+is3,rc)
c                  else
                     rtmp = exo1(i1-is1,i2-is2,i3-is3,is1,is2,is3,rc)
c                  endif
c                 if ( i2.ne.irb(0,1).and.(u(i1,i2,i3,rc).gt.
c     &                u(i1+is1,i2+is2,i3+is3,rc)) ) then
c                    rtmp = u(i1,i2,i3,rc)
c                  else
c                     rtmp = u(i1+is1,i2+is2,i3+is3,rc)
c                  endif
c                  print *,"final density = ",rtmp
               end if
            end if

            u(i1-is1,i2-is2,i3-is3,rc) = rtmp
c XXX should still check for small or negative density...
c            if ( rtmp.lt.1e-40 ) print *,"low density :",i1,i2,rtmp
c            if (u(i1,i2,i3,rc).lt.1e-20) 
c     &           print *,"low bdy density : ",i1,i2,u(i1,i2,i3,rc)
            alpha = u(i1+is1,i2+is2,i3+is3,rc)/rtmp

            if (isaxi.eq.1 .and. !.false. .and.
     &          (1d0+abs(x(i1-is1,i2-is2,i3-is3,radaxis))).gt.one .and.
     & (1d0+abs(x(i1+is1,i2+is2,i3+is3,radaxis))).gt.one ) then
c               print *,"alpha was  ",alpha

               rr = x(i1+is1,i2+is2,i3+is3,radaxis)/
     &                       x(i1-is1,i2-is2,i3-is3,radaxis)
c               if ( abs(rr).gt.2d0 ) then
c                  rr = sign(2d0,rr)
c               else if ( abs(rr).lt.(.5d0) ) then
c                  rr = sign(.5d0,rr)
c               endif

ckkc 060712               if ( rr.lt.0d0 ) rr = 1d0

               alpha = alpha*rr
c               print *,"alpha is now  ",alpha
c               print *,"rads ",x(i1+is1,i2+is2,i3+is3,radaxis),
c    & x(i1-is1,i2-is2,i3-is3,radaxis)

            end if

            if ( nd.eq.2 ) then

               do i=0,nd-1
                  do j=0,nd-1
                     aa(i,j) = rsxy(i1-is1,i2-is2,i3-is3,i,j)
                  end do
               end do
               jdetg = aa(0,0)*aa(1,1)-aa(0,1)*aa(1,0)
               den = 1d0/(aj(i1-is1,i2-is2,i3-is3)*jdetg)
               aainv(0,0) = aa(1,1)*den
               aainv(1,1) = aa(0,0)*den
               aainv(1,0) = -aa(1,0)*den
               aainv(0,1) = -aa(0,1)*den

            else ! 3d

            end if ! if 2d


            jdet = aj(i1+is1,i2+is2,i3+is3)
            do i=0,nd-1
               do j=0,nd-1
                  aa(i,j) = jdet*rsxy(i1+is1,i2+is2,i3+is3,i,j)
               end do
            end do

            do i=0,nd-1
               rhs(i)=0d0
               do j=0,nd-1
                rhs(i) = rhs(i)+(u(i1+is1,i2+is2,i3+is3,uc+j)*aa(i,j))
               end do
ckkc 060213 actually DON'T scale the tangential component
ckkc 060412 ok, actually DO scale it
               rhs(i) = alpha*rhs(i)
ckkc 060706 ok, what is the right thing to do ???
ckkc               rhs(i) = rhs(i)
            end do

ckkc 060213 scale ONLY the normal component            
ckkc 060412 actually do scale the tangential           
            rhs(axis) = -rhs(axis)
ckkc 060706 ok, what is the right thing to do ???
ckkc            rhs(axis) = -alpha*rhs(axis)

            do i=0,nd-1
               u(i1,i2,i3,uc+i) = 0d0 ! this is just the no-slip wall condition
               u(i1-is1,i2-is2,i3-is3,uc+i) = 0d0
               do j=0,nd-1
                  u(i1-is1,i2-is2,i3-is3,uc+i) =
     &            u(i1-is1,i2-is2,i3-is3,uc+i) + rhs(j)*aainv(i,j)
               end do
            end do

            if ( withswirl.eq.1 ) then
               u(i1-is1,i2-is2,i3-is3,wc) =
     &                exo2(i1-is1,i2-is2,i3-is3,is1,is2,is3,wc)
            end if

         end do !i1
         end do !i2
         end do !i3

      end if ! if noSlipWall

      end do ! side
      end do ! axis


      return
      end

      subroutine cnsNoSlipWallBCCoeff(nd,nd1a,nd1b,nd2a,nd2b,nd3a,
     &     nd3b,nd4a,nd4b,coeff,rhs,u,x,aj,rsxy,iprm,rprm,
     &     indexRange,bc,bd, bt, nbv, cfrhs)
c
c     no-slip wall, zero mass flux boundary conditions for icns
c
c
c     060312 kkc : Initial Version
c
c     This subroutine applies the no-slip wall boundary condition to the coefficient matrix
c     and the right hand side vector for the implicit CNS method.
c
c     Notes:
c          i) we set velocity = 0 on the boundary
c         ii) zero mass flux implies f_{i-1/2}+f_{i+1/2}=0  where i is on the boundary and
c             f is the total flux (convective and artificial viscosity) for the continuity
c             equation.
c         iv) There are several ways to impose the flux condition.  Right now we assume
c             that viscous fluxes, f^a, satisfy f^a_{i-1/2} = -f^a_{i+1/2} when the artificial
c             viscosity is computed. XXX NOTE THIS IS A CONDITION ON icns.
c          v) Given (iv) and the simple flux averagin in cnsdu,  we now have the condition
c             \rho_{i-1}( a_{11}u + a_{12}v )_{i-1} ) = -\rho_{i+1}( a_{11}u + a_{12}v )_{i+1} ) .
c             We can either extrapolate \rho and compute the velocity or vice versa.  Right
c             now we choose to extrapolate \rho using a limited extrapolation starting with 3^{rd}
c             order that gets reduced in order until the density is positive.  The
c             remaining condition looks (kind-of) like a scaled vector symmetry; the velocity
c             components are computed by solving:
c
c             [ a_{11}  a_{12} ]        { u }          { -\alpha(a_{11}u + a_{12}v) }
c             [                ]        {   } =        {                            }
c             [ a_{21}  a_{22} ]_{i-1}  { v }          {         a_{21}u + a_{22}v  }_{i+1}
c
c             where \alpha = \rho_{i+1} / \rho{i-1}.             
c
c             Note the previous discussion applies to the "r" or "i" direction fluxes;
c             the other direction is handled by replacing "i" with "j" and switching the odd/even vector
c             symmetry condition.
c
c         vi) Boundary conditions for T (temperature) must be done outside this function
c
c        vii) the swirl component (if present) is just extrapolated
c       viii) the loops/indices are 0 based 
c
c         ix) cfrhs is a switch,  cfrhs.eq.0 - fill in coefficient array, otherwise fill in rhs
c          x) XXX THE EQUATION NUMBERS ARE CURRENTLY NOT ALTERED IN THE SPARSE REP!!! 
c                 THIS ROUTINE RELIES ON THE DEFAULT NUMBERING PROVIDED BY THE SPARSE REP FOR MGF
c                 this means that the boundary condition operators for the coeff array should NEVER
c                 be called for the density and velocity components on the boundary and first ghost lines
c                 (because those reset the equation number array)
      implicit none

c     INPUT
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,iprm(0:*)
      real coeff(0:iprm(10)*iprm(1)**2-1,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b) ! coefficient matrix
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b) ! state to linearize about
      real rhs(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b) ! right hand side vector
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1) ! grid vertices
      real aj(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b) ! determinant of grid Jacobian matrix
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1) ! metric derivatives
      real rprm(*)
      integer indexRange(0:1,0:2), bc(0:1,0:2),nbv
      real bd(0:nbv-1,0:1,0:2,0:*)
      integer bt(0:1,0:1,0:2,0:*)
      integer cfrhs

c     OUTPUT
c     u adjusted at boundaries and ghost points

c     LOCAL
      integer i1l,i2l,i3l,eqn,cc
      integer i1,i2,i3,is1,is2,is3,iss(0:2),c,axis,side,irb(0:1,0:2),a,
     & s
      integer i1g,i2g,i3g,i1i,i2i,i3i,e,ncmp,ndim,gmove,neq,teq,ic1,ic2
      integer rc,uc,vc,wc,tc,grid,gridtype,debug,isaxi,radaxis,
     & withswirl
      integer i,j,isten_size,cbnd,width,width3,hwidth,hwidth3
      real rr,aa(0:2,0:2),jdet,jdetg,rtmp,aainv(0:2,0:2),den,xr
      real ubv(0:30),norm(3),tang(3),grav(3)
      logical useneumanntemp,iscorner
      real ren,prn,man,gam,gm1,gm2, oren, oprn, f43, f13,coeff_u,
     & coeff_v

c     LOCAL PARAMETERS
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
      integer noSlipWall,slipWall
      parameter( noslipwall=1,slipWall=4 )
      real one
      parameter (one=1d0)
      real eps
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
      real exo1,exo2,exo3
      real rx,xyz

      include 'icnssfdec.h'
      rx(i1,i2,i3,e,c) = rsxy(i1,i2,i3,e-1,c-1)
      xyz(i1,i2,i3,c) = x(i1,i2,i3,c)
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
      usestrik = .true.
      alpha = 1d0/6d0
c      alpha = 1d0/12d0
      off(1)=0
      off(2)=0
      off(3)=0
      occ=0
      oce=0

c      print *,"inside no slip wall"
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
c      av2 = rprm(13)
c      av4 = rprm(14)
      grav(1) = rprm(15)
      grav(2) = rprm(16)
      grav(3) = rprm(17)

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


c      print *,oren,oprn
      do a=0,2
         do s=0,1
            irb(s,a)=0
         end do
      end do

      do axis=0,nd-1
      do side=0,1

      if ( bc(side,axis).eq.noSlipWall .or.
     &     bc(side,axis).eq.slipWall)  then

c          grab the user defined bc values
         useneumanntemp = .true.
         if ( bc(side,axis).ne.slipWall .and.
     &        bt(0,side,axis,grid).gt.3 ) then
            do c=0, nbv-1
               ubv(c) = bd(c,side,axis,grid)
            end do

            if ( ( bt(0,side,axis,grid).eq.linearrampinx .and.
     &           ubv(3*(max(vc,wc)+1)).gt.0d0 ) .or.
     &           ( bt(0,side,axis,grid).eq.axisymmetricsbr .and.
     &           ubv(3).gt.0d0 ) ) then
               useneumanntemp=.false.
            endif
         endif

         iss(0) = 0
         iss(1) = 0
         iss(2) = 0
         iss(axis) = 1-2*side
         is1=iss(0)
         is2=iss(1)
         is3=iss(2)

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

         do i1l=irb(0,0),irb(1,0)
         do i2l=irb(0,1),irb(1,1)
         do i3l=irb(0,2),irb(1,2)

            i1g = i1l-is1
            i2g = i2l-is2
            i3g = i3l-is3
            i1  = i1l
            i2  = i2l
            i3  = i3l
            i1i = i1l+is1
            i2i = i2l+is2
            i3i = i3l+is3

            iscorner = (
     & (i1.eq.indexRange(0,0) .and. i2.eq.indexRange(0,1) .and.
     &           bc(0,0).eq.noslipwall .and. bc(0,1).eq.noslipwall).or.
     & (i1.eq.indexRange(0,0) .and. i2.eq.indexRange(1,1) .and.
     &           bc(0,0).eq.noslipwall .and. bc(1,1).eq.noslipwall).or.
     & (i1.eq.indexRange(1,0) .and. i2.eq.indexRange(0,1) .and.
     &           bc(1,0).eq.noslipwall .and. bc(0,1).eq.noslipwall).or.
     & (i1.eq.indexRange(1,0) .and. i2.eq.indexRange(1,1) .and.
     &           bc(1,0).eq.noslipwall .and. bc(1,1).eq.noslipwall))

c             iscorner = .false.
c            if ( iscorner ) then
c               print *,"iscorner ",iscorner,grid,i1,i2
c               if ( (i1.eq.irb(0,0) .and. i2.eq.irb(0,1) .and.
c     &           bc(0,0).eq.noslipwall .and. bc(0,1).eq.noslipwall))then
c                  print *,"bc,side,axis ",bc(0,0),bc(0,1),noslipwall
c               endif
c            endif
c           we are going to set the equations for the density and velocity components so initialize those rows
            if ( .true. ) then ! but only when first hitting the corner
            if ( cfrhs.eq.0 ) then

c           reset coefficient matrix
               do m3=-hwidth3,hwidth3
               do m2=-hwidth, hwidth
               do m1=-hwidth, hwidth
                  do c=0,ncmp-1
c                     if ( .not.(iscorner.and.(axis.gt.0))) then
                     do e=rc,tc
                    coeff(icf(m1,m2,m3,e,c),i1g,i2g,i3g)=0d0
                     end do

                     if ( bc(side,axis).eq.noslipwall ) then

                        do e=uc,max(vc,wc)
                           coeff(icf(m1,m2,m3,e,c),i1,i2,i3)=0d0
                        end do
                     else if ( withswirl.gt.0 ) then
c                        coeff(icf(m1,m2,m3,wc,c),i1,i2,i3)=0d0
                     endif

                     if ( .not. useneumanntemp ) then
                        coeff(icf(m1,m2,m3,tc,c),i1,i2,i3)=0d0
                     endif
c                     if ( iscorner .and. .true. ) then
                     if ( iscorner ) then!or. side.eq.0 ) then
                        coeff(icf(m1,m2,m3,rc,c),i1,i2,i3)=0d0
                     endif
c                     endif
                  end do
               end do
               end do
               end do
            else
c           reset rhs
c               if ( .not.(iscorner.and.(axis.gt.0))) then
               do e=rc,tc
                  rhs(i1g,i2g,i3g,e) = 0d0
               end do
               if ( bc(side,axis).eq.noslipwall ) then
                  do e=uc,max(vc,wc)
                     rhs(i1,i2,i3,e) = 0d0
                  end do
               else if ( withswirl .gt. 0 ) then
c                  rhs(i1,i2,i3,wc) = 0d0
               endif

               if ( iscorner ) rhs(i1,i2,i3,rc)=0d0 !.or. side.eq.0 ) rhs(i1,i2,i3,rc) = 0d0


c               endif

            endif
            endif ! ok to zero out

         a = mod(axis+1,ndim)

         norm(1) = rsxy(i1,i2,i3,a,1)
         norm(2) = -rsxy(i1,i2,i3,a,0)
         tang(1) = -rsxy(i1,i2,i3,axis,1)
         tang(2) = rsxy(i1,i2,i3,axis,0)

         norm(1) = norm(1)/
     &        sqrt(norm(1)*norm(1)+norm(2)*norm(2))
         norm(2) = norm(2)/
     &        sqrt(norm(1)*norm(1)+norm(2)*norm(2))
         tang(1) = tang(1)/sqrt(tang(1)*tang(1)+tang(2)*tang(2))
         tang(2) = tang(2)/sqrt(tang(1)*tang(1)+tang(2)*tang(2))

            rtmp = u(i1g,i2g,i3g,rc)

            if ( .true. .or.
     &           bc(side,axis).eq.slipwall ) then
c      OLD WAY TO SET DENSITY AT THE GHOST POINT (EXTRAPOLATION)
c            rtmp = exo3(i1g,i2g,i3g,is1,is2,is3,rc)
c         we only use at most linear extrapolation in the implicit code since it only needs the same width
c           stencil (i.e. same default equation numbers) as the discretization
               rtmp = exo2(i1g,i2g,i3g,is1,is2,is3,rc)
c 060110 machine epsilon is too tight a tolerance, esp when extrapolating near zero, 
c        instead limit the extrapolation to keep the density from changing more than, say,
c        one or two orders of magnitude
c     

c        base the equation for the ghost point density on the order of extrapolation used for the ul density
               xr = x(i1g,i2g,i3g,radaxis)
               if (             !.true. .or.
     & ((rtmp.lt.0d0) .or. (isaxi.ne.0 .and. xr.lt.0d0 ) .or.
     &              (((rtmp/u(i1i,i2i,i3i,rc)).lt.eps).or.
     &              ((u(i1i,i2i,i3i,rc)/rtmp).lt.eps))) ) then
                  rtmp = exo1(i1g,i2g,i3g,is1,is2,is3,rc)
c     use constant extrapolation or Neumann 
                  if ( cfrhs.eq.0 ) then
c     print *,"using const extrap for rho (cf) : ",i1,i2,i3,rtmp
                     coeff(icf(0,0,0,rc,rc),i1g,i2g,i3g) = 1d0
                     if ( (isaxi.ne.0 .and. xr.lt.0d0 ) .or.
     & (u(i1,i2,i3,rc).gt.u(i1+is1,i2+is2,i3+is3,rc)) ) then
                        coeff(icf(is1,is2,is3,rc,rc),i1g,i2g,i3g) =-1d0
                     else
                    coeff(icf(2*is1,2*is2,2*is3,rc,rc),i1g,i2g,i3g)=-
     & 1d0
                     endif
                  else
c     print *,"using const extrap for rho (r) : ",i1,i2,i3,rtmp
                     rhs(i1g,i2g,i3g,rc) = 0d0
                  endif

               else             ! use linear extrapolation for the ghost point density

                  if ( cfrhs.eq.0 ) then
c     print *,"using linear extrap for rho (cf) : ",i1,i2,i3,rtmp
                     coeff(icf(0,0,0,rc,rc),i1g,i2g,i3g) = 1d0
                     coeff(icf(is1,is2,is3,rc,rc),i1g,i2g,i3g) = -2d0
                   coeff(icf(2*is1,2*is2,2*is3,rc,rc),i1g,i2g,i3g) =+
     & 1d0
                  else
c     print *,"using linear extrap for rho (r) : ",i1,i2,i3,rtmp
                     rhs(i1g,i2g,i3g,rc) = 0d0
                  endif

               end if

c               if ( iscorner .and. (cfrhs.eq.0) ) then
               if ( iscorner .and. (cfrhs.eq.0) ) then
c     print *,"corner at ",i1,",  ",i2
                  if ( i1.eq.indexRange(0,0) ) then
                     ic1 = 1
                  else
                     ic1 = -1
                  endif
                  if ( i2.eq.indexRange(0,1) ) then
                     ic2 = 1
                  else
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

               else if ( .false. .and. cfrhs.eq.0 )then !.and. side.eq.0 ) then
                  coeff(icf(0,0,0,rc,rc),i1,i2,i3) = 1d0
                  coeff(icf(is1,is2,is3,rc,rc),i1,i2,i3) = -2d0
                  coeff(icf(2*is1,2*is2,2*is3,rc,rc),i1,i2,i3) =+1d0

               endif

            else if ( .false. ) then !.or.  rtmp.gt.0d0 ) then
c         NEW WAY TO SET DENSITY AT THE GHOST POINT (use momentum equation at the boundary)
c              this is not really appropriate for the slip wall
               a = mod(axis+1,ndim)

c     if ( axis.eq.0 ) then
               norm(1) = rsxy(i1,i2,i3,a,1)
               norm(2) = -rsxy(i1,i2,i3,a,0)
               norm(1) = norm(1)/
     &              sqrt(norm(1)*norm(1)+norm(2)*norm(2))
               norm(2) = norm(2)/
     &              sqrt(norm(1)*norm(1)+norm(2)*norm(2))

c           i1,i2,i3 are set by the icnscf macros
               i1 = i1g
               i2 = i2g
               i3 = i3g

               if ( cfrhs.eq.0 ) then
c     set coefficient array
                  eqn = rc
                  off(axis+1) = 1-2*side !shift application of the stencil to the boundary point

                  if ( .true. .and. bc(side,axis).eq.slipWall ) then
c     add the convective part too
                     if ( .true. ) then
                     do cc=uc, uc+ndim-1
                        d = cc-uc+1

c     u[cc]^{n+1} * D_d (u[eqn]^n)
c     coeff(icf(0,0,0,eqn,cc),i1,i2,i3) =
c     &                 coeff(icf(0,0,0,eqn,cc),i1,i2,i3) + 
c     &                    ux(i1,i2,i3,eqn,d)
c     &                 rx(i1,i2,i3,1,d)*ur + rx(i1,i2,i3,2,d)*us

c                 u[cc]^n * (r_d u[eqn]_r^{n+1} + s_d u[eqn]_s^{n+1} )
c                  UXC(d, (u(i1,i2,i3,cc)), eqn)
c     coefficients for a  difference approximation to -u(i1l,i2l,i3l,rc)*u(i1l,i2l,i3l,cc)*norm(1) * u[uc]_X
c     r-part
                              coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) + (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(1))*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*
     & dri2(1)
                              coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) - (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(1))*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*
     & dri2(1)
c     s-part
                              coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) + (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(1))*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*
     & dri2(2)
                              coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) - (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(1))*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*
     & dri2(2)
c     coefficients for a  difference approximation to -u(i1l,i2l,i3l,rc)*u(i1l,i2l,i3l,cc)*norm(2) * u[vc]_X
c     r-part
                              coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) + (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(2))*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*
     & dri2(1)
                              coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) - (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(2))*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*
     & dri2(1)
c     s-part
                              coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) + (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(2))*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*
     & dri2(2)
                              coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) - (-u(i1l,i2l,i3l,rc)*u(i1l,
     & i2l,i3l,cc)*norm(2))*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*
     & dri2(2)
                        coeff(icf(0,0,0,eqn,cc),i1,i2,i3) =
     &                       coeff(icf(0,0,0,eqn,cc),i1,i2,i3) -
     &                 norm(1)*u(i1l,i2l,i3l,rc)*ux(i1l,i2l,i3l,uc,d)-
     &                 norm(2)*u(i1l,i2l,i3l,rc)*ux(i1l,i2l,i3l,vc,d)

                     end do     ! cc (each vel. component)
                        coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                       coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 norm(1)*(u(i1l,i2l,i3l,uc)*ux(i1l,i2l,i3l,uc,1)+
     & u(i1l,i2l,i3l,vc)*ux(i1l,i2l,i3l,uc,2))-
     &                 norm(2)*(u(i1l,i2l,i3l,uc)*ux(i1l,i2l,i3l,vc,1)+
     &                          u(i1l,i2l,i3l,vc)*ux(i1l,i2l,i3l,vc,2))
                     endif
                  endif

c              u-momentum part
c     9 point curvilinear grid second derivative stencil:  (f43*oren*norm(1)) * u[uc]_{XY}
                                 coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,1)*dri2(2))
                                 coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,1)*dri2(1))
                                 coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(f43*oren*norm(1))
     & )*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2)
     & ,i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2))
                                 coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,1)*dri2(1))
                                 coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,1)*dri2(2))
                                 coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (f43*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
c     9 point curvilinear grid second derivative stencil:  (oren*norm(1)) * u[uc]_{XY}
                                 coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,2)*dri2(2))
                                 coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,2)*dri2(1))
                                 coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(3)
     & ,2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2))
                                 coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,2)*dri2(1))
                                 coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,2)*dri2(2))
                                 coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (oren*norm(1)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
c     9 point curvilinear grid second derivative stencil:  (f13*oren*norm(1)) * u[vc]_{XY}
                                 coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,2)*dri2(2))
                                 coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,2)*dri2(1))
                                 coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(f13*oren*norm(1))
     & )*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2)
     & ,i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2))
                                 coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,2)*dri2(1))
                                 coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,2)*dri2(2))
                                 coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (f13*oren*norm(1)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                     if ( isaxi.gt.0 .and. x(i1,i2,i3,1).gt.0d0 ) then
c     coefficients for a  difference approximation to (oren/x(i1l,i2l,i3l,1)*norm(1)) * u[uc]_X
c     r-part
                              coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) + ((oren/x(i1l,i2l,i3l,1)*
     & norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                              coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) - ((oren/x(i1l,i2l,i3l,1)*
     & norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                              coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) + ((oren/x(i1l,i2l,i3l,1)*
     & norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                              coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = 
     & coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) - ((oren/x(i1l,i2l,i3l,1)*
     & norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
c     coefficients for a  difference approximation to (f13*oren/x(i1l,i2l,i3l,1)*norm(1)) * u[vc]_X
c     r-part
                              coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) + ((f13*oren/x(i1l,i2l,i3l,
     & 1)*norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
                              coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) - ((f13*oren/x(i1l,i2l,i3l,
     & 1)*norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
c     s-part
                              coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) + ((f13*oren/x(i1l,i2l,i3l,
     & 1)*norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
                              coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) - ((f13*oren/x(i1l,i2l,i3l,
     & 1)*norm(1)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
                     endif

c              v-momentum part
c     9 point curvilinear grid second derivative stencil:  (f43*oren*norm(2)) * u[vc]_{XY}
                                 coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,2)*dri2(2))
                                 coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,2)*dri2(1))
                                 coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(f43*oren*norm(2))
     & )*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2)
     & ,i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2))
                                 coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,2)*dri2(1))
                                 coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,2)*dri2(2))
                                 coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (f43*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
c     9 point curvilinear grid second derivative stencil:  (oren*norm(2)) * u[vc]_{XY}
                                 coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,1)*dri2(2))
                                 coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,1)*dri2(1))
                                 coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(3)
     & ,2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2))
                                 coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,1)*dri2(1))
                                 coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                                 coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,1)*dri2(2))
                                 coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) = 
     &  coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (oren*norm(2)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
c     9 point curvilinear grid second derivative stencil:  (f13*oren*norm(2)) * u[uc]_{XY}
                                 coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,1)*dri2(2))
                                 coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,1)*dri2(1))
                                 coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(f13*oren*norm(2))
     & )*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2)
     & ,i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2))
                                 coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,1)*dri2(1))
                                 coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                                 coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,1)*dri2(2))
                                 coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) = 
     &  coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (f13*oren*norm(2)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                     if ( isaxi.gt.0 .and. x(i1,i2,i3,1).gt.0d0 ) then
c     coefficients for a  difference approximation to (f43*oren*norm(2)/x(i1l,i2l,i3l,1)) * u[vc]_X
c     r-part
                              coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) + ((f43*oren*norm(2)/x(i1l,
     & i2l,i3l,1)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                              coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) - ((f43*oren*norm(2)/x(i1l,
     & i2l,i3l,1)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                              coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) + ((f43*oren*norm(2)/x(i1l,
     & i2l,i3l,1)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                              coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) - ((f43*oren*norm(2)/x(i1l,
     & i2l,i3l,1)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                        coeff(icf(0,0,0,eqn,vc),i1,i2,i3) =
     &                       coeff(icf(0,0,0,eqn,vc),i1,i2,i3) -
     &                       f43*oren/(x(i1l,i2l,i3l,1)**2)*norm(2)
                     endif

                  if ( withswirl.gt.0 ) then
                     if ( .true. ) then
c     -u[wc]^2/r 
c     will contribute to RHS

                        coeff(icf(0,0,0,eqn,wc),i1,i2,i3) =
     &                       coeff(icf(0,0,0,eqn,wc),i1,i2,i3) -
     & norm(2)*2d0 * u(i1l,i2l,i3l,rc)*u(i1l,i2l,i3l,wc)/
     &                       x(i1l,i2l,i3l,1)

                        coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                       coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                       norm(2)*u(i1l,i2l,i3l,wc)**2/
     &                       x(i1l,i2l,i3l,1)

                     endif
                  endif

c                  UUXC(1,-gm2*norm(1),tc,rc)
c                  UUXC(2,-gm2*norm(2),tc,rc)
c                  UUXC(1,-gm2*norm(1),rc,tc)
c                  UUXC(2,-gm2*norm(2),rc,tc)
c     coefficients for a  difference approximation to -gm2*u(i1l,i2l,i3l,tc)*norm(1) * u[rc]_X
c     r-part
                      coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) = coeff(icf(+
     & 1,0,0,eqn,rc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,tc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
                      coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) = coeff(icf(-
     & 1,0,0,eqn,rc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,tc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
c     s-part
                      coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) = coeff(icf(0,
     & +1,0,eqn,rc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,tc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
                      coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) = coeff(icf(0,
     & -1,0,eqn,rc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,tc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
c     coefficients for a  difference approximation to -gm2*u(i1l,i2l,i3l,tc)*norm(2) * u[rc]_X
c     r-part
                      coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) = coeff(icf(+
     & 1,0,0,eqn,rc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,tc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                      coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) = coeff(icf(-
     & 1,0,0,eqn,rc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,tc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                      coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) = coeff(icf(0,
     & +1,0,eqn,rc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,tc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                      coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) = coeff(icf(0,
     & -1,0,eqn,rc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,tc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &              coeff(icf(0,0,0,eqn,tc),i1,i2,i3) -
     &               gm2*norm(1)*ux(i1l,i2l,i3l,rc,1) -
     &               gm2*norm(2)*ux(i1l,i2l,i3l,rc,2)
c     coefficients for a  difference approximation to -gm2*u(i1l,i2l,i3l,rc)*norm(1) * u[tc]_X
c     r-part
                      coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(+
     & 1,0,0,eqn,tc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,rc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
                      coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(-
     & 1,0,0,eqn,tc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,rc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
c     s-part
                      coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) = coeff(icf(0,
     & +1,0,eqn,tc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,rc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
                      coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) = coeff(icf(0,
     & -1,0,eqn,tc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,rc)*norm(1))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
c     coefficients for a  difference approximation to -gm2*u(i1l,i2l,i3l,rc)*norm(2) * u[tc]_X
c     r-part
                      coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(+
     & 1,0,0,eqn,tc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,rc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                      coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(-
     & 1,0,0,eqn,tc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,rc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                      coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) = coeff(icf(0,
     & +1,0,eqn,tc),i1,i2,i3) + (-gm2*u(i1l,i2l,i3l,rc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                      coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) = coeff(icf(0,
     & -1,0,eqn,tc),i1,i2,i3) - (-gm2*u(i1l,i2l,i3l,rc)*norm(2))*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &              coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &               gm2*norm(1)*ux(i1l,i2l,i3l,tc,1) -
     &               gm2*norm(2)*ux(i1l,i2l,i3l,tc,2)

                coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &               norm(1)*grav(1)+norm(2)*grav(2)

                  off(axis+1) = 0 !reset offset for icf
               else
c           set right hand side

                  if ( .true. .and. bc(side,axis).eq.slipWall ) then
                     if ( .true. ) then
                     do cc=uc, uc+ndim-1
                        d = cc-uc+1

                        rhs(i1,i2,i3,rc) = rhs(i1,i2,i3,rc) +
     &                       u(i1l,i2l,i3l,rc)*2d0*(
     &                  -norm(1)*u(i1l,i2l,i3l,cc)*ux(i1l,i2l,i3l,uc,d)
     & -norm(2)*u(i1l,i2l,i3l,cc)*ux(i1l,i2l,i3l,vc,d))
                     end do
                     endif
                  endif

c     
                  rhs(i1,i2,i3,rc) = rhs(i1,i2,i3,rc) -
     &            u(i1l,i2l,i3l,tc)*(gm2*norm(1)*ux(i1l,i2l,i3l,rc,1)+
     &                 gm2*norm(2)*ux(i1l,i2l,i3l,rc,2))-
     & u(i1l,i2l,i3l,rc)*(gm2*norm(1)*ux(i1l,i2l,i3l,tc,1)+
     &                 gm2*norm(2)*ux(i1l,i2l,i3l,tc,2))
c     &              u(i1l,i2l,i3l,rc)*(norm(1)*grav(1)+norm(2)*grav(2))-

                  if ( withswirl.gt.0 ) then
                     if ( .true. ) then
                        rhs(i1,i2,i3,rc) = rhs(i1,i2,i3,rc) -
     &               2d0*norm(2)*u(i1l,i2l,i3l,rc)*u(i1l,i2l,i3l,wc)**2
                     endif
                  endif
c               print *,i1,i2,i3,norm(1)*grav(1),norm(2)*grav(2)
c        print *,i1,i2,i3,uxx(i1l,i2l,i3l,rc,1,1),uxx(i1l,i2l,i3l,rc,2,2)
c        print *,"      ",uxx(i1l,i2l,i3l,tc,1,1),uxx(i1l,i2l,i3l,tc,2,2)
               endif

c     reset these for the rest of the bc coding
               i1 = i1l
               i2 = i2l
               i3 = i3l

               if ( .true. .and. iscorner .and. (cfrhs.eq.0) ) then
c     print *,"corner at ",i1,",  ",i2
                  if ( i1.eq.indexRange(0,0) ) then
                     ic1 = 1
                  else
                     ic1 = -1
                  endif
                  if ( i2.eq.indexRange(0,1) ) then
                     ic2 = 1
                  else
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

C          else
C c     use constant extrapolation since we are very near zero
C             if ( cfrhs.eq.0 ) then
C c      print *,"using const extrap for rho (cf) : ",i1,i2,i3,rtmp
C                coeff(icf(0,0,0,rc,rc),i1g,i2g,i3g) = 1d0
C                if ( (isaxi.ne.0 .and. xr.lt.0d0 ) .or.
C      &            (u(i1,i2,i3,rc).gt.u(i1+is1,i2+is2,i3+is3,rc)) ) then
C                   coeff(icf(is1,is2,is3,rc,rc),i1g,i2g,i3g) =-1d0
C                else
C                   coeff(icf(2*is1,2*is2,2*is3,rc,rc),i1g,i2g,i3g)=-1d0
C                endif
C             else
C c              print *,"using const extrap for rho (r) : ",i1,i2,i3,rtmp
C                rhs(i1g,i2g,i3g,rc) = 0d0
C             endif
            endif

         rr = 1d0
         if (isaxi.ne.0 .and.
     &        (1d0+abs(x(i1g,i2g,i3g,radaxis))).gt.one .and.
     &        (1d0+abs(x(i1i,i2i,i3i,radaxis))).gt.one ) then

            rr = x(i1i,i2i,i3i,radaxis)/
     &           x(i1g,i2g,i3g,radaxis)

c            print *,i1,i2,rr
ckkc 060712 if ( rr.lt.0d0 ) rr = 1d0
c     if ( abs(rr).gt.2d0 ) then
c     rr = sign(2d0,rr)
c     else if ( abs(rr).lt.(.5d0) ) then
c     rr = sign(.5d0,rr)
c     endif

         end if

         if ( bc(side,axis).eq.noslipwall) then
c     The actuall no-slip wall velocity bc is installed here (homogeneous dirichlet)
            if ( cfrhs.eq.0 ) then
               coeff(icf(0,0,0,uc,uc),i1,i2,i3) = 1d0
               coeff(icf(0,0,0,vc,vc),i1,i2,i3) = 1d0


            else
               rhs(i1,i2,i3,uc) = 0d0
               rhs(i1,i2,i3,vc) = 0d0

            endif

         else if ( bc(side,axis).eq.slipwall) then

            neq = vc
            teq = uc

            if ( cfrhs.eq.0 ) then
               do m3=-hwidth3,hwidth3
               do m2=-hwidth, hwidth
               do m1=-hwidth, hwidth
                  do c=0,ncmp-1

                  coeff_u = coeff(icf(m1,m2,m3,uc,c),i1,i2,i3)
                  coeff_v = coeff(icf(m1,m2,m3,vc,c),i1,i2,i3)
                  coeff(icf(m1,m2,m3,neq,c),i1,i2,i3) = 0d0
                  coeff(icf(m1,m2,m3,teq,c),i1,i2,i3) =
     &              tang(1)*coeff_u + tang(2)*coeff_v
c                  if ( i2.eq.0 ) then
c                     print *,i1,i2,c,tang(1)*coeff_u,tang(2)*coeff_v
c                  endif
                  end do
               end do
               end do
               end do
               coeff(icf(0,0,0,neq,uc),i1,i2,i3) =
     &              norm(1)
               coeff(icf(0,0,0,neq,vc),i1,i2,i3) =
     &              norm(2)

            else
               rhs(i1,i2,i3,teq) =
     &             tang(1)*rhs(i1,i2,i3,uc)+tang(2)*rhs(i1,i2,i3,vc)
               rhs(i1,i2,i3,neq) = 0d0
            endif

         endif


         if ( cfrhs.eq.0 ) then
            if ( .not. useneumanntemp ) then
               coeff(icf(0,0,0,tc,tc),i1,i2,i3) = 1d0
c     extrapolate the ghost point
               coeff(icf(0,0,0,tc,tc),i1g,i2g,i3g) = 1d0
               coeff(icf(2*is1,2*is2,2*is3,tc,tc),i1g,i2g,i3g) = 1d0
               coeff(icf(is1,is2,is3,tc,tc),i1g,i2g,i3g) = -2d0
            else
c     add a neumann condition to form an adiabatic wall
               coeff(icf(2*is1,2*is2,2*is3,tc,tc),i1g,i2g,i3g) = 1d0
               coeff(icf(0,0,0,tc,tc),i1g,i2g,i3g) = -1d0

            endif
         else
            rhs(i1g,i2g,i3g,tc) = 0d0
            if ( .not. useneumanntemp ) then
               if ( bt(0,side,axis,grid).eq.axisymmetricsbr) then
                  rhs(i1,i2,i3,tc) = ubv(3)
               else
                  rhs(i1,i2,i3,tc) = ubv( 3*(max(vc,wc)+1) ) +
     &                 ubv( 3*(max(vc,wc)+1) +1 )*x(i1,i2,i3,0)
               endif
            endif
         endif

c     There are two equations to add, one for the normal component of the velocity at
c       the boundary and one for the tangential component.   

c            if ( i2.eq.irb(0,1) ) then
c               rr=1d0
c               rtmp=1d0
c            endif
c            rr = 1d0
c            write(*,*) i1g,i2g,rr,rtmp
         neq = uc               !+axis
         teq = vc               !-axis

c     if ( axis.eq.0 ) then
c     norm(1) = 1
c     norm(2) = 0
c     tang(1) = 0
c     tang(2) = 1
c     else
c     tang(1) = rsxy(i1,i2,i3,a,1)
c     tang(2) = -rsxy(i1,i2,i3,a,0)
c     norm(1) = -rsxy(i1,i2,i3,axis,1)
c     norm(2) = rsxy(i1,i2,i3,axis,0)

c     norm(1) = 0
c     norm(2) = 1
c     tang(1) = 1
c     tang(2) = 0
c     endif

c     print *,axis,side,norm(1),norm(2)
c     print *,"        ",tang(1),tang(2)
         if ( cfrhs.eq.0 ) then

c$$$  if ( i2.eq.irb(0,1) ) then
c$$$  write(*,*) "i1 = ",i1,", axis = ",axis,",rr = ",rr
c$$$  write(*,*)"   rtmp = ",rtmp,",ul(rc) = ",ul(i1i,i2i,i3i,rc)
c$$$  
c$$$  endif
            coeff(icf(0,0,0,neq,uc),i1g,i2g,i3g) =
     &           norm(1) * (
     & u(i1g,i2g,i3g,rc)*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0) )
c     &              rtmp*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0)

            coeff(icf(0,0,0,neq,vc),i1g,i2g,i3g) =
     &           norm(1) * (
     & u(i1g,i2g,i3g,rc)*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1) )
c     &              rtmp*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1)

            coeff(icf(0,0,0,neq,rc),i1g,i2g,i3g) =
     &           norm(1) * (
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0)*u(i1g,i2g,i3g,uc)+
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1)*u(i1g,i2g,i3g,vc) )

            coeff(icf(2*is1,2*is2,2*is3,neq,uc),i1g,i2g,i3g) =
     &           norm(1) * (
     & rr*u(i1i,i2i,i3i,rc)*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0) )
            coeff(icf(2*is1,2*is2,2*is3,neq,vc),i1g,i2g,i3g) =
     &           norm(1) * (
     &  rr*u(i1i,i2i,i3i,rc)*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1) )

            coeff(icf(2*is1,2*is2,2*is3,neq,rc),i1g,i2g,i3g) =
     &           norm(1) * (
     & rr*aj(i1i,i2i,i3i)*(rsxy(i1i,i2i,i3i,axis,0)*u(i1i,i2i,i3i,uc)+
     &           rsxy(i1i,i2i,i3i,axis,1)*u(i1i,i2i,i3i,vc) ) )

            coeff(icf(0,0,0,neq,uc),i1g,i2g,i3g) =
     &           coeff(icf(0,0,0,neq,uc),i1g,i2g,i3g) +
     &           norm(2) * (
     &           aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0) )
            coeff(icf(0,0,0,neq,vc),i1g,i2g,i3g) =
     &           coeff(icf(0,0,0,neq,vc),i1g,i2g,i3g) +
     &           norm(2) * (
     &           aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1) )

            coeff(icf(2*is1,2*is2,2*is3,neq,uc),i1g,i2g,i3g) =
     &           coeff(icf(2*is1,2*is2,2*is3,neq,uc),i1g,i2g,i3g) +
     &           norm(2) * (
     &           -rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0) )
            coeff(icf(2*is1,2*is2,2*is3,neq,vc),i1g,i2g,i3g) =
     &           coeff(icf(2*is1,2*is2,2*is3,neq,vc),i1g,i2g,i3g) +
     &           norm(2) * (
     &           -rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1) )


c     tangential eq

            coeff(icf(0,0,0,teq,uc),i1g,i2g,i3g) =
     &           tang(1) * (
     & u(i1g,i2g,i3g,rc)*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0) )
c     &              rtmp*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0)

            coeff(icf(0,0,0,teq,vc),i1g,i2g,i3g) =
     &           tang(1) * (
     & u(i1g,i2g,i3g,rc)*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1) )
c     &              rtmp*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1)

            coeff(icf(0,0,0,teq,rc),i1g,i2g,i3g) =
     &           tang(1) * (
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0)*u(i1g,i2g,i3g,uc)+
     & aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1)*u(i1g,i2g,i3g,vc) )


            coeff(icf(2*is1,2*is2,2*is3,teq,uc),i1g,i2g,i3g) =
     &           tang(1) * (
     & rr*u(i1i,i2i,i3i,rc)*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0) )
            coeff(icf(2*is1,2*is2,2*is3,teq,vc),i1g,i2g,i3g) =
     &           tang(1) * (
     & rr*u(i1i,i2i,i3i,rc)*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1) )

            coeff(icf(2*is1,2*is2,2*is3,teq,rc),i1g,i2g,i3g) =
     &           tang(1) * (
     & rr*aj(i1i,i2i,i3i)*(rsxy(i1i,i2i,i3i,axis,0)*u(i1i,i2i,i3i,uc)+
     &           rsxy(i1i,i2i,i3i,axis,1)*u(i1i,i2i,i3i,vc) ) )

            coeff(icf(0,0,0,teq,uc),i1g,i2g,i3g) =
     &           coeff(icf(0,0,0,teq,uc),i1g,i2g,i3g) +
     &           tang(2) * (
     &           aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0) )
            coeff(icf(0,0,0,teq,vc),i1g,i2g,i3g) =
     &           coeff(icf(0,0,0,teq,vc),i1g,i2g,i3g) +
     &           tang(2) * (
     &           aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1) )

            coeff(icf(2*is1,2*is2,2*is3,teq,uc),i1g,i2g,i3g) =
     &           coeff(icf(2*is1,2*is2,2*is3,teq,uc),i1g,i2g,i3g) +
     &           tang(2) * (
     &           -rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0) )
            coeff(icf(2*is1,2*is2,2*is3,teq,vc),i1g,i2g,i3g) =
     &           coeff(icf(2*is1,2*is2,2*is3,teq,vc),i1g,i2g,i3g) +
     &           tang(2) * (
     &           -rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1) )

c     coeff(icf(2*is1,2*is2,2*is3,teq,uc),i1g,i2g,i3g) = 
c     &              -aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0)
c     coeff(icf(2*is1,2*is2,2*is3,teq,vc),i1g,i2g,i3g) = 
c     &              -aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1)

         else

c     &              rtmp*aj(i1g,i2g,i3g)*
            rhs(i1g,i2g,i3g,neq) =
     &           norm(1) * (
     &           u(i1g,i2g,i3g,rc)*aj(i1g,i2g,i3g)*
     &           (rsxy(i1g,i2g,i3g,axis,0)*u(i1g,i2g,i3g,uc) +
     &           rsxy(i1g,i2g,i3g,axis,1)*u(i1g,i2g,i3g,vc))+
     &           aj(i1i,i2i,i3i)*rr*u(i1i,i2i,i3i,rc)*
     &           (rsxy(i1i,i2i,i3i,axis,0)*u(i1i,i2i,i3i,uc) +
     &           rsxy(i1i,i2i,i3i,axis,1)*u(i1i,i2i,i3i,vc)) )

c     rhs(i1g,i2g,i3g,teq) = 0d0
            rhs(i1g,i2g,i3g,teq) =
     &           tang(1) * (
     &           u(i1g,i2g,i3g,rc)*aj(i1g,i2g,i3g)*
     &           (rsxy(i1g,i2g,i3g,axis,0)*u(i1g,i2g,i3g,uc) +
     &           rsxy(i1g,i2g,i3g,axis,1)*u(i1g,i2g,i3g,vc))+
     &           aj(i1i,i2i,i3i)*rr*u(i1i,i2i,i3i,rc)*
     &           (rsxy(i1i,i2i,i3i,axis,0)*u(i1i,i2i,i3i,uc) +
     &           rsxy(i1i,i2i,i3i,axis,1)*u(i1i,i2i,i3i,vc)) )

         endif

         if ( withswirl.eq.1 ) then

            if ( cfrhs.eq.0 ) then
               if ( bc(side,axis).eq.noslipwall) then
               if ( bt(0,side,axis,grid).eq. axisymmetricsbr  .or.
     &              bt(0,side,axis,grid).eq. linearrampinx ) then
c                  dirchilet condition on w
                  coeff(icf(0,0,0,wc,wc),i1,i2,i3) = 1d0
c 070216 always extrapolate the ghost                  coeff(icf(0,0,0,wc,wc),i1g,i2g,i3g) = 1d0
               endif
               endif
c               else
c                  coeff(icf(0,0,0,wc,wc),i1,i2,i3) = 1d0

                  coeff(icf(0,0,0,wc,wc),i1g,i2g,i3g) = 1d0
                  coeff(icf(is1,is2,is3,wc,wc),i1g,i2g,i3g) = -2d0
                  coeff(icf(2*is1,2*is2,2*is3,wc,wc),i1g,i2g,i3g) =+1d0
c               endif
            else
               if ( bc(side,axis).eq.noslipwall) then
               if ( bt(0,side,axis,grid).eq. axisymmetricsbr  ) then
                  rhs(i1,i2,i3,wc) = ubv(0)*x(i1,i2,i3,1)
c                 rhs(i1g,i2g,i3g,wc) = ubv(0)*x(i1g,i2g,i3g,1)
               else if ( bt(0,side,axis,grid).eq. linearrampinx )then
                  rhs(i1,i2,i3,wc) = ubv( 3*wc ) +
     &                 ubv( 3*wc +1 )*x(i1,i2,i3,0)
c                  rhs(i1g,i2g,i3g,wc) = ubv( 3*wc ) + 
c     &                 ubv( 3*wc +1 )*x(i1g,i2g,i3g,0)
               endif
               endif
c                  rhs(i1,i2,i3,wc) = u(i1,i2,i3,wc)
                  rhs(i1g,i2g,i3g,wc) = 0d0
c                  endif
            endif

         end if

c     print *,"rhs: ",i1g,i2g,rhs(i1g,i2g,i3g,rc)

      end do                    !i1
      end do                    !i2
      end do                    !i3

      end if                    ! if noSlipWall

      end do                    ! side
      end do                    ! axis


      return
      end
