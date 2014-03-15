#Include "icnscfMacros.h"

 
      subroutine inoutflowcoeff(nd,nd1a,nd1b,nd2a,nd2b,nd3a,
     &     nd3b,nd4a,nd4b,coeff,rhs,u,x,aj,rsxy,iprm,rprm,
     &     indexRange,bc,bd, bt, nbd, cfrhs)
c
c     mass flux inflow and outflow conditions
c
c
c     070208 kkc : Initial Version
c
c     Notes:

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
      integer indexRange(0:1,0:2), bc(0:1,0:2),nbd
      real bd(0:nbd-1,0:1,0:nd-1,0:*)
      integer bt(0:2,0:1,0:2,0:*)
      integer cfrhs

c     OUTPUT
c     u adjusted at boundaries and ghost points

c     LOCAL
      integer i1l,i2l,i3l,eqn,cc
      integer i1,i2,i3,is1,is2,is3,iss(0:2),c,axis,side,irb(0:1,0:2),a,s
      integer it1,it2,it3,ist(0:2)
      integer i1g,i2g,i3g,i1i,i2i,i3i,e,ncmp,ndim,gmove,neq,teq,ic1,ic2
      integer rc,uc,vc,wc,tc,grid,gridtype,debug,isaxi,radaxis,withswirl
      integer i,j,isten_size,cbnd,width,width3,hwidth,hwidth3
      real rr,aa(0:2,0:2),jdet,jdetg,rtmp,aainv(0:2,0:2),den,xr
      real ubv(0:30),norm(3),tang(3),grav(3),mag
      real rho,uvel,vvel,wvel,temp,mdot,port,pdotn,prhs
      logical useneumanntemp,iscorner
      real ren,prn,man,gam,gm1,gm2, oren, oprn, f43, f13,coeff_u,coeff_v
      real len,hlen,scale,eta,flowsum,totalflow

c     LOCAL PARAMETERS
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
      integer subsonicinflow,subsonicoutflow
      parameter( subsonicinflow=8,subsonicoutflow=10 )
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

      exo1(i1,i2,i3,is1,is2,is3,c) =  u(i1+is1,i2+is2,i3+is3,c)
c     &                                u(i1+2*is1,i2+2*is2,i3+2*is3,c) )
c     2nd order extrapolation of u
      exo2(i1,i2,i3,is1,is2,is3,c) = 2d0*u(i1+is1,i2+is2,i3+is3,c) - 
     &                                  u(i1+2*is1,i2+2*is2,i3+2*is3,c)
c     3rd order extrapolation of u
      exo3(i1,i2,i3,is1,is2,is3,c)=3d0*u(i1+  is1,i2+  is2,i3+  is3,c)-
     &                             3d0*u(i1+2*is1,i2+2*is2,i3+2*is3,c)+
     &                                 u(i1+3*is1,i2+3*is2,i3+3*is3,c)

c     setup the macro variables
      off(1)=0
      off(2)=0
      off(3)=0
      occ=0
      oce=0

c      print *,"inside icns inflow/outflow"
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

      neq = vc
      teq = uc
      
      totalflow = 0d0
      do axis=0,nd-1
      do side=0,1

         flowsum = 0d0

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

c     we get the mass flux per unit area that we want to enforce from
c        rho*uvel and rho*vvel, density and velocity are only specified at the inflow
c        Note that we interpret uvel as the component normal to the boundary and 
c        vvel as the component tangential to it. Positive mass flux means flow is coming 
c        into the domain.
         rho = bd(rc,side,axis,grid)
         uvel= bd(uc,side,axis,grid)
         vvel= bd(vc,side,axis,grid)
         if ( bc(side,axis).eq.subsonicoutflow ) then
         print *,"rho,uvel,vvel : ",rho,uvel,vvel
         endif
         if ( withswirl.gt.0 .or. nd.eq.3 ) wvel= bd(wc,side,axis,grid)
         port= bd(tc,side,axis,grid) ! this is either the pressure or temperature
c         print *,"p vals: ",port,pdotn,prhs
c         print *,"rho : ",side,axis,grid,rho
         len = 0d0
         do i1=irb(0,0),irb(1,0)-it1
         do i2=irb(0,1),irb(1,1)-it2
         do i3=irb(0,2),irb(1,2)-it3
            len = len + sqrt( 
     &           (x(i1+it1,i2+it2,i3+it3,0)-x(i1,i2,i3,0))**2d0 +
     &           (x(i1+it1,i2+it2,i3+it3,1)-x(i1,i2,i3,1))**2d0 )
         end do
         end do
         end do

         rr = 1d0
         hlen = 0d0
         do i1l=irb(0,0),irb(1,0)
         do i2l=irb(0,1),irb(1,1)
         do i3l=irb(0,2),irb(1,2)

            rr = 1d0

            i1g = i1l-is1
            i2g = i2l-is2
            i3g = i3l-is3
            i1  = i1l
            i2  = i2l
            i3  = i3l
            i1i = i1l+is1
            i2i = i2l+is2
            i3i = i3l+is3

            eta = 1d0
            if ( i1l.ne.irb(0,0) .or. i2l.ne.irb(0,1) 
     &           .or. i3l.ne.irb(0,2) ) then
               hlen = hlen + sqrt( 
     &              (x(i1-it1,i2-it2,i3+it3,0)-x(i1,i2,i3,0))**2d0 +
     &              (x(i1-it1,i2-it2,i3+it3,1)-x(i1,i2,i3,1))**2d0 )/len
            endif
            if ( 
     &           (i1l.eq.irb(0,0) .and. i2l.eq.irb(0,1) 
     &           .and. i3l.eq.irb(0,2)) .or.
     &           (i1l.eq.irb(1,0) .and. i2l.eq.irb(1,1) 
     &           .and. i3l.eq.irb(1,2))  ) then
c               eta = .5
            endif

            scale = 1d0
c            scale = (hlen*6d0 - hlen*hlen*6d0)
c            scale = exp(-(5d0*(hlen-.5))**2)*acos(-1d0)/2d0
c           scale = 1d0
            if ( isaxi.gt.0 .and. x(i1,i2,i3,1).gt.0d0 ) then
c               print *,i1,i2,scale
               scale = scale/x(i1,i2,i3,1)
            endif

            a = mod(axis+1,ndim)
            norm(1) = (2d0*a-1d0)*rsxy(i1,i2,i3,a,1)/aj(i1,i2,i3)
            norm(2) = (1d0-2d0*a)*rsxy(i1,i2,i3,a,0)/aj(i1,i2,i3)

            tang(1) = (2d0*axis-1d0)*rsxy(i1,i2,i3,axis,1)
            tang(2) = (1d0-2d0*axis)*rsxy(i1,i2,i3,axis,0)
            mag = sqrt(tang(1)*tang(1)+tang(2)*tang(2))
            tang(1) = tang(1)/mag
            tang(2) = tang(2)/mag
            if ( bc(side,axis).eq.subsonicinflow ) then

            if ( .false. ) then
            scale = 1d0
c            scale = (hlen*6d0 - hlen*hlen*6d0)
c            scale = exp(-(5d0*(hlen-.5))**2)*acos(-1d0)/2d0
c           scale = 1d0
            if ( isaxi.gt.0 .and. x(i1,i2,i3,1).gt.0d0 ) then
c               print *,i1,i2,scale
               scale = scale/x(i1,i2,i3,1)
            endif
            endif
               rr = 1d0
               if (isaxi.ne.0 .and. 
     &              (1d0+abs(x(i1g,i2g,i3g,radaxis))).gt.one .and.
     &              (1d0+abs(x(i1i,i2i,i3i,radaxis))).gt.one ) then
                  
                  rr = x(i1i,i2i,i3i,radaxis)/
     &                 x(i1g,i2g,i3g,radaxis)
                  
               end if

               mag = sqrt(norm(1)*norm(1)+norm(2)*norm(2))
               norm(1) = (1d0-2d0*side)*norm(1)/mag
               norm(2) = (1d0-2d0*side)*norm(2)/mag

c     specify rho, u, v at the boundary
c     extrapolate T, rho, u, v, w at the ghosts
               if ( cfrhs.eq.0 ) then
                  do m3=-hwidth3,hwidth3
                     do m2=-hwidth, hwidth
                        do m1=-hwidth, hwidth
                           do c=0,ncmp-1
                              do e=rc,tc
                                coeff(icf(m1,m2,m3,e,c),i1g,i2g,i3g)=0d0
                              end do
c     do e=uc,tc !max(vc,wc)
                              if ( withswirl.gt. 0 ) then
                                 coeff(icf(m1,m2,m3,wc,c),i1,i2,i3)=0d0
                              endif
                              coeff(icf(m1,m2,m3,rc,c),i1,i2,i3) = 0d0
                              if ( port.gt.0d0 ) then
                                coeff(icf(m1,m2,m3,tc,c),i1,i2,i3) = 0d0
                              endif
                           end do
                        end do
                     end do
                  end do

c           set coefficients

c                specified pressure 
c                 coeff(icf(0,0,0,rc,tc),i1,i2,i3) = u(i1,i2,i3,rc)
c                 coeff(icf(0,0,0,rc,rc),i1,i2,i3) = u(i1,i2,i3,tc)
c          specified density
                  coeff(icf(0,0,0,rc,rc),i1,i2,i3) = 1d0 
c          specified temperature
                  if ( port.gt.0d0 ) then
                     coeff(icf(0,0,0,tc,tc),i1,i2,i3) = 1d0 
                  endif
c      the mass flux boundary condtions are used to derive the ghost point equations
c      F_ghost + 2*F_bdy + F_interior = 4*scale*rho*uvel*len/dr
c      where F is the normal flux
c      the tangential flux is set by
c      G_ghost - G_interior = 0 (symmetry, G is the tangential mass flux)
c      of course we linearize all the above equations
c      note:
c      F = rho*aj*(u*rsxy(axis,0) + v*rsxy(axis,1))
c      G = rho*aj*(u*rsxy(a,0)    + v*rsxy(a,1))

                  if ( .true. ) then
                     do m3=-hwidth3,hwidth3
                        do m2=-hwidth, hwidth
                           do m1=-hwidth, hwidth
                              do c=0,ncmp-1
                             
                            coeff_u = coeff(icf(m1,m2,m3,uc,c),i1,i2,i3)
                            coeff_v = coeff(icf(m1,m2,m3,vc,c),i1,i2,i3)
                            coeff(icf(m1,m2,m3,neq,c),i1,i2,i3) = 0d0
                            coeff(icf(m1,m2,m3,teq,c),i1,i2,i3) =  0d0
c     &                           tang(1)*coeff_u + tang(2)*coeff_v
                              end do
                           end do
                        end do
                     end do
c      F_bdy = rho*uvel
                     eqn = neq
c      UVCF(0,0,0,norm(1),rc,uc,COEFF)
c      UVCF(0,0,0,norm(2),rc,vc,COEFF)
                     coeff(icf(0,0,0,eqn,uc),i1,i2,i3) = norm(1)
                     coeff(icf(0,0,0,eqn,vc),i1,i2,i3) = norm(2)
c      G_bdy = rho*vvel
                     eqn = teq
c      UVCF(0,0,0,tang(1),rc,uc,COEFF)
c      UVCF(0,0,0,tang(2),rc,vc,COEFF)
                     coeff(icf(0,0,0,eqn,uc),i1,i2,i3) = tang(1)
                     coeff(icf(0,0,0,eqn,vc),i1,i2,i3) = tang(2)
                  endif

c               the swirl component can be specified with a dirichlet condition

                  if ( withswirl.gt.0 ) then
                     coeff(icf(0,0,0,wc,wc),i1,i2,i3) = 1d0
                  endif

c               assign macros on the ghost points
c               apply macros shifted to the boundary point
                  i1 = i1g
                  i2 = i2g
                  i3 = i3g
                  rr = 1d0
                  eqn = neq
c                  rr = 2d0*side-1d0
c     ghost point
                  if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
                  UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0),rc,uc,COEFF)
                  UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1),rc,vc,COEFF)
c                  UVCF(0,0,0,rr*norm(1),rc,uc,COEFF)
c                  UVCF(0,0,0,rr*norm(2),rc,vc,COEFF)
                  
                  if ( .false. ) then
c     boundary point
c                     rr = 2d0*side-1d0
                     rr = 1d0
                     if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
c                     UVCF(is1,is2,is3,rr*2d0*norm(1),rc,uc,COEFF)
c                     UVCF(is1,is2,is3,rr*2d0*norm(2),rc,vc,COEFF)
                     UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,0),rc,uc,COEFF)
                     UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,1),rc,vc,COEFF)
                  endif
                
c     interior point
c                  rr = 2d0*side-1d0
                  rr = 1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)
c     rr = -1d0
                  UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0),rc,uc,COEFF)
                  UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1),rc,vc,COEFF)
c                  UVCF(2*is1,2*is2,2*is3,rr*norm(1),rc,uc,COEFF)
c                  UVCF(2*is1,2*is2,2*is3,rr*norm(2),rc,vc,COEFF)
                  
c     G_ghost - G_interior = 0 (symmetry, G is the tangential mass flux)
                  eqn = teq
                  a = mod(axis+1,ndim)
                  UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0),rc,uc,COEFF)
                  UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1),rc,vc,COEFF)
                  UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0),rc,uc,COEFF)
                  UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1),rc,vc,COEFF)
c     UVCF(0,0,0,norm(1),rc,uc,COEFF)
c                UVCF(0,0,0,norm(2),rc,vc,COEFF)
c     UVCF( is1, is2, is3,-norm(1),rc,uc,COEFF)
c     UVCF( is1, is2, is3,-norm(2),rc,vc,COEFF)
                
                
c                  off(1)=is1
c                  off(2)=is2
c                  off(3)=is3
                  do e=tc,tc
                     eqn = e
c                     UXC(1,norm(1),e)
c                     UXC(2,norm(2),e)
                     coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
        coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g) = 1
c                     coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g) = -1
                  enddo
                  do e=rc,rc
                     eqn = e
c                     UXC(1,norm(1),e)
c                     UXC(2,norm(2),e)
                  off(1)=0
                  off(2)=0
                  off(3)=0
                     coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
      coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
      coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g) = 1
c                     coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g) = -1
                  end do

c     reset macro application
                  i1 = i1l
                  i2 = i2l
                  i3 = i3l
                  off(1)=0
                  off(2)=0
                  off(3)=0

                  if ( withswirl.gt.0 ) then 
                     e = wc
                     coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
                     coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
                     coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g) = 1
                  endif                   
                  
               else

c     set rhs
c        rhs(i1g,i2g,i3g,rc)=port*rho+u(i1g,i2g,i3g,tc)*u(i1g,i2g,i3g,rc)
c                rhs(i1,i2,i3,rc)=port*rho+u(i1,i2,i3,tc)*u(i1,i2,i3,rc)
                  rhs(i1,i2,i3,rc)=rho
                  if ( port.gt.0d0 ) then
                     rhs(i1,i2,i3,tc)=port
                  endif
                     
c                  rhs(i1,i2,i3,teq) = tang(1)*rhs(i1,i2,i3,uc)+
c     &                                tang(2)*rhs(i1,i2,i3,vc)
c                  rhs(i1,i2,i3,neq) = rho*uvel+
c     &                 u(i1,i2,i3,rc)*(u(i1,i2,i3,uc)*norm(1) +
c     &                                 u(i1,i2,i3,vc)*norm(2))
                  if ( withswirl.gt.0 ) then 
                     rhs(i1,i2,i3,wc) = wvel
                     rhs(i1g,i2g,i3g,wc) = 0d0
                  endif

                  if ( .true. ) then
c     G_bdy = rho*vvel
c                   rhs(i1,i2,i3,teq) = rhs(i1,i2,i3,uc)*tang(1)+
c     &                  rhs(i1,i2,i3,vc)*tang(2)
c      F_bdy = rho*uvel
                     eqn = neq
c                    rhs(i1,i2,i3,eqn) = scale*rho*uvel
c     print *,i1,i2,scale*rho*uvel
                     rhs(i1,i2,i3,eqn) = scale*uvel
c                   UVCF(0,0,0,norm(1),rc,uc,RHS)
c                   UVCF(0,0,0,norm(2),rc,vc,RHS)
                   
c      G_bdy = rho*vvel
                     eqn = teq
c                    rhs(i1,i2,i3,eqn) = scale*rho*vvel
                     rhs(i1,i2,i3,eqn) = scale*vvel
c                   UVCF(0,0,0,tang(1),rc,uc,RHS)
c                   UVCF(0,0,0,tang(2),rc,vc,RHS)
                  endif

c               apply macros on the ghost points
                  i1 = i1g
                  i2 = i2g
                  i3 = i3g
                  
                  rr = 1d0
                  eqn = neq
                  rhs(i1g,i2g,i3g,eqn) = 0d0
c     ghost point
c                rr = 2d0*side-1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
                UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0),rc,uc,RHS)
                UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1),rc,vc,RHS)
c                  UVCF(0,0,0,rr*norm(1),rc,uc,RHS)
c                  UVCF(0,0,0,rr*norm(2),rc,vc,RHS)

                  rr = 1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
                rhs(i1g,i2g,i3g,eqn) = rhs(i1g,i2g,i3g,eqn)+
     &               2d0*rr*scale*rho*uvel*len*dri(mod(axis+1,ndim)+1)/
     &               (irb(1,mod(axis+1,ndim))-irb(0,mod(axis+1,ndim)))

c                  rhs(i1g,i2g,i3g,eqn) = rhs(i1g,i2g,i3g,eqn)
c     &                 +2d0*rr*scale*rho*uvel

c                print *,
c     &                   4d0*scale*rho*uvel*len*dri(mod(axis+1,ndim)+1)/
c     &               (irb(1,mod(axis+1,ndim))-irb(0,mod(axis+1,ndim)))
c                print *,len
c                print *,dri(mod(axis+1,ndim)+1)/
c     &               (irb(1,mod(axis+1,ndim))-irb(0,mod(axis+1,ndim)))
c                print *,scale*rho*uvel*len*dri(mod(axis+1,ndim)+1)/
c     &               (irb(1,mod(axis+1,ndim))-irb(0,mod(axis+1,ndim)))
                  if ( .false. ) then
c               boundary point
c                   rr = 2d0*side-1d0
                     rr = 1d0
                     if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
                     UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,0),rc,uc,RHS)
                     UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,1),rc,vc,RHS)
c                     UVCF(is1,is2,is3,rr*2d0*norm(1),rc,uc,RHS)
c                     UVCF(is1,is2,is3,rr*2d0*norm(2),rc,vc,RHS)

                  endif
                
c               interior point
c                rr = 2d0*side-1d0
c                rr = -1d0
                  rr = 1d0
                  if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)
                  UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0),rc,uc,RHS)
                  UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1),rc,vc,RHS)
c                  UVCF(2*is1,2*is2,2*is3,rr*norm(1),rc,uc,RHS)
c                  UVCF(2*is1,2*is2,2*is3,rr*norm(2),rc,vc,RHS)

c      G_ghost - G_interior = 0 (symmetry, G is the tangential mass flux)
                  eqn = teq
                  a = mod(axis+1,ndim)
                  rhs(i1g,i2g,i3g,eqn) = 0d0
                  UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0),rc,uc,RHS)
                  UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1),rc,vc,RHS)
                  UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0),rc,uc,RHS)
                  UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1),rc,vc,RHS)
c                UVCF(0,0,0,norm(1),rc,uc,RHS)
c                UVCF(0,0,0,norm(2),rc,vc,RHS)
c                UVCF( is1, is2, is3,-norm(1),rc,uc,RHS)
c                UVCF( is1, is2, is3,-norm(2),rc,vc,RHS)
c               reset macro application
                  i1 = i1l
                  i2 = i2l
                  i3 = i3l

                
                  rhs(i1g,i2g,i3g,rc) = 0d0
                  rhs(i1g,i2g,i3g,tc) = 0d0
                  if ( withswirl.gt. 0 ) then
                     rhs(i1g,i2g,i3g,wc) = 0d0
                  endif
               endif            !coeff or rhs
               rr = 1d0

            else if ( bc(side,axis).eq.subsonicoutflow ) then

            if ( .false. ) then
            scale = 1d0
c            scale = (hlen*6d0 - hlen*hlen*6d0)
c            scale = exp(-(5d0*(hlen-.5))**2)*acos(-1d0)/2d0
c           scale = 1d0
            if ( isaxi.gt.0 .and. x(i1,i2,i3,1).gt.0d0 ) then
c               print *,i1,i2,scale
               scale = scale/x(i1,i2,i3,1)
            endif
            endif
c               print *,"outflow ",i1,rho,uvel,scale

c               print *,side,axis,norm(1),norm(2)
c               print *,tang(1),tang(2)
c               print *, "outflow bd : ",rho,uvel,vvel,port,pdotn
c     specify rho * u and rho * v at the boundary
c     let the continuity, w and T equations stay the same on the boundary
c     extrapolate T, rho, u, v, w at the ghosts
               mag = sqrt(norm(1)*norm(1)+norm(2)*norm(2))

               norm(1) = (2d0*side-1d0)*norm(1)/mag
               norm(2) = (2d0*side-1d0)*norm(2)/mag

c      print *,"o : ",i1,i2,norm(1),norm(2)

               if ( cfrhs.eq.0 ) then
c     set coefficients
                  
                  if ( port.lt.1e-10 ) then
c     mass flux boundary condition
                     do m3=-hwidth3,hwidth3
                        do m2=-hwidth, hwidth
                           do m1=-hwidth, hwidth
                              do c=0,ncmp-1
                                 do e=rc,tc
                                coeff(icf(m1,m2,m3,e,c),i1g,i2g,i3g)=0d0
                                 end do
c                         do e=uc,vc
c                            coeff(icf(m1,m2,m3,e,c),i1,i2,i3)=0d0
c                         end do
                              end do
                           end do
                        end do
                     end do


                     do m3=-hwidth3,hwidth3
                        do m2=-hwidth, hwidth
                           do m1=-hwidth, hwidth
                              do c=0,ncmp-1
                              
                              coeff_u=coeff(icf(m1,m2,m3,uc,c),i1,i2,i3)
                              coeff_v=coeff(icf(m1,m2,m3,vc,c),i1,i2,i3)
                            
                               coeff(icf(m1,m2,m3,neq,c),i1,i2,i3) = 0d0
                               coeff(icf(m1,m2,m3,teq,c),i1,i2,i3) = 0d0
c     &                                tang(1)*coeff_u + tang(2)*coeff_v
c     if ( i2.eq.0 ) then
c                     print *,i1,i2,c,tang(1)*coeff_u,tang(2)*coeff_v
c                  endif
                              end do
                           end do
                        end do
                     end do
                  
                     coeff(icf(0,0,0,neq,uc),i1,i2,i3) = 
     &                    norm(1)*u(i1,i2,i3,rc)
                     coeff(icf(0,0,0,neq,vc),i1,i2,i3) = 
     &                    norm(2)*u(i1,i2,i3,rc)
                     coeff(icf(0,0,0,neq,rc),i1,i2,i3) = 
     &                    norm(1)*u(i1,i2,i3,uc)+norm(2)*u(i1,i2,i3,vc)
              


                     coeff(icf(0,0,0,teq,uc),i1,i2,i3) = 
     &                    tang(1)*u(i1,i2,i3,rc)
                     coeff(icf(0,0,0,teq,vc),i1,i2,i3) = 
     &                    tang(2)*u(i1,i2,i3,rc)
                     coeff(icf(0,0,0,teq,rc),i1,i2,i3) = 
     &                    tang(1)*u(i1,i2,i3,uc)+tang(2)*u(i1,i2,i3,vc)
              


                     do e=tc,tc
                        coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
                        if ( port.gt.0d0 ) then
                        coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=1
                        else
                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=-1
                        endif
                     end do
                  
                     do e=rc,rc
                        coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
c     coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
c     coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g) = 1
                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=-1
                     end do

                     if ( withswirl.gt. 0 ) then
                        e = wc
                        coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
c     coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
c     coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g) = 1
                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=-1
                     endif

                     i1 = i1g
                     i2 = i2g
                     i3 = i3g
                     rr = 1d0
                     eqn = neq
                     rr = 2d0*side-1d0
c     ghost point
                     if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
                     UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0),rc,uc,COEFF)
                     UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1),rc,vc,COEFF)
c                     UVCF(0,0,0,rr*norm(1),rc,uc,COEFF)
c                     UVCF(0,0,0,rr*norm(2),rc,vc,COEFF)

                     if ( .false. ) then
c     boundary point
c                rr = 2d0*side-1d0
                        rr = 1d0
                        if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
c                        UVCF(is1,is2,is3,rr*2d0*norm(1),rc,uc,COEFF)
c                        UVCF(is1,is2,is3,rr*2d0*norm(2),rc,vc,COEFF)
                        UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,0),rc,uc,COEFF)
                        UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,1),rc,vc,COEFF)
                     endif

c     interior point
                     rr = 1d0
                     rr = 2d0*side-1d0
                     if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)
c     rr = -1d0
                     UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0),rc,uc,COEFF)
                     UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1),rc,vc,COEFF)
c                     UVCF(2*is1,2*is2,2*is3,rr*norm(1),rc,uc,COEFF)
c                     UVCF(2*is1,2*is2,2*is3,rr*norm(2),rc,vc,COEFF)
                  
c      G_ghost - G_interior = 0 (symmetry, G is the tangential mass flux)
                     eqn = teq
                     a = mod(axis+1,ndim)
                     UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0),rc,uc,COEFF)
                     UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1),rc,vc,COEFF)
                     UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0),rc,uc,COEFF)
                     UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1),rc,vc,COEFF)

c               reset macro application
                     i1 = i1l
                     i2 = i2l
                     i3 = i3l

                  else
c     mixed derivative on the pressure
                     port = bd(tc+ncmp,side,axis,grid)
                     pdotn= bd(tc+ncmp*2,side,axis,grid)
                     prhs= bd(tc,side,axis,grid)
c                     print *,port,pdotn,prhs
                     do m3=-hwidth3,hwidth3
                        do m2=-hwidth, hwidth
                           do m1=-hwidth, hwidth
                              do c=0,ncmp-1
                                 do e=rc,tc
                                coeff(icf(m1,m2,m3,e,c),i1g,i2g,i3g)=0d0
                                 end do
                                 do e=rc,rc
                                   coeff(icf(m1,m2,m3,e,c),i1,i2,i3)=0d0
                                 end do
                              end do
                           end do
                        end do
                     end do

                     do m3=-hwidth3,hwidth3
                        do m2=-hwidth, hwidth
                           do m1=-hwidth, hwidth
                              do c=0,ncmp-1
                              
                              coeff_u=coeff(icf(m1,m2,m3,uc,c),i1,i2,i3)
                              coeff_v=coeff(icf(m1,m2,m3,vc,c),i1,i2,i3)
                            
                               coeff(icf(m1,m2,m3,neq,c),i1,i2,i3) = !0d0
     &                                norm(1)*coeff_u + norm(2)*coeff_v
                               coeff(icf(m1,m2,m3,teq,c),i1,i2,i3) = 0d0
c     if ( i2.eq.0 ) then
c                     print *,i1,i2,c,tang(1)*coeff_u,tang(2)*coeff_v
c                  endif
                              end do
                           end do
                        end do
                     end do

                     coeff(icf(0,0,0,teq,uc),i1,i2,i3) = 
     &                    tang(1)
                     coeff(icf(0,0,0,teq,vc),i1,i2,i3) = 
     &                    tang(2)

                     eqn = rc
                  
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3)= port*u(i1,i2,i3,tc)
                  coeff(icf(0,0,0,eqn,tc),i1,i2,i3)= port*u(i1,i2,i3,rc)
                  
                  UXC(1,pdotn*u(i1l,i2l,i3l,tc)*norm(1),rc)
                  UXC(2,pdotn*u(i1l,i2l,i3l,tc)*norm(2),rc)
                  coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,tc),i1,i2,i3) +
     &                    pdotn*norm(1)*ux(i1l,i2l,i3l,rc,1) +
     &                    pdotn*norm(2)*ux(i1l,i2l,i3l,rc,2) 
                     UXC(1,pdotn*u(i1l,i2l,i3l,rc)*norm(1),tc)
                     UXC(2,pdotn*u(i1l,i2l,i3l,rc)*norm(2),tc)
                     coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                    pdotn*norm(1)*ux(i1l,i2l,i3l,tc,1) +
     &                    pdotn*norm(2)*ux(i1l,i2l,i3l,tc,2) 
                  
c           extrapolate the ghost points
                     do e=rc,rc
                        coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
                        coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=1
c                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=-1
                     end do
                     do e=tc,tc
                        coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
                        coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=1
c                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=-1
                     end do
                     do e=uc,max(vc,wc)
                        coeff(icf(0,0,0,e,e),i1g,i2g,i3g) = 1d0
c                        coeff(icf(is1,is2,is3,e,e),i1g,i2g,i3g) = -2d0
c                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=1
                        coeff(icf(2*is1,2*is2,2*is3,e,e),i1g,i2g,i3g)=-1
                     end do
                  
                  endif         ! mass flux or pressure bc
                 
               else
c     set rhs of outflow
                  if ( port.lt.1e-10 ) then
c     specified mass flux
c                     rhs(i1,i2,i3,teq) = 
c     &                 tang(1)*rhs(i1,i2,i3,uc)+tang(2)*rhs(i1,i2,i3,vc)
                     rhs(i1,i2,i3,teq) =scale*rho*vvel+ 
     &                    u(i1,i2,i3,rc)*( tang(1)*u(i1,i2,i3,uc) +
     &                    tang(2)*u(i1,i2,i3,vc) )

                     rhs(i1,i2,i3,neq) =scale*rho*uvel+ 
     &                    u(i1,i2,i3,rc)*( norm(1)*u(i1,i2,i3,uc) +
     &                    norm(2)*u(i1,i2,i3,vc) )

c     apply macros on the ghost points
                     i1 = i1g
                     i2 = i2g
                     i3 = i3g
                     
                     rr = 1d0
                     eqn = neq
                     rhs(i1g,i2g,i3g,eqn) = 0d0
c     ghost point
                     rr = 2d0*side-1d0
                     if ( isaxi.gt.0 ) rr = rr*x(i1g,i2g,i3g,radaxis)
                     UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,0),rc,uc,RHS)
                     UVCF(0,0,0,rr*aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,axis,1),rc,vc,RHS)
c                     UVCF(0,0,0,rr*norm(1),rc,uc,RHS)
c                     UVCF(0,0,0,rr*norm(2),rc,vc,RHS)
                    
                     rr = 1d0
c                     rr = 1d0-2d0*side
                     if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
                     rhs(i1g,i2g,i3g,eqn) = rhs(i1g,i2g,i3g,eqn)+
     &               2d0*scale*rr*rho*uvel*len*dri(mod(axis+1,ndim)+1)/
     &               (irb(1,mod(axis+1,ndim))-irb(0,mod(axis+1,ndim)))
                    
c                     rhs(i1g,i2g,i3g,eqn) = rhs(i1g,i2g,i3g,eqn)
c     &                    +2d0*rr*scale*rho*uvel

                     if ( .false. ) then
c     boundary point
c     rr = 2d0*side-1d0
                        rr = 1d0
                        if ( isaxi.gt.0 ) rr = rr*x(i1l,i2l,i3l,radaxis)
                        UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,0),rc,uc,RHS)
                        UVCF(is1,is2,is3,rr*2d0*aj(i1l,i2l,i3l)*rsxy(i1l,i2l,i3l,axis,1),rc,vc,RHS)
c                        UVCF(is1,is2,is3,rr*2d0*norm(1),rc,uc,RHS)
c                        UVCF(is1,is2,is3,rr*2d0*norm(2),rc,vc,RHS)

                     endif
                    
c     interior point
                     rr = 2d0*side-1d0
c     rr = -1d0
c                     rr = 1d0
                     if ( isaxi.gt.0 ) rr = rr*x(i1i,i2i,i3i,radaxis)
                     UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,0),rc,uc,RHS)
                     UVCF(2*is1,2*is2,2*is3,rr*aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,axis,1),rc,vc,RHS)
c                     UVCF(2*is1,2*is2,2*is3,rr*norm(1),rc,uc,RHS)
c                     UVCF(2*is1,2*is2,2*is3,rr*norm(2),rc,vc,RHS)
                    
c      G_ghost - G_interior = 0 (symmetry, G is the tangential mass flux)
                     eqn = teq
                     a = mod(axis+1,ndim)
                     rhs(i1g,i2g,i3g,eqn) = 0d0
                     UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,0),rc,uc,RHS)
                     UVCF(0,0,0,aj(i1g,i2g,i3g)*rsxy(i1g,i2g,i3g,a,1),rc,vc,RHS)
                     UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,0),rc,uc,RHS)
                     UVCF( 2*is1, 2*is2, 2*is3,-aj(i1i,i2i,i3i)*rsxy(i1i,i2i,i3i,a,1),rc,vc,RHS)
c                UVCF(0,0,0,norm(1),rc,uc,RHS)
c     UVCF(0,0,0,norm(2),rc,vc,RHS)
c     UVCF( is1, is2, is3,-norm(1),rc,uc,RHS)
c     UVCF( is1, is2, is3,-norm(2),rc,vc,RHS)
c               reset macro application
                     i1 = i1l
                     i2 = i2l
                     i3 = i3l
                    
c                    do e=rc,tc
c     do e=uc,tc
                     rhs(i1g,i2g,i3g,rc) = 0d0
                     rhs(i1g,i2g,i3g,tc) = 0d0
                     if ( withswirl.gt. 0 ) then
                        rhs(i1g,i2g,i3g,wc) = 0d0
                     endif

c                    enddo
c                    print *,i1,i2,rhs(i1,i2,i3,rc)
                  else
c     mixed condition on the pressure
                     rhs(i1,i2,i3,neq) = 
     &                 norm(1)*rhs(i1,i2,i3,uc)+norm(2)*rhs(i1,i2,i3,vc)
                     rhs(i1,i2,i3,teq) = 0d0

                     port = bd(tc+ncmp,side,axis,grid)
                     pdotn= bd(tc+ncmp*2,side,axis,grid)
                     prhs= bd(tc,side,axis,grid)
                     rhs(i1,i2,i3,rc) = 
     &                    port*u(i1,i2,i3,rc)*u(i1,i2,i3,tc)+prhs+
     &          u(i1l,i2l,i3l,tc)*(pdotn*norm(1)*ux(i1l,i2l,i3l,rc,1)+
     &                    pdotn*norm(2)*ux(i1l,i2l,i3l,rc,2))+
     &          u(i1l,i2l,i3l,rc)*(pdotn*norm(1)*ux(i1l,i2l,i3l,tc,1)+
     &                    pdotn*norm(2)*ux(i1l,i2l,i3l,tc,2))
                     do e=rc,tc
c     do e=uc,tc
                        rhs(i1g,i2g,i3g,e) = 0d0
                     enddo
                  endif

               endif            ! if build coeff or rhs

               rr = -1d0
               
            endif               ! if inflow or outflow

            xr = rr/dri(mod(axis+1,ndim)+1)/  
     &           rx(i1,i2,i3,mod(axis+1,ndim)+1,mod(axis+1,ndim)+1)
            if ( isaxi.gt.0 ) rr = xr*x(i1,i2,i3,1)
            if ( 
     &           (i1l.eq.irb(0,0) .and. i2l.eq.irb(0,1) 
     &           .and. i3l.eq.irb(0,2)) .or.
     &           (i1l.eq.irb(1,0) .and. i2l.eq.irb(1,1) 
     &           .and. i3l.eq.irb(1,2))  ) then
               rr = .5*rr
            endif
            if ( bc(side,axis).eq.subsonicoutflow .or. 
     &           bc(side,axis).eq.subsonicinflow ) then
c               print *,i1,i2,u(i1,i2,i3,rc)*(norm(1)*u(i1,i2,i3,uc)+
c     &                                          norm(2)*u(i1,i2,i3,vc))
            flowsum =flowsum+(rr*u(i1,i2,i3,rc)*(norm(1)*u(i1,i2,i3,uc)+
     &                                      norm(2)*u(i1,i2,i3,vc))*2d0)

            if ( isaxi .gt. 0) then
               rr = xr*x(i1g,i2g,i3g,1)
            endif
            flowsum = flowsum + 
     &           rr*u(i1g,i2g,i3g,rc)*(norm(1)*u(i1g,i2g,i3g,uc)+
     &                                 norm(2)*u(i1g,i2g,i3g,vc))
            if ( isaxi .gt. 0) then
               rr = xr*x(i1i,i2i,i3i,1)
            endif
            flowsum = flowsum + 
     &            rr*u(i1i,i2i,i3i,rc)*(norm(1)*u(i1i,i2i,i3i,uc)+
     &                               norm(2)*u(i1i,i2i,i3i,vc))
c            print *,i1,i2,flowsum
c            if ( .true. ) then !flowsum .ne. flowsum ) then
c               print *,norm(1),norm(2),rr
c           print *,u(i1g,i2g,i3g,rc),u(i1i,i2i,i3i,rc),u(i1l,i2l,i3l,rc)
c           print *,u(i1g,i2g,i3g,uc),u(i1i,i2i,i3i,uc),u(i1l,i2l,i3l,uc)
c           print *,u(i1g,i2g,i3g,vc),u(i1i,i2i,i3i,vc),u(i1l,i2l,i3l,vc)
c            endif
            endif

         end do
         end do
         end do

c         if ( bc(side,axis).eq.subsonicoutflow .or. 
c     &        bc(side,axis).eq.subsonicinflow ) then
            print *,"flowsum ",grid,side,axis," = ",flowsum/4d0
            totalflow = totalflow + flowsum/4d0
c         endif
      end do ! side
      end do ! axis

      print *,"totalflow ",grid," = ",totalflow
      end


      subroutine inoutflowexp(nd,nd1a,nd1b,nd2a,nd2b,nd3a,
     &     nd3b,nd4a,nd4b,u,x,aj,rsxy,iprm,rprm,
     &     indexRange,bc,bd, bt, nbd)
c
c     mass flux inflow and outflow conditions
c
c
c     070208 kkc : Initial Version
c
c     !!!! THIS IS A HACK TO INITIALIZE THE NEWTON SOLVER/STEADY STATE CODE
c     Notes:

      implicit none
      
c     INPUT
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,iprm(0:*)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b) ! state to linearize about 
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1) ! grid vertices
      real aj(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b) ! determinant of grid Jacobian matrix
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1) ! metric derivatives
      real rprm(*) 
      integer indexRange(0:1,0:2), bc(0:1,0:2),nbd
      real bd(0:nbd-1,0:1,0:nd-1,0:*)
      integer bt(0:1,0:1,0:2,0:*)

c     OUTPUT
c     u adjusted at boundaries and ghost points

c     LOCAL
      integer i1l,i2l,i3l,eqn,cc
      integer i1,i2,i3,is1,is2,is3,iss(0:2),c,axis,side,irb(0:1,0:2),a,s
      integer it1,it2,it3,ist(0:2)
      integer i1g,i2g,i3g,i1i,i2i,i3i,e,ncmp,ndim,gmove,neq,teq,ic1,ic2
      integer rc,uc,vc,wc,tc,grid,gridtype,debug,isaxi,radaxis,withswirl
      integer i,j,isten_size,cbnd,width,width3,hwidth,hwidth3
      real rr,aa(0:2,0:2),jdet,jdetg,rtmp,aainv(0:2,0:2),den,xr
      real ubv(0:30),norm(3),tang(3),grav(3),mag
      real rho,uvel,vvel,wvel,temp,mdot,port,pdotn,prhs
      logical useneumanntemp,iscorner
      real ren,prn,man,gam,gm1,gm2, oren, oprn, f43, f13,coeff_u,coeff_v
      real len,hlen,scale,eta,flowsum,totalflow

c     LOCAL PARAMETERS
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
      integer subsonicinflow,subsonicoutflow
      parameter( subsonicinflow=8,subsonicoutflow=10 )
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

      exo1(i1,i2,i3,is1,is2,is3,c) =  u(i1+is1,i2+is2,i3+is3,c)
c     &                                u(i1+2*is1,i2+2*is2,i3+2*is3,c) )
c     2nd order extrapolation of u
      exo2(i1,i2,i3,is1,is2,is3,c) = 2d0*u(i1+is1,i2+is2,i3+is3,c) - 
     &                                  u(i1+2*is1,i2+2*is2,i3+2*is3,c)
c     3rd order extrapolation of u
      exo3(i1,i2,i3,is1,is2,is3,c)=3d0*u(i1+  is1,i2+  is2,i3+  is3,c)-
     &                             3d0*u(i1+2*is1,i2+2*is2,i3+2*is3,c)+
     &                                 u(i1+3*is1,i2+3*is2,i3+3*is3,c)

c     setup the macro variables
      off(1)=0
      off(2)=0
      off(3)=0
      occ=0
      oce=0

c      print *,"inside icns inflow/outflow"
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

      teq = uc
      neq = vc

      totalflow = 0d0
      do axis=0,nd-1
      do side=0,1

         flowsum = 0d0

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

c     we get the mass flux per unit area that we want to enforce from
c        rho*uvel and rho*vvel, density and velocity are only specified at the inflow
c        Note that we interpret uvel as the component normal to the boundary and 
c        vvel as the component tangential to it. Positive mass flux means flow is coming 
c        into the domain.
         rho = bd(rc,side,axis,grid)
         uvel= bd(uc,side,axis,grid)
         vvel= bd(vc,side,axis,grid)
         if ( withswirl.gt.0 .or. nd.eq.3 ) wvel= bd(wc,side,axis,grid)
         port= bd(tc,side,axis,grid) ! this is either the pressure or temperature
c         print *,"p vals: ",port,pdotn,prhs
c         print *,"rho : ",side,axis,grid,rho
         len = 0d0
         do i1=irb(0,0),irb(1,0)-it1
         do i2=irb(0,1),irb(1,1)-it2
         do i3=irb(0,2),irb(1,2)-it3
            len = len + sqrt( 
     &           (x(i1+it1,i2+it2,i3+it3,0)-x(i1,i2,i3,0))**2d0 +
     &           (x(i1+it1,i2+it2,i3+it3,1)-x(i1,i2,i3,1))**2d0 )
         end do
         end do
         end do

         rr = 1d0
         hlen = 0d0
         do i1l=irb(0,0),irb(1,0)
         do i2l=irb(0,1),irb(1,1)
         do i3l=irb(0,2),irb(1,2)

            rr = 1d0

            i1g = i1l-is1
            i2g = i2l-is2
            i3g = i3l-is3
            i1  = i1l
            i2  = i2l
            i3  = i3l
            i1i = i1l+is1
            i2i = i2l+is2
            i3i = i3l+is3

            if ( i1l.ne.irb(0,0) .or. i2l.ne.irb(0,1) 
     &           .or. i3l.ne.irb(0,2) ) then
               hlen = hlen + sqrt( 
     &              (x(i1-it1,i2-it2,i3+it3,0)-x(i1,i2,i3,0))**2d0 +
     &              (x(i1-it1,i2-it2,i3+it3,1)-x(i1,i2,i3,1))**2d0 )/len
            endif
            scale = 1d0
c            scale = (hlen*6d0 - hlen*hlen*6d0)
c            scale = exp(-(5d0*(hlen-.5))**2)*acos(-1d0)/2d0
            eta = hlen*4d0 - hlen*hlen*4d0
c           scale = 1d0
c            if ( isaxi.gt.0 .and. x(i1,i2,i3,1).gt.0d0 ) then
c               print *,i1,i2,scale
c               scale = scale/x(i1,i2,i3,1)
c            endif

c            eta = exp(-(10d0*(hlen-.5))**2)*acos(-1d0)/2d0
c            scale = eta
            eta = 1d0
c            eta = 0d0
c            scale = 1d0
c            print *,i1,hlen,eta
            a = mod(axis+1,ndim)
            norm(1) = (2d0*a-1d0)*rsxy(i1,i2,i3,a,1)
            norm(2) = (1d0-2d0*a)*rsxy(i1,i2,i3,a,0)
            tang(1) = (2d0*axis-1d0)*rsxy(i1,i2,i3,axis,1)
            tang(2) = (1d0-2d0*axis)*rsxy(i1,i2,i3,axis,0)

            norm(1) = norm(1)/sqrt(norm(1)*norm(1)+norm(2)*norm(2))
            norm(2) = norm(2)/sqrt(norm(1)*norm(1)+norm(2)*norm(2))
            tang(1) = tang(1)/sqrt(tang(1)*tang(1)+tang(2)*tang(2))
            tang(2) = tang(2)/sqrt(tang(1)*tang(1)+tang(2)*tang(2))

c           outward pointing normal
            norm(1) = (2d0*side-1d0)*norm(1)
            norm(2) = (2d0*side-1d0)*norm(2)

            if ( bc(side,axis).eq.subsonicinflow ) then

c               print *,"inflow ",i1,rho,uvel,port
               mag = sqrt(norm(1)*norm(1)+norm(2)*norm(2))
*             print *,"i : ",i1,i2,norm(1),norm(2),side

c     specify rho, u, v at the boundary
c     extrapolate T, rho, u, v, w at the ghosts

            mag = u(i1,i2,i3,rc)*(norm(1)*tang(2)-norm(2)*tang(1))
c            u(i1,i2,i3,uc) = rho
            u(i1,i2,i3,uc) = -scale*norm(1)*uvel + tang(1)*vvel
            u(i1,i2,i3,vc) = -scale*norm(2)*uvel + tang(2)*vvel!rho*(-tang(1)*uvel+norm(1)*vvel)/mag
c            u(i1,i2,i3,tc) = port
c            u(i1g,i2g,i3g,rc) = exo1(i1g,i2g,i3g,is1,is2,is3,rc)
c            u(i1g,i2g,i3g,uc) = exo2(i1g,i2g,i3g,is1,is2,is3,uc)
c            u(i1g,i2g,i3g,vc) = exo2(i1g,i2g,i3g,is1,is2,is3,vc)
c            u(i1g,i2g,i3g,tc) = exo1(i1g,i2g,i3g,is1,is2,is3,tc)

c            if ( withswirl.gt.0 ) then
c               u(i1,i2,i3,wc) = wvel
c               u(i1g,i2g,i3g,wc) = exo1(i1g,i2g,i3g,is1,is2,is3,wc)
c            endif

            else if(bc(side,axis).eq.subsonicoutflow .and. .false. )then
c               print *,"outflow ",i1,rho,uvel,scale

               rr = -1d0
c               print *,side,axis,norm(1),norm(2)
c               print *,tang(1),tang(2)
c               print *, "outflow bd : ",rho,uvel,vvel,port,pdotn
c     specify rho * u and rho * v at the boundary
c     let the continuity, w and T equations stay the same on the boundary
c     extrapolate T, rho, u, v, w at the ghosts
               mag = sqrt(norm(1)*norm(1)+norm(2)*norm(2))

               norm(1) = (2d0*side-1d0)*norm(1)/mag
               norm(2) = (2d0*side-1d0)*norm(2)/mag
               
c     print *,"o : ",i1,i2,norm(1),norm(2)
               
               if ( port.lt.1e-10 ) then
                  mag = norm(1)*tang(2)-norm(2)*tang(1)
                u(i1,i2,i3,uc)= scale*rho*(tang(2)*uvel - norm(2)*vvel)/
     .                                    mag/u(i1,i2,i3,rc)
                u(i1,i2,i3,vc)=scale*rho*(-tang(1)*uvel + norm(1)*vvel)/
     .                                    mag/u(i1,i2,i3,rc)
               else
                  port = bd(tc+ncmp,side,axis,grid)
                  pdotn= bd(tc+ncmp*2,side,axis,grid)
                  prhs= bd(tc,side,axis,grid)
                  u(i1,i2,i3,rc) = port/u(i1,i2,i3,tc)
               endif
               
c     print *,"outflow rhs : ",rhs(i1,i2,i3,neq),rhs(i1,i2,i3,teq)
            if ( withswirl.gt.0 ) then
               u(i1,i2,i3,wc) = wvel
               u(i1g,i2g,i3g,wc) = exo1(i1g,i2g,i3g,is1,is2,is3,wc)
            endif

c               do e=rc,tc
c                  u(i1g,i2g,i3g,e) = exo1(i1g,i2g,i3g,is1,is2,is3,e)
c               enddo
               
         endif ! inflow or outflow


         end do
         end do
         end do

      end do ! side
      end do ! axis

c      print *,"totalflow ",grid," = ",totalflow
      end
