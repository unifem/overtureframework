! This file automatically generated from addViscousTerms.bf with bpp.
c===================================================
c This macro will add on the viscous terms
c   GRIDTYPE: curvilinear or rectangular 
c===================================================


      subroutine addViscousTerms(nd,md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,
     & n2a,n2b,n1bm,n2bm,nda,ndb, dr,ds,rx,gv,det,rx2,gv2,det2,xy,u,
     & up,mask,icart,  w,ndw )
c==================================================================================
c Add the viscous terms for the compressible Navier-Stokes equations
c
c               up = up + viscous terms
c
c u (input) : current solution
c up (input/output) : du/dt on input, du/dt plus viscous terms on output
c w (input) : work-space of size (nda:ndb,4) where nda=min(nd1a,nd2a) and ndb=max(nd1b,nd2b)
c==================================================================================

      implicit none
      integer nd,md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,n1bm,n2bm,
     & nda,ndb,icart,ndw
      real dr,ds
      real rx(nd1a:n1bm,nd2a:n2bm,2,2),gv(nd1a:nd1b,nd2a:nd2b,2),
     &    det(nd1a:n1bm,nd2a:n2bm),rx2(nd1a:n1bm,nd2a:n2bm,2,2),
     &    gv2(nd1a:nd1b,nd2a:nd2b,2),det2(nd1a:n1bm,nd2a:n2bm),
     &    xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,md),
     &    up(nd1a:nd1b,nd2a:nd2b,md)
      ! work space
      real w(nda:ndb,4)

      integer mask(nd1a:nd1b,nd2a:nd2b)

c --------- local variables
      integer i1,i2,i3,i,j,k,ir,m1,m2,ie
      real aja,tmpr,tmps,amu0,amu23,akappa0,tau(2,2),q(2)
      real hdi(2),hdi2(2),hdi4(2)
      real tmp(3,0:2)
      real v
      real aa1,aa2,aa11,aa12,aa21,aa22,aCartesian(2,2),detCartesian
c --------- end local variables
      real amu,akappa,cmu1,cmu2,cmu3,ckap1,ckap2,ckap3
      common /viscosityCoefficients/ amu,akappa,cmu1,cmu2,cmu3,ckap1,
     & ckap2,ckap3

c --------- start statement functions
      v(i1,i2,k) = u(i1,i2,m1+k-1)/u(i1,i2,ir)
      aa1(i1,i2,i,j)=.5*(rx(i1  ,i2,i,j)*det(i1  ,i2)
     &                  +rx(i1+1,i2,i,j)*det(i1+1,i2))
      aa2(i1,i2,i,j)=.5*(rx(i1,i2  ,i,j)*det(i1,i2)
     &                  +rx(i1,i2+1,i,j)*det(i1,i2+1))
c --------- end statement functions

c calculate viscous flux contribution, do r-component first
      if( amu.eq.0. .and. akappa.eq.0. )then
        return
      end if

      write(*,'("addViscousTerms: amu,akappa=",2e10.3)') amu,akappa
      if( .false. )then
        return
      end if

      if( ndw.lt. (ndb-nda)*4 )then
        write(*,'(" addViscousTerms:ERROR not enough work space -- fix 
     & this Bill")')
        ! '
        stop 3318
      end if

      ir=1                      ! location of the density, momentums and energy
      m1=2
      m2=3
      ie=4

      hdi(1)=1./dr
      hdi(2)=1./ds
      do i=1,nd
        hdi2(i)=.5*hdi(i)
        hdi4(i)=.25*hdi(i)
      end do

      if( icart.eq.0 )then ! curvilinear case
! viscousMacro(curvilinear)
! #If "curvilinear" eq "curvilinear"
         do i2=n2a,n2b
           do i1=n1a-1,n1b
             ! get cell centered values
             aja=.5*(det(i1,i2)+det(i1+1,i2))
             do k=1,3
               tmp(k,0)=.5*(v(i1+1,i2,k)+v(i1,i2,k))
               tmpr=(v(i1+1,i2,k)-v(i1,i2,k))*hdi(1)
               tmps=(v(i1+1,i2+1,k)+v(i1,i2+1,k)-v(i1+1,i2-1,k)-v(i1,
     & i2-1,k))*hdi4(2)
               ! (x,y)-derivatives at cell center
               tmp(k,1)=(aa1(i1,i2,1,1)*tmpr+aa1(i1,i2,2,1)*tmps)/aja
               tmp(k,2)=(aa1(i1,i2,1,2)*tmpr+aa1(i1,i2,2,2)*tmps)/aja
             end do
             ! temperature dependent viscosities
             amu0=amu*(cmu1+cmu2*abs(tmp(3,0))**cmu3)
             amu23=2.*amu0/3.
             akappa0=akappa*(ckap1+ckap2*abs(tmp(3,0))**ckap3)
             !  stress tensor and heat flux
             tau(1,1)=amu23*(2.*tmp(1,1)-tmp(2,2))
             tau(1,2)=amu0*(tmp(1,2)+tmp(2,1))
             tau(2,1)=tau(1,2)
             tau(2,2)=amu23*(2.*tmp(2,2)-tmp(1,1))
             q(1)=-akappa0*tmp(3,1)
             q(2)=-akappa0*tmp(3,2)
             AA11=aa1(i1,i2,1,1)
             AA12=aa1(i1,i2,1,2)
             w(i1,ie)= AA11*(tmp(1,0)*tau(1,1) +tmp(2,0)*tau(2,1)-q(1))
     &  +AA12*(tmp(1,0)*tau(1,2)  +tmp(2,0)*tau(2,2)-q(2))
             w(i1,m1)= AA11*tau(1,1)+AA12*tau(1,2)
             w(i1,m2)= AA11*tau(2,1)+AA12*tau(2,2)
           end do
           do j=2,4
             do i1=n1a,n1b
               write(*,'("r: i1,i2,j=",2i4,1x,i1," up(in)=",e10.3," 
     & delta=",e10.3," new=",e10.3)') i1,i2,j,up(i1,i2,j),hdi(1)*(w(
     & i1,j)-w(i1-1,j)),up(i1,i1,j)+hdi(1)*(w(i1,j)-w(i1-1,j))
                ! '
               up(i1,i2,j)=up(i1,i2,j)+hdi(1)*(w(i1,j)-w(i1-1,j))
             end do
           end do
         end do
         ! do s-component
         do i1=n1a,n1b
           do i2=n2a-1,n2b
             ! tmp(1,0:2) : u, ur, us
             ! tmp(2,0:2) : v, vr, vs
             ! tmp(3,0:2) : T, Tr, Ts
             aja=.5*(det(i1,i2)+det(i1,i2+1))
             do k=1,3
               tmp(k,0)=.5*(v(i1,i2+1,k)+v(i1,i2,k))
               tmps=(v(i1,i2+1,k)-v(i1,i2,k))*hdi(2)
               tmpr=(v(i1+1,i2+1,k)+v(i1+1,i2,k) -v(i1-1,i2+1,k)-v(i1-
     & 1,i2,k))*hdi4(1)
               tmp(k,1)=(aa2(i1,i2,1,1)*tmpr+aa2(i1,i2,2,1)*tmps)/aja
               tmp(k,2)=(aa2(i1,i2,1,2)*tmpr+aa2(i1,i2,2,2)*tmps)/aja
             end do
             ! temperature dependent viscosties
             amu0=amu*(cmu1+cmu2*abs(tmp(3,0))**cmu3)
             amu23=2.*amu0/3.
             akappa0=akappa*(ckap1+ckap2*abs(tmp(3,0))**ckap3)
             ! stress tensor and heat flux
             tau(1,1)=amu23*(2.*tmp(1,1)-tmp(2,2))
             tau(1,2)=amu0*(tmp(1,2)+tmp(2,1))
             tau(2,1)=tau(1,2)
             tau(2,2)=amu23*(2.*tmp(2,2)-tmp(1,1))
             q(1)=-akappa0*tmp(3,1)
             q(2)=-akappa0*tmp(3,2)
             AA21=aa2(i1,i2,2,1)
             AA22=aa2(i1,i2,2,2)
             w(i2,ie)=AA21*(tmp(1,0)*tau(1,1)+tmp(2,0)*tau(2,1)-q(1))+
     & AA22*(tmp(1,0)*tau(1,2)+tmp(2,0)*tau(2,2)-q(2))
             w(i2,m1)= AA21*tau(1,1)+AA22*tau(1,2)
             w(i2,m2)= AA21*tau(2,1)+AA22*tau(2,2)
           end do
           do j=2,4
             do i2=n2a,n2b
               write(*,'("s: i1,i2,j=",2i4,1x,i1," up(in)=",e10.3," 
     & delta=",e10.3," new=",e10.3)') i1,i2,j,up(i1,i2,j),hdi(2)*(w(
     & i2,j)-w(i2-1,j)),up(i1,i2,j)+hdi(2)*(w(i2,j)-w(i2-1,j))
                  ! '
               up(i1,i2,j)=up(i1,i2,j)+hdi(2)*(w(i2,j)-w(i2-1,j))
             end do
           end do
         end do
      else
        ! The jacobian terms are constant in the cartesian grid case
        do i=1,nd
          do j=1,nd
            aCartesian(i,j)=rx(nd1a,nd2a,i,j)*det(nd1a,nd2a)
          end do
        end do
        detCartesian=det(nd1a,nd2a)

! viscousMacro(rectangular)
! #If "rectangular" eq "curvilinear"
! #Else
         do i2=n2a,n2b
           do i1=n1a-1,n1b
             ! get cell centered values
             aja=.5*(detCartesian+detCartesian)
             do k=1,3
               tmp(k,0)=.5*(v(i1+1,i2,k)+v(i1,i2,k))
               tmpr=(v(i1+1,i2,k)-v(i1,i2,k))*hdi(1)
               tmps=(v(i1+1,i2+1,k)+v(i1,i2+1,k)-v(i1+1,i2-1,k)-v(i1,
     & i2-1,k))*hdi4(2)
               ! (x,y)-derivatives at cell center
               tmp(k,1)=(aCartesian(1,1)*tmpr+aCartesian(2,1)*tmps)/aja
               tmp(k,2)=(aCartesian(1,2)*tmpr+aCartesian(2,2)*tmps)/aja
             end do
             ! temperature dependent viscosities
             amu0=amu*(cmu1+cmu2*abs(tmp(3,0))**cmu3)
             amu23=2.*amu0/3.
             akappa0=akappa*(ckap1+ckap2*abs(tmp(3,0))**ckap3)
             !  stress tensor and heat flux
             tau(1,1)=amu23*(2.*tmp(1,1)-tmp(2,2))
             tau(1,2)=amu0*(tmp(1,2)+tmp(2,1))
             tau(2,1)=tau(1,2)
             tau(2,2)=amu23*(2.*tmp(2,2)-tmp(1,1))
             q(1)=-akappa0*tmp(3,1)
             q(2)=-akappa0*tmp(3,2)
             AA11=aCartesian(1,1)
             AA12=aCartesian(1,2)
             w(i1,ie)= AA11*(tmp(1,0)*tau(1,1) +tmp(2,0)*tau(2,1)-q(1))
     &  +AA12*(tmp(1,0)*tau(1,2)  +tmp(2,0)*tau(2,2)-q(2))
             w(i1,m1)= AA11*tau(1,1)+AA12*tau(1,2)
             w(i1,m2)= AA11*tau(2,1)+AA12*tau(2,2)
           end do
           do j=2,4
             do i1=n1a,n1b
               write(*,'("r: i1,i2,j=",2i4,1x,i1," up(in)=",e10.3," 
     & delta=",e10.3," new=",e10.3)') i1,i2,j,up(i1,i2,j),hdi(1)*(w(
     & i1,j)-w(i1-1,j)),up(i1,i1,j)+hdi(1)*(w(i1,j)-w(i1-1,j))
                ! '
               up(i1,i2,j)=up(i1,i2,j)+hdi(1)*(w(i1,j)-w(i1-1,j))
             end do
           end do
         end do
         ! do s-component
         do i1=n1a,n1b
           do i2=n2a-1,n2b
             ! tmp(1,0:2) : u, ur, us
             ! tmp(2,0:2) : v, vr, vs
             ! tmp(3,0:2) : T, Tr, Ts
             aja=.5*(detCartesian+detCartesian)
             do k=1,3
               tmp(k,0)=.5*(v(i1,i2+1,k)+v(i1,i2,k))
               tmps=(v(i1,i2+1,k)-v(i1,i2,k))*hdi(2)
               tmpr=(v(i1+1,i2+1,k)+v(i1+1,i2,k) -v(i1-1,i2+1,k)-v(i1-
     & 1,i2,k))*hdi4(1)
               tmp(k,1)=(aCartesian(1,1)*tmpr+aCartesian(2,1)*tmps)/aja
               tmp(k,2)=(aCartesian(1,2)*tmpr+aCartesian(2,2)*tmps)/aja
             end do
             ! temperature dependent viscosties
             amu0=amu*(cmu1+cmu2*abs(tmp(3,0))**cmu3)
             amu23=2.*amu0/3.
             akappa0=akappa*(ckap1+ckap2*abs(tmp(3,0))**ckap3)
             ! stress tensor and heat flux
             tau(1,1)=amu23*(2.*tmp(1,1)-tmp(2,2))
             tau(1,2)=amu0*(tmp(1,2)+tmp(2,1))
             tau(2,1)=tau(1,2)
             tau(2,2)=amu23*(2.*tmp(2,2)-tmp(1,1))
             q(1)=-akappa0*tmp(3,1)
             q(2)=-akappa0*tmp(3,2)
             AA21=aCartesian(2,1)
             AA22=aCartesian(2,2)
             w(i2,ie)=AA21*(tmp(1,0)*tau(1,1)+tmp(2,0)*tau(2,1)-q(1))+
     & AA22*(tmp(1,0)*tau(1,2)+tmp(2,0)*tau(2,2)-q(2))
             w(i2,m1)= AA21*tau(1,1)+AA22*tau(1,2)
             w(i2,m2)= AA21*tau(2,1)+AA22*tau(2,2)
           end do
           do j=2,4
             do i2=n2a,n2b
               write(*,'("s: i1,i2,j=",2i4,1x,i1," up(in)=",e10.3," 
     & delta=",e10.3," new=",e10.3)') i1,i2,j,up(i1,i2,j),hdi(2)*(w(
     & i2,j)-w(i2-1,j)),up(i1,i2,j)+hdi(2)*(w(i2,j)-w(i2-1,j))
                  ! '
               up(i1,i2,j)=up(i1,i2,j)+hdi(2)*(w(i2,j)-w(i2-1,j))
             end do
           end do
         end do
      end if


      return
      end
