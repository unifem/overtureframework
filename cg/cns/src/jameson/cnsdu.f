      subroutine cnsdu22 (t,nd,ndra,ndrb,ndsa,ndsb,nrsab,
     &                    mrsab,mask,u,xy,rx,aj,ut,v,
     &                    nda,ndb,w,aa,tmp,
     &                    ipu,rpu,moving,gv )
c==================================================================
c     Calculates du/dt on conponent grid for CGCNS
c               (idmeth=2, 2 dimensions)
c
c Note: the parameter akappa = (thermal diffusivity)/(gas constant)
c
c gv : grid velocity for a moving grid problem
c Notes:
c     --- temperature dependent viscosity is input as:
c    mu=amu*(RT/rt0)**betat, kappa=akappa*(RT/rt0)**betak
c       (RT = p/rho)
c     --- but it evaluated internally as:
c     amu0=amu*(cmu1+cmu2*abs(tp)**cmu3)
c     amu23=2.*amu0/3.
c     akappa0=akappa*(ckap1+ckap2*abs(tp)**ckap3)
c==================================================================
      integer nrsab(2,2),mrsab(2,2),mask(ndra:ndrb,ndsa:ndsb),
     &        moving,ipu(*)
      real u(ndra:ndrb,ndsa:ndsb,4),
     &     rx(ndra:ndrb,ndsa:ndsb,2,2),
     &     aj(ndra:ndrb,ndsa:ndsb),
     &     ut(ndra:ndrb,ndsa:ndsb,4),
     &     gv(ndra:ndrb,ndsa:ndsb,2)
      real v(ndra:ndrb,ndsa:ndsb,4),
     &     w(nda:ndb,4),aa(nda:ndb,2,2),tmp(nda:ndb,3,0:2)
      real hd(2),hdi(2),hdi2(2),hdi4(2),tau(2,2),q(2)
      real rpu(*)
c...............local
      integer grid
c
c**      include 'cgcns2.h'
      logical d
c**      include 'cgcns.h'
c*** plotting:
c**      include 'cnspl.h'
c...start statement functions
      d(i)=mod(idebug/2**i,2).eq.1
c
c extrapolation formula
      ue(i1,i2,is1,is2,j)= 3.0*u(i1-is1  ,i2-is2  ,j)
     &                           -3.0*u(i1-2*is1,i2-2*is2,j)
     &                               +u(i1-3*is1,i2-3*is2,j)
c...end statement functions
c
c     do j=1,4
c       diff=0.
c       uchk=u(mrsab(1,1)-1,mrsab(2,1)-1,j)
c       do i1=mrsab(1,1)-1,mrsab(1,2)+1
c         do i2=mrsab(2,1)-1,mrsab(2,2)+1
c           diff=max(abs(u(i1,i2,j)-uchk),diff)
c         end do
c       end do
c       write(6,*)j,diff
c     end do
c     pause
c
      idebug=0
      ! write(*,'(" ***** entering cnsdu22 t=",f6.3," *****")') t
      ! write(*,'("u=(",(8f5.2,1x))') u

c     ...extract parameters
      grid  =ipu( 1)
      ! write(*,*) "cnsdu22: ipu=",(ipu(i),i=1,10)

      amu   =rpu( 1)
      akappa=rpu( 2)
      gamma =rpu( 3)
      av2   =rpu( 4)
      aw2   =rpu( 5)
      av4   =rpu( 6)
      aw4   =rpu( 7)
      betat =rpu( 8)
      betak =rpu( 9)
      rt0   =rpu(10)
      re    =rpu(11)  ! For plot titles only
      ama   =rpu(12)  ! For plot titles only

      hd(1) = rpu(13) ! *wdh* 050311 -- change for parallel version
      hd(2) = rpu(14) 

      t     = rpu(16)
      dt    = rpu(17)

c     index positions of (rho,u,v,e) :    
      ir=1
      m1=2
      m2=3
      ie=4

c     --- temperature dependent viscosity is input as:
c    mu=amu*(Tp/rt0)**betat, kappa=akappa*(Tp/rt0)**betak
c     --- but it evaluated internally as:
c     amu0=amu*(cmu1+cmu2*tp**cmu3)
c     amu23=2.*amu0/3.
c     akappa0=akappa*(ckap1+ckap2*tp**ckap3)
c
      if( betat.eq.0. )then
        cmu1=1.
        cmu2=0.
        cmu3=1.
      else
        cmu1=0.
        cmu2=1./rt0**betat
        cmu3=betat
      end if
      if( betak.eq.0. )then
        ckap1=1.
        ckap2=0.
        ckap3=1.
      else
        ckap1=0.
        ckap2=1./rt0**betak
        ckap3=betak
      end if
c
      if( d(4) )then
        write(31,'('' rho: '')')
        i2=1
        write(31,'( (10f7.3) )') (u(i1,i2,ir),
     &      i1=mrsab(1,1),mrsab(1,2))
        write(31,'('' m1: '')')
        i2=1
        write(31,'( (10f7.3) )') (u(i1,i2,m1),
     &      i1=mrsab(1,1),mrsab(1,2))
      end if
c
c check for zero or negative densities
      icount=0
      do i2=mrsab(2,1)-1,mrsab(2,2)+1
        do i1=mrsab(1,1)-1,mrsab(1,2)+1
          if (abs(u(i1,i2,ir)).lt.1.e-10) then
c           write(6,*)i1,i2,u(i1,i2,ir)
c*wdh            icount=icount+1
            if( mask(i1,i2).ne.0 )then
              icount=icount+1
              if( icount.lt.10 )then
                write(*,'("cnsdu22: Small density: grid=",i4," u(",'//
     &            'i4,",",i4,") =",e10.2)') grid,i1,i2,u(i1,i2,ir)
              end if
              u(i1,i2,ir)=1.e-8
            else
              u(i1,i2,ir)=1.
            end if
          end if
        end do
      end do
      if (icount.gt.0) then
        write(6,*)'cnsdu22: Small density detected, icount =',icount
        ! write(*,*) u
      end if
c
c calculate velocities, pressure and temperature
      do i2=mrsab(2,1)-1,mrsab(2,2)+1
        do i1=mrsab(1,1)-1,mrsab(1,2)+1
c..velocities
          v(i1,i2,1)=u(i1,i2,m1)/u(i1,i2,ir)
          v(i1,i2,2)=u(i1,i2,m2)/u(i1,i2,ir)
          q2=v(i1,i2,1)**2+v(i1,i2,2)**2
c..pressure and temperature*Rg
          v(i1,i2,4)=(gamma-1.0)*(u(i1,i2,ie)
     &                               -0.5*u(i1,i2,ir)*q2)
          v(i1,i2,3)=v(i1,i2,4)/u(i1,i2,ir)
        end do
      end do
c
c mesh spacings
      do kd=1,nd
! *wdh* 050311    hd(kd)=1.0/(nrsab(kd,2)-nrsab(kd,1))
        hdi(kd)=1.0/hd(kd)
        hdi2(kd)=.5*hdi(kd)
        hdi4(kd)=.25*hdi(kd)
      end do
c
c calculate inviscid flux contributions, do r-component first
      do i2=mrsab(2,1),mrsab(2,2)
        do i1=mrsab(1,1)-1,mrsab(1,2)+1
          a11=rx(i1,i2,1,1)*aj(i1,i2)
          a12=rx(i1,i2,1,2)*aj(i1,i2)
          w(i1,ir)= a11*u(i1,i2,m1)+a12*u(i1,i2,m2)
          w(i1,ie)=(a11*v(i1,i2,1)+a12*v(i1,i2,2))
     &             *(u(i1,i2,ie)+v(i1,i2,4))
          w(i1,m1)= a11*(u(i1,i2,m1)*v(i1,i2,1)+v(i1,i2,4))
     &             +a12* u(i1,i2,m2)*v(i1,i2,1)
          w(i1,m2)= a12*(u(i1,i2,m2)*v(i1,i2,2)+v(i1,i2,4))
     &             +a11* u(i1,i2,m1)*v(i1,i2,2)
        end do
c
        if( moving.gt.0 )then
          do i1=mrsab(1,1)-1,mrsab(1,2)+1
            ugv=(rx(i1,i2,1,1)*gv(i1,i2,1)
     &          +rx(i1,i2,1,2)*gv(i1,i2,2))*aj(i1,i2)
            aa(i1,1,1)=-ugv
            w(i1,ir)=w(i1,ir)-ugv*u(i1,i2,ir)
            w(i1,ie)=w(i1,ie)-ugv*u(i1,i2,ie)
            w(i1,m1)=w(i1,m1)-ugv*u(i1,i2,m1)
            w(i1,m2)=w(i1,m2)-ugv*u(i1,i2,m2)
          end do
        end if
          
c
        do j=1,4
          do i1=mrsab(1,1),mrsab(1,2)
            ut(i1,i2,j)=-hdi2(1)*(w(i1+1,j)-w(i1-1,j))
          end do
          if(  moving.gt.0 )then
            do i1=mrsab(1,1),mrsab(1,2)
              ut(i1,i2,j)=ut(i1,i2,j)+
     &          hdi2(1)*(aa(i1+1,1,1)-aa(i1-1,1,1))*u(i1,i2,j)
            end do
          end if
        end do
      end do
c
c now do s-component
      do i1=mrsab(1,1),mrsab(1,2)
        do i2=mrsab(2,1)-1,mrsab(2,2)+1
          a21=rx(i1,i2,2,1)*aj(i1,i2)
          a22=rx(i1,i2,2,2)*aj(i1,i2)
          w(i2,ir)= a21*u(i1,i2,m1)+a22*u(i1,i2,m2)
          w(i2,ie)=(a21*v(i1,i2,1)+a22*v(i1,i2,2))
     &             *(u(i1,i2,ie)+v(i1,i2,4))
          w(i2,m1)= a21*(u(i1,i2,m1)*v(i1,i2,1)+v(i1,i2,4))
     &            +a22*u(i1,i2,m2)*v(i1,i2,1)
          w(i2,m2)= a22*(u(i1,i2,m2)*v(i1,i2,2)+v(i1,i2,4))
     &            +a21*u(i1,i2,m1)*v(i1,i2,2)
        end do
        if( moving.gt.0 )then
          do i2=mrsab(2,1)-1,mrsab(2,2)+1
            ugv=(rx(i1,i2,2,1)*gv(i1,i2,1)
     &          +rx(i1,i2,2,2)*gv(i1,i2,2))*aj(i1,i2)
            aa(i2,1,1)=-ugv
            w(i2,ir)=w(i2,ir)-ugv*u(i1,i2,ir)
            w(i2,ie)=w(i2,ie)-ugv*u(i1,i2,ie)
            w(i2,m1)=w(i2,m1)-ugv*u(i1,i2,m1)
            w(i2,m2)=w(i2,m2)-ugv*u(i1,i2,m2)
          end do
        end if
c
        do j=1,4
          do i2=mrsab(2,1),mrsab(2,2)
            ut(i1,i2,j)=ut(i1,i2,j)-hdi2(2)*(w(i2+1,j)-w(i2-1,j))
          end do
          if(  moving.gt.0 )then
            do i2=mrsab(2,1),mrsab(2,2)
              ut(i1,i2,j)=ut(i1,i2,j)+
     &          hdi2(2)*(aa(i2+1,1,1)-aa(i2-1,1,1))*u(i1,i2,j)
            end do
          end if
        end do
      end do
c
c free stream correction
      if (ir.eq.0) then
      do i1=mrsab(1,1),mrsab(1,2)
        do i2=mrsab(2,1),mrsab(2,2)
          fact1=hdi2(1)*(rx(i1+1,i2,1,1)*aj(i1+1,i2)
     *                  -rx(i1-1,i2,1,1)*aj(i1-1,i2))
     *         +hdi2(2)*(rx(i1,i2+1,2,1)*aj(i1,i2+1)
     *                 - rx(i1,i2-1,2,1)*aj(i1,i2-1))
          fact2=hdi2(1)*(rx(i1+1,i2,1,2)*aj(i1+1,i2)
     *                  -rx(i1-1,i2,1,2)*aj(i1-1,i2))
     *         +hdi2(2)*(rx(i1,i2+1,2,2)*aj(i1,i2+1)
     *                 - rx(i1,i2-1,2,2)*aj(i1,i2-1))
          ut(i1,i2,1)=ut(i1,i2,1)+fact1*u(i1,i2,m1)+fact2*u(i1,i2,m2)
          ut(i1,i2,2)=ut(i1,i2,2)+fact1*(u(i1,i2,m1)*v(i1,i2,1)
     *                                              +v(i1,i2,4))
     *                           +fact2*u(i1,i2,m2)*v(i1,i2,1)
          ut(i1,i2,3)=ut(i1,i2,3)+fact1*u(i1,i2,m1)*v(i1,i2,2)
     *                           +fact2*(u(i1,i2,m2)*v(i1,i2,2)
     *                                              +v(i1,i2,4))
          ut(i1,i2,4)=ut(i1,i2,4)+(fact1*v(i1,i2,1)+fact2*v(i1,i2,2))
     *                           *(u(i1,i2,ie)+v(i1,i2,4))
c         write(6,246)i1,i2,(ut(i1,i2,i),i=1,4)
c 246     format(2(1x,i3),4(1x,1pe9.2))
        end do
      end do
c     pause
      end if
c
c free stream correction
      do i1=mrsab(1,1),mrsab(1,2)
        do i2=mrsab(2,1),mrsab(2,2)
          fact1=hdi2(1)*(rx(i1+1,i2,1,1)*aj(i1+1,i2)
     *                  -rx(i1-1,i2,1,1)*aj(i1-1,i2))
     *         +hdi2(2)*(rx(i1,i2+1,2,1)*aj(i1,i2+1)
     *                 - rx(i1,i2-1,2,1)*aj(i1,i2-1))
          fact2=hdi2(1)*(rx(i1+1,i2,1,2)*aj(i1+1,i2)
     *                  -rx(i1-1,i2,1,2)*aj(i1-1,i2))
     *         +hdi2(2)*(rx(i1,i2+1,2,2)*aj(i1,i2+1)
     *                 - rx(i1,i2-1,2,2)*aj(i1,i2-1))
          amn=fact1*v(i1,i2,1)
     *       +fact2*v(i1,i2,2)
          ut(i1,i2,1)=ut(i1,i2,1)+amn*u(i1,i2,1)
          ut(i1,i2,2)=ut(i1,i2,2)+amn*u(i1,i2,2)
     *                           +fact1*v(i1,i2,4)
          ut(i1,i2,3)=ut(i1,i2,3)+amn*u(i1,i2,3)
     *                           +fact2*v(i1,i2,4)
          ut(i1,i2,4)=ut(i1,i2,4)+amn*(u(i1,i2,ie)+v(i1,i2,4))
c         write(6,246)i1,i2,(ut(i1,i2,i),i=1,4)
c 246     format(2(1x,i3),4(1x,1pe9.2))
        end do
      end do
c     pause
c
c calculate viscous flux contribution, do r-component first
      if( amu.ne.0. .or. akappa.ne.0. )then
        do i2=mrsab(2,1),mrsab(2,2)
          do i=1,2
            do j=1,2
              do i1=mrsab(1,1)-1,mrsab(1,2)
                aa(i1,i,j)=.5*(rx(i1  ,i2,i,j)*aj(i1  ,i2)
     &                        +rx(i1+1,i2,i,j)*aj(i1+1,i2))
              end do
            end do
          end do
          do k=1,3
            do i1=mrsab(1,1)-1,mrsab(1,2)
              aja=.5*(aj(i1,i2)+aj(i1+1,i2))
c solution at cell center
              tmp(i1,k,0)=.5*(v(i1+1,i2,k)+v(i1,i2,k))
              tmpr=(v(i1+1,i2,k)-v(i1,i2,k))*hdi(1)
              tmps=(v(i1+1,i2+1,k)+v(i1,i2+1,k)
     &             -v(i1+1,i2-1,k)-v(i1,i2-1,k))*hdi4(2)
c (x,y)-derivatives at cell center
              tmp(i1,k,1)=(aa(i1,1,1)*tmpr+aa(i1,2,1)*tmps)/aja
              tmp(i1,k,2)=(aa(i1,1,2)*tmpr+aa(i1,2,2)*tmps)/aja
            end do
          end do
c
          do i1=mrsab(1,1)-1,mrsab(1,2)
c
c temperature dependent viscosties
            amu0=amu*(cmu1+cmu2*abs(tmp(i1,3,0))**cmu3)
            amu23=2.*amu0/3.
            akappa0=akappa*(ckap1+ckap2*abs(tmp(i1,3,0))**ckap3)
c
c stress tensor and heat flux
            tau(1,1)=amu23*(2.*tmp(i1,1,1)-tmp(i1,2,2))
            tau(1,2)=amu0*(tmp(i1,1,2)+tmp(i1,2,1))
            tau(2,1)=tau(1,2)
            tau(2,2)=amu23*(2.*tmp(i1,2,2)-tmp(i1,1,1))
            q(1)=-akappa0*tmp(i1,3,1)
            q(2)=-akappa0*tmp(i1,3,2)
c
            w(i1,ie)= aa(i1,1,1)*(tmp(i1,1,0)*tau(1,1)
     &                           +tmp(i1,2,0)*tau(2,1)-q(1))
     &               +aa(i1,1,2)*(tmp(i1,1,0)*tau(1,2)
     &                           +tmp(i1,2,0)*tau(2,2)-q(2))
            w(i1,m1)= aa(i1,1,1)*tau(1,1)+aa(i1,1,2)*tau(1,2)
            w(i1,m2)= aa(i1,1,1)*tau(2,1)+aa(i1,1,2)*tau(2,2)
          end do
          do j=2,4
            do i1=mrsab(1,1),mrsab(1,2)
              ut(i1,i2,j)=ut(i1,i2,j)+hdi(1)*(w(i1,j)-w(i1-1,j))
            end do
          end do
        end do
c
c do s-component
        do i1=mrsab(1,1),mrsab(1,2)
          do i=1,2
            do j=1,2
              do i2=mrsab(2,1)-1,mrsab(2,2)
                aa(i2,i,j)=.5*(rx(i1,i2  ,i,j)*aj(i1,i2)
     &                        +rx(i1,i2+1,i,j)*aj(i1,i2+1))
              end do
            end do
          end do
          do k=1,3
            do i2=mrsab(2,1)-1,mrsab(2,2)
              aja=.5*(aj(i1,i2)+aj(i1,i2+1))
c solution at cell center
              tmp(i2,k,0)=.5*(v(i1,i2+1,k)+v(i1,i2,k))
              tmps=(v(i1,i2+1,k)-v(i1,i2,k))*hdi(2)
              tmpr=(v(i1+1,i2+1,k)+v(i1+1,i2,k)
     &             -v(i1-1,i2+1,k)-v(i1-1,i2,k))*hdi4(1)
c derivative at cell center
              tmp(i2,k,1)=(aa(i2,1,1)*tmpr+aa(i2,2,1)*tmps)/aja
              tmp(i2,k,2)=(aa(i2,1,2)*tmpr+aa(i2,2,2)*tmps)/aja
            end do
          end do
c
          do i2=mrsab(2,1)-1,mrsab(2,2)
c
c temperature dependent viscosties
            amu0=amu*(cmu1+cmu2*abs(tmp(i2,3,0))**cmu3)
            amu23=2.*amu0/3.
            akappa0=akappa*(ckap1+ckap2*abs(tmp(i2,3,0))**ckap3)
c
c stress tensor and heat flux
            tau(1,1)=amu23*(2.*tmp(i2,1,1)-tmp(i2,2,2))
            tau(1,2)=amu0*(tmp(i2,1,2)+tmp(i2,2,1))
            tau(2,1)=tau(1,2)
            tau(2,2)=amu23*(2.*tmp(i2,2,2)-tmp(i2,1,1))
            q(1)=-akappa0*tmp(i2,3,1)
            q(2)=-akappa0*tmp(i2,3,2)
c
            w(i2,ie)=aa(i2,2,1)*(tmp(i2,1,0)*tau(1,1)
     &                          +tmp(i2,2,0)*tau(2,1)-q(1))
     &              +aa(i2,2,2)*(tmp(i2,1,0)*tau(1,2)
     &                          +tmp(i2,2,0)*tau(2,2)-q(2))
            w(i2,m1)= aa(i2,2,1)*tau(1,1)+aa(i2,2,2)*tau(1,2)
            w(i2,m2)= aa(i2,2,1)*tau(2,1)+aa(i2,2,2)*tau(2,2)
          end do
          do j=2,4
            do i2=mrsab(2,1),mrsab(2,2)
              ut(i1,i2,j)=ut(i1,i2,j)+hdi(2)*(w(i2,j)-w(i2-1,j))
            end do
          end do
        end do
      end if
c
c calculate artificial viscosity (2nd and 4th order, r-component)
      if( av2.ne.0. .or. av4.ne.0. )then
        do i2=mrsab(2,1),mrsab(2,2)
          do i1=mrsab(1,1),mrsab(1,2)
            w(i1,1)=abs(
     &         (v(i1+1,i2,4)-2.0*v(i1,i2,4)+v(i1-1,i2,4))
     &        /(v(i1+1,i2,4)+2.0*v(i1,i2,4)+v(i1-1,i2,4)))
          end do
          ! *wdh* 050323 --- fix this for parallel ---
          i1=mrsab(1,1)-1
          w(i1  ,1)=0.
          w(i1-1,1)=0.
          i1=mrsab(1,2)+1
          w(i1  ,1)=0.
          w(i1+1,1)=0.
          do i1=mrsab(1,1)-1,mrsab(1,2)
            a1=.5*(rx(i1,i2,1,1)*aj(i1,i2)+rx(i1+1,i2,1,1)*aj(i1+1,i2))
            a2=.5*(rx(i1,i2,1,2)*aj(i1,i2)+rx(i1+1,i2,1,2)*aj(i1+1,i2))
            v1=.5*(v(i1,i2,1)+v(i1+1,i2,1))
            v2=.5*(v(i1,i2,2)+v(i1+1,i2,2))
            dist=sqrt(a1**2+a2**2)
            vn=a1*v1+a2*v2
c **** why doesn't the next line have a .5* ... ????
c*wdh 981110           csq=gamma*(v(i1,i2,3)+v(i1+1,i2,3))
            csq=.5*gamma*(v(i1,i2,3)+v(i1+1,i2,3))
c..Bill: why is this here? Do you remember? Was there a problem with
c        gamma*p/rho being negative?
c*** fix this?
            if( csq.lt.0. )then
              if( mask(i1,i2).gt.0 )then
                write(*,9000) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
     &            u(i1,i2,3),gv(i1,i2,1),gv(i1,i2,2)
 9000           format('cnsdu22: csq<0, mask>0 i1=',i3,' i2=',i3,
     &           ' csq=',e8.2,' rho=',f5.2,' p=',f6.2,' e=',f6.2,
     &           ' (u,v)=',2f5.2,' (m1,m2)=',2f5.2,' gv=',2f5.2)
                write(*,9020) grid,t,dt,ndra,ndrb,ndsa,ndsb,mrsab(1,1),
     &            mrsab(1,2),mrsab(2,1),mrsab(2,2)
 9020  format('  grid=',i3,' t=',f6.3,' dt=',e8.2,' ndra,...=',4i5,
     &        ' mrsab=',4i5)

              end if
              c=0.
            else
c$$$             if( mask(i1,i2).gt.0 )then
c$$$                write(*,9010) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
c$$$     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
c$$$     &            u(i1,i2,3),gv(i1,i2,1),gv(i1,i2,2)
c$$$ 9010           format('CNSDT: csq>0, mask>0 i1+=',i3,' i2=',i3,
c$$$     &           ' csq=',e8.2,' rho=',f5.2,' p=',f6.2,' e=',f6.2,
c$$$     &           ' (u,v)=',2f5.2,' (m1,m2)=',2f5.2,' gv=',2f5.2)
c$$$              end if
              c=sqrt(csq)
            end if
            alam=(abs(vn)+c*dist)*hdi(1)
            wmax=max(w(i1-1,1),w(i1,1),w(i1+1,1),w(i1+2,1))
            w(i1,2)=av2*alam*min(1.0,wmax/aw2)
            w(i1,3)=av4*alam*max(0.,1.0-wmax/aw4)
c*wdh            if (wmax.gt.1.e-6) then
c*wdh              w(i1,3)=av4*alam*max(0.,1.0-wmax/aw4)
c*wdh            else
c*wdh              w(i1,3)=0.
c*wdh            end if
          end do
          do j=1,4
            do i1=mrsab(1,1)-1,mrsab(1,2)
              w(i1,1)=w(i1,2)*(u(i1+1,i2,j)-u(i1,i2,j))
            end do
            ! *wdh* 050323 -- check this too ---
            i1=mrsab(1,1)-2
            w(i1,4)=ue(i1,i2,-1,0,j)
            do i1=mrsab(1,1)-1,mrsab(1,2)+1
              w(i1,4)=u(i1,i2,j)
            end do
            i1=mrsab(1,2)+2
            w(i1,4)=ue(i1,i2,+1,0,j)
            do i1=mrsab(1,1)-1,mrsab(1,2)
              w(i1,1)=w(i1,1)-w(i1,3)*(w(i1+2,4)
     &                -3.0*(w(i1+1,4)-w(i1,4))-w(i1-1,4))
            end do
ckkc MASS FLUX FIX XXX
c060504 uncomment these lines if activating the cnsNoSlipWallBC for CNS Jameson
c            w(mrsab(1,1)-1,1) = -w(mrsab(1,1),1)
c            w(mrsab(1,2),1) = -w(mrsab(1,2)-1,1)

            do i1=mrsab(1,1),mrsab(1,2)
              ut(i1,i2,j)=ut(i1,i2,j)+(w(i1,1)-w(i1-1,1))
            end do
          end do
        end do
c
c calculate artificial viscosity (2nd and 4th order, s-component)
        do i1=mrsab(1,1),mrsab(1,2)
          do i2=mrsab(2,1),mrsab(2,2)
            w(i2,1)=abs(
     &        (v(i1,i2+1,4)-2.0*v(i1,i2,4)+v(i1,i2-1,4))
     &       /(v(i1,i2+1,4)+2.0*v(i1,i2,4)+v(i1,i2-1,4)))
          end do

          ! *wdh* 050323 --- fix this for parallel ---
          i2=mrsab(2,1)-1
          w(i2  ,1)=0.
          w(i2-1,1)=0.
          i2=mrsab(2,2)+1
          w(i2  ,1)=0.
          w(i2+1,1)=0.
          do i2=mrsab(2,1)-1,mrsab(2,2)
            a1=.5*(rx(i1,i2,2,1)*aj(i1,i2)+rx(i1,i2+1,2,1)*aj(i1,i2+1))
            a2=.5*(rx(i1,i2,2,2)*aj(i1,i2)+rx(i1,i2+1,2,2)*aj(i1,i2+1))
            dist=sqrt(a1**2+a2**2)
            v1=.5*(v(i1,i2,1)+v(i1,i2+1,1))
            v2=.5*(v(i1,i2,2)+v(i1,i2+1,2))
            vn=a1*v1+a2*v2
            csq=.5*gamma*(v(i1,i2,3)+v(i1,i2+1,3))
c..Bill: same comment as above
            if( csq.lt.0. )then
              if( mask(i1,i2).gt.0 )then
                write(*,9000) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
     &            u(i1,i2,3),gv(i1,i2,1),gv(i1,i2,2)
                write(*,9020) grid,t,dt,ndra,ndrb,ndsa,ndsb,mrsab(1,1),
     &            mrsab(1,2),mrsab(2,1),mrsab(2,2)
              end if
              c=0.
            else
c$$$             if( mask(i1,i2).gt.0 )then
c$$$                write(*,9010) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
c$$$     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
c$$$     &            u(i1,i2,3),gv(i1,i2,1),gv(i1,i2,2)
c$$$              end if
              c=sqrt(csq)
            end if
            alam=(abs(vn)+c*dist)*hdi(2)
            wmax=max(w(i2-1,1),w(i2,1),w(i2+1,1),w(i2+2,1))
            w(i2,2)=av2*alam*min(1.0,wmax/aw2)
            w(i2,3)=av4*alam*max(0.,1.0-wmax/aw4)
c*wdh            if (wmax.gt.1.e-6) then
c*wdh              w(i2,3)=av4*alam*max(0.,1.0-wmax/aw4)
c*wdh            else
c*wdh              w(i2,3)=0.
c*wdh            end if
          end do
          do j=1,4
            do i2=mrsab(2,1)-1,mrsab(2,2)
              w(i2,1)=w(i2,2)*(u(i1,i2+1,j)-u(i1,i2,j))
            end do
            i2=mrsab(2,1)-2
            w(i2,4)=ue(i1,i2,0,-1,j)
            do i2=mrsab(2,1)-1,mrsab(2,2)+1
              w(i2,4)=u(i1,i2,j)
            end do
            i2=mrsab(2,2)+2
            w(i2,4)=ue(i1,i2,0,+1,j)
            do i2=mrsab(2,1)-1,mrsab(2,2)
              w(i2,1)=w(i2,1)-w(i2,3)*(w(i2+2,4)
     &              -3.0*(w(i2+1,4)-w(i2,4))-w(i2-1,4))
            end do
ckkc MASS FLUX FIX XXX
c060504 uncomment these lines if activating the cnsNoSlipWallBC for CNS Jameson
c            w(mrsab(2,1)-1,1) = -w(mrsab(2,1),1)
c            w(mrsab(2,2),1) = -w(mrsab(2,2)-1,1)

            do i2=mrsab(2,1),mrsab(2,2)
              ut(i1,i2,j)=ut(i1,i2,j)+(w(i2,1)-w(i2-1,1))
            end do
          end do
        end do
      end if
c
c divide by the Jacobian
      do i2=mrsab(2,1),mrsab(2,2)
        do i1=mrsab(1,1),mrsab(1,2)
          ajac=1.0/aj(i1,i2)
          ut(i1,i2,1)=ut(i1,i2,1)*ajac
          ut(i1,i2,2)=ut(i1,i2,2)*ajac
          ut(i1,i2,3)=ut(i1,i2,3)*ajac
          ut(i1,i2,4)=ut(i1,i2,4)*ajac
c         write(6,246)i1,i2,(ut(i1,i2,i),i=1,4)
c 246     format(2(1x,i3),4(1x,1pe9.2))
        end do
      end do
c     pause
c
c add forcing to ut
c**      call cnsfn22 (t,nd,ndra,ndrb,ndsa,ndsb,mrsab,mask,ut,xy)
c
      return
      end

      subroutine cnsdu23 (t,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,nrsab,
     &                    mrsab,mask,u,xy,rx,aj,ut,v,nda,ndb,w,aa,tmp,
     &                    ipu,rpu,moving,gv)
c==================================================================
c     Calculates du/dt on conponent grid for CGCNS
c               (idmeth=2, 3 dimensions)
c
c Note: the parameter akappa = (thermal diffusivity)/(gas constant)
c
c Notes:
c     --- temperature dependent viscosity is input as:
c    mu=amu*(RT/rt0)**betat, kappa=akappa*(RT/rt0)**betak
c       (RT = p/rho)
c     --- but it evaluated internally as:
c     amu0=amu*(cmu1+cmu2*abs(tp)**cmu3)
c     amu23=2.*amu0/3.
c     akappa0=akappa*(ckap1+ckap2*abs(tp)**ckap3)
c==================================================================
      integer nrsab(3,2),mrsab(3,2),mask(ndra:ndrb,ndsa:ndsb,ndta:ndtb),
     &        moving,ipu(*)
      real u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,5),
     &     rx(ndra:ndrb,ndsa:ndsb,ndta:ndtb,3,3),
     &     aj(ndra:ndrb,ndsa:ndsb,ndta:ndtb),
     &     ut(ndra:ndrb,ndsa:ndsb,ndta:ndtb,5),
     &     gv(ndra:ndrb,ndsa:ndsb,ndta:ndtb,3)
      real v(ndra:ndrb,ndsa:ndsb,ndta:ndtb,5),
     &     w(nda:ndb,5),aa(nda:ndb,3,3),tmp(nda:ndb,4,0:3)
      real hd(3),hdi(3),hdi2(3),hdi4(3),tau(3,3),q(3)
      real rpu(*)
c...............local
      integer grid
c
      logical d
c...start statement functions
      d(i)=mod(idebug/2**i,2).eq.1
c
c extrapolation formula
      ue(i1,i2,i3,is1,is2,is3,j)= 3.0*u(i1-is1  ,i2-is2  ,i3-is3  ,j)
     &                           -3.0*u(i1-2*is1,i2-2*is2,i3-2*is3,j)
     &                               +u(i1-3*is1,i2-3*is2,i3-3*is3,j)
c...end statement functions

c*wdh
      idebug=0

c     ...extract parameters
      grid  =ipu( 1)

      amu   =rpu( 1)
      akappa=rpu( 2)
      gamma =rpu( 3)
      av2   =rpu( 4)
      aw2   =rpu( 5)
      av4   =rpu( 6)
      aw4   =rpu( 7)
      betat =rpu( 8)
      betak =rpu( 9)
      rt0   =rpu(10)
      re    =rpu(11)  ! For plot titles only
      ama   =rpu(12)  ! For plot titles only

      hd(1) = rpu(13) ! *wdh* 050311 -- change for parallel version
      hd(2) = rpu(14) 
      hd(3) = rpu(15)

      t     = rpu(16)
      dt    = rpu(17)

c     index positions for rho, m1, m2,...
      ir=1
      m1=2
      m2=3
      m3=4
      ie=5

c     --- temperature dependent viscosity is input as:
c    mu=amu*(Tp/rt0)**betat, kappa=akappa*(Tp/rt0)**betak
c     --- but it evaluated internally as:
c     amu0=amu*(cmu1+cmu2*tp**cmu3)
c     amu23=2.*amu0/3.
c     akappa0=akappa*(ckap1+ckap2*tp**ckap3)
c
      if( betat.eq.0. )then
        cmu1=1.
        cmu2=0.
        cmu3=1.
      else
        cmu1=0.
        cmu2=1./rt0**betat
        cmu3=betat
      end if
      if( betak.eq.0. )then
        ckap1=1.
        ckap2=0.
        ckap3=1.
      else
        ckap1=0.
        ckap2=1./rt0**betak
        ckap3=betak
      end if
c
      if( d(4) )then
        write(31,'('' rho: '')')
        i2=1
        i3=1
        write(31,'( (10f7.3) )') (u(i1,i2,i3,1),
     &      i1=mrsab(1,1),mrsab(1,2))
        write(31,'('' m1: '')')
        i2=1
        i3=1
        write(31,'( (10f7.3) )') (u(i1,i2,i3,3),
     &      i1=mrsab(1,1),mrsab(1,2))
      end if
c
c check for zero or negative densities
      icount=0
      do i3=mrsab(3,1)-1,mrsab(3,2)+1
        do i2=mrsab(2,1)-1,mrsab(2,2)+1
          do i1=mrsab(1,1)-1,mrsab(1,2)+1
            if (abs(u(i1,i2,i3,ir)).lt.1.e-10) then
c             write(6,*)i1,i2,i3,u(i1,i2,i3,1)
              icount=icount+1
              u(i1,i2,i3,ir)=1.
              if( mask(i1,i2,i3).ne.0 )then
                u(i1,i2,i3,ir)=1.e-8
                icount=icount+1
              end if
            end if
          end do
        end do
      end do
      if (icount.gt.0) then
        write(6,*)'cnsdu23: Zero density detected, icount =',icount
      end if
c
c calculate velocities, pressure and temperature
      do i3=mrsab(3,1)-1,mrsab(3,2)+1
        do i2=mrsab(2,1)-1,mrsab(2,2)+1
          do i1=mrsab(1,1)-1,mrsab(1,2)+1
c..velocities
            v(i1,i2,i3,1)=u(i1,i2,i3,m1)/u(i1,i2,i3,ir)
            v(i1,i2,i3,2)=u(i1,i2,i3,m2)/u(i1,i2,i3,ir)
            v(i1,i2,i3,3)=u(i1,i2,i3,m3)/u(i1,i2,i3,ir)
            q2=v(i1,i2,i3,1)**2+v(i1,i2,i3,2)**2+v(i1,i2,i3,3)**2
c..pressure and temperature
            v(i1,i2,i3,5)=(gamma-1.0)*(u(i1,i2,i3,ie)
     &                                 -0.5*u(i1,i2,i3,ir)*q2)
            v(i1,i2,i3,4)=v(i1,i2,i3,5)/u(i1,i2,i3,ir)
          end do
        end do
      end do
c
c mesh spacings
      do kd=1,nd
! *wdh* 050311      hd(kd)=1.0/(nrsab(kd,2)-nrsab(kd,1))
        hdi(kd)=1.0/hd(kd)
        hdi2(kd)=.5*hdi(kd)
        hdi4(kd)=.25*hdi(kd)
      end do
c
c calculate inviscid flux contributions, do r-component first
      do i3=mrsab(3,1),mrsab(3,2)
        do i2=mrsab(2,1),mrsab(2,2)
          do i1=mrsab(1,1)-1,mrsab(1,2)+1
            a11=rx(i1,i2,i3,1,1)*aj(i1,i2,i3)
            a12=rx(i1,i2,i3,1,2)*aj(i1,i2,i3)
            a13=rx(i1,i2,i3,1,3)*aj(i1,i2,i3)
            w(i1,ir)= a11*u(i1,i2,i3,m1)+a12*u(i1,i2,i3,m2)
     &               +a13*u(i1,i2,i3,m3)
            w(i1,ie)=(a11*v(i1,i2,i3,1)+a12*v(i1,i2,i3,2)
     &               +a13*v(i1,i2,i3,3))*(u(i1,i2,i3,ie)+v(i1,i2,i3,5))
            w(i1,m1)= a11*(u(i1,i2,i3,m1)*v(i1,i2,i3,1)+v(i1,i2,i3,5))
     &               +a12* u(i1,i2,i3,m2)*v(i1,i2,i3,1)
     &               +a13* u(i1,i2,i3,m3)*v(i1,i2,i3,1)
            w(i1,m2)= a12*(u(i1,i2,i3,m2)*v(i1,i2,i3,2)+v(i1,i2,i3,5))
     &               +a13* u(i1,i2,i3,m3)*v(i1,i2,i3,2)
     &               +a11* u(i1,i2,i3,m1)*v(i1,i2,i3,2)
            w(i1,m3)= a13*(u(i1,i2,i3,m3)*v(i1,i2,i3,3)+v(i1,i2,i3,5))
     &               +a11* u(i1,i2,i3,m1)*v(i1,i2,i3,3)
     &               +a12* u(i1,i2,i3,m2)*v(i1,i2,i3,3)
          end do
          if( moving.gt.0 )then
            do i1=mrsab(1,1)-1,mrsab(1,2)+1
              ugv=(rx(i1,i2,i3,1,1)*gv(i1,i2,i3,1)
     &            +rx(i1,i2,i3,1,2)*gv(i1,i2,i3,2)
     &            +rx(i1,i2,i3,1,3)*gv(i1,i2,i3,3))*aj(i1,i2,i3)
              aa(i1,1,1)=-ugv
              w(i1,ir)=w(i1,ir)-ugv*u(i1,i2,i3,ir)
              w(i1,ie)=w(i1,ie)-ugv*u(i1,i2,i3,ie)
              w(i1,m1)=w(i1,m1)-ugv*u(i1,i2,i3,m1)
              w(i1,m2)=w(i1,m2)-ugv*u(i1,i2,i3,m2)
              w(i1,m3)=w(i1,m3)-ugv*u(i1,i2,i3,m3)
            end do
          end if
c
          do j=1,5
            do i1=mrsab(1,1),mrsab(1,2)
              ut(i1,i2,i3,j)=-hdi2(1)*(w(i1+1,j)-w(i1-1,j))
            end do
            if(  moving.gt.0 )then
              do i1=mrsab(1,1),mrsab(1,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+
     &            hdi2(1)*(aa(i1+1,1,1)-aa(i1-1,1,1))*u(i1,i2,i3,j)
              end do
            end if
          end do
        end do
      end do
c
c now do s-component
      do i1=mrsab(1,1),mrsab(1,2)
        do i3=mrsab(3,1),mrsab(3,2)
          do i2=mrsab(2,1)-1,mrsab(2,2)+1
            a21=rx(i1,i2,i3,2,1)*aj(i1,i2,i3)
            a22=rx(i1,i2,i3,2,2)*aj(i1,i2,i3)
            a23=rx(i1,i2,i3,2,3)*aj(i1,i2,i3)
            w(i2,ir)= a21*u(i1,i2,i3,m1)+a22*u(i1,i2,i3,m2)
     &               +a23*u(i1,i2,i3,m3)
            w(i2,ie)=( a21*v(i1,i2,i3,1)+a22*v(i1,i2,i3,2)
     &                +a23*v(i1,i2,i3,3))*(u(i1,i2,i3,ie)+v(i1,i2,i3,5))
            w(i2,m1)= a21*(u(i1,i2,i3,m1)*v(i1,i2,i3,1)+v(i1,i2,i3,5))
     &               +a22* u(i1,i2,i3,m2)*v(i1,i2,i3,1)
     &               +a23* u(i1,i2,i3,m3)*v(i1,i2,i3,1)
            w(i2,m2)= a22*(u(i1,i2,i3,m2)*v(i1,i2,i3,2)+v(i1,i2,i3,5))
     &               +a23* u(i1,i2,i3,m3)*v(i1,i2,i3,2)
     &               +a21* u(i1,i2,i3,m1)*v(i1,i2,i3,2)
            w(i2,m3)= a23*(u(i1,i2,i3,m3)*v(i1,i2,i3,3)+v(i1,i2,i3,5))
     &               +a21* u(i1,i2,i3,m1)*v(i1,i2,i3,3)
     &               +a22* u(i1,i2,i3,m2)*v(i1,i2,i3,3)
          end do
          if( moving.gt.0 )then
            do i2=mrsab(2,1)-1,mrsab(2,2)+1
              ugv=(rx(i1,i2,i3,2,1)*gv(i1,i2,i3,1)
     &            +rx(i1,i2,i3,2,2)*gv(i1,i2,i3,2)
     &            +rx(i1,i2,i3,2,3)*gv(i1,i2,i3,3))*aj(i1,i2,i3)
              aa(i2,1,1)=-ugv
              w(i2,ir)=w(i2,ir)-ugv*u(i1,i2,i3,ir)
              w(i2,ie)=w(i2,ie)-ugv*u(i1,i2,i3,ie)
              w(i2,m1)=w(i2,m1)-ugv*u(i1,i2,i3,m1)
              w(i2,m2)=w(i2,m2)-ugv*u(i1,i2,i3,m2)
              w(i2,m3)=w(i2,m3)-ugv*u(i1,i2,i3,m3)
            end do
          end if
c
          do j=1,5
            do i2=mrsab(2,1),mrsab(2,2)
              ut(i1,i2,i3,j)=ut(i1,i2,i3,j)
     &                          -hdi2(2)*(w(i2+1,j)-w(i2-1,j))
            end do
            if(  moving.gt.0 )then
              do i2=mrsab(2,1),mrsab(2,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+
     &            hdi2(2)*(aa(i2+1,1,1)-aa(i2-1,1,1))*u(i1,i2,i3,j)
              end do
            end if
          end do
        end do
      end do
c
c now do t-component
      do i2=mrsab(2,1),mrsab(2,2)
        do i1=mrsab(1,1),mrsab(1,2)
          do i3=mrsab(3,1)-1,mrsab(3,2)+1
            a31=rx(i1,i2,i3,3,1)*aj(i1,i2,i3)
            a32=rx(i1,i2,i3,3,2)*aj(i1,i2,i3)
            a33=rx(i1,i2,i3,3,3)*aj(i1,i2,i3)
            w(i3,ir)= a31*u(i1,i2,i3,m1)+a32*u(i1,i2,i3,m2)
     &               +a33*u(i1,i2,i3,m3)
            w(i3,ie)=( a31*v(i1,i2,i3,1)+a32*v(i1,i2,i3,2)
     &                +a33*v(i1,i2,i3,3))*(u(i1,i2,i3,ie)+v(i1,i2,i3,5))
            w(i3,m1)= a31*(u(i1,i2,i3,m1)*v(i1,i2,i3,1)+v(i1,i2,i3,5))
     &               +a32* u(i1,i2,i3,m2)*v(i1,i2,i3,1)
     &               +a33* u(i1,i2,i3,m3)*v(i1,i2,i3,1)
            w(i3,m2)= a32*(u(i1,i2,i3,m2)*v(i1,i2,i3,2)+v(i1,i2,i3,5))
     &               +a33* u(i1,i2,i3,m3)*v(i1,i2,i3,2)
     &               +a31* u(i1,i2,i3,m1)*v(i1,i2,i3,2)
            w(i3,m3)= a33*(u(i1,i2,i3,m3)*v(i1,i2,i3,3)+v(i1,i2,i3,5))
     &               +a31* u(i1,i2,i3,m1)*v(i1,i2,i3,3)
     &               +a32* u(i1,i2,i3,m2)*v(i1,i2,i3,3)
          end do
c
          if( moving.gt.0 )then
            do i3=mrsab(3,1)-1,mrsab(3,2)+1
              ugv=(rx(i1,i2,i3,3,1)*gv(i1,i2,i3,1)
     &            +rx(i1,i2,i3,3,2)*gv(i1,i2,i3,2)
     &            +rx(i1,i2,i3,3,3)*gv(i1,i2,i3,3))*aj(i1,i2,i3)
              aa(i3,1,1)=-ugv
              w(i3,ir)=w(i3,ir)-ugv*u(i1,i2,i3,ir)
              w(i3,ie)=w(i3,ie)-ugv*u(i1,i2,i3,ie)
              w(i3,m1)=w(i3,m1)-ugv*u(i1,i2,i3,m1)
              w(i3,m2)=w(i3,m2)-ugv*u(i1,i2,i3,m2)
              w(i3,m3)=w(i3,m3)-ugv*u(i1,i2,i3,m3)
            end do
          end if
c
          do j=1,5
            do i3=mrsab(3,1),mrsab(3,2)
              ut(i1,i2,i3,j)=ut(i1,i2,i3,j)
     &                          -hdi2(3)*(w(i3+1,j)-w(i3-1,j))
            end do
            if(  moving.gt.0 )then
              do i3=mrsab(3,1),mrsab(3,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+
     &            hdi2(3)*(aa(i3+1,1,1)-aa(i3-1,1,1))*u(i1,i2,i3,j)
              end do
            end if
          end do
        end do
      end do
c
c free stream correction
      do i1=mrsab(1,1),mrsab(1,2)
        do i2=mrsab(2,1),mrsab(2,2)
          do i3=mrsab(3,1),mrsab(3,2)
            fact1=hdi2(1)*(rx(i1+1,i2,i3,1,1)*aj(i1+1,i2,i3)
     *                    -rx(i1-1,i2,i3,1,1)*aj(i1-1,i2,i3))
     *           +hdi2(2)*(rx(i1,i2+1,i3,2,1)*aj(i1,i2+1,i3)
     *                    -rx(i1,i2-1,i3,2,1)*aj(i1,i2-1,i3))
     *           +hdi2(3)*(rx(i1,i2,i3+1,3,1)*aj(i1,i2,i3+1)
     *                    -rx(i1,i2,i3-1,3,1)*aj(i1,i2,i3-1))
            fact2=hdi2(1)*(rx(i1+1,i2,i3,1,2)*aj(i1+1,i2,i3)
     *                    -rx(i1-1,i2,i3,1,2)*aj(i1-1,i2,i3))
     *           +hdi2(2)*(rx(i1,i2+1,i3,2,2)*aj(i1,i2+1,i3)
     *                    -rx(i1,i2-1,i3,2,2)*aj(i1,i2-1,i3))
     *           +hdi2(3)*(rx(i1,i2,i3+1,3,2)*aj(i1,i2,i3+1)
     *                    -rx(i1,i2,i3-1,3,2)*aj(i1,i2,i3-1))
            fact3=hdi2(1)*(rx(i1+1,i2,i3,1,3)*aj(i1+1,i2,i3)
     *                    -rx(i1-1,i2,i3,1,3)*aj(i1-1,i2,i3))
     *           +hdi2(2)*(rx(i1,i2+1,i3,2,3)*aj(i1,i2+1,i3)
     *                    -rx(i1,i2-1,i3,2,3)*aj(i1,i2-1,i3))
     *           +hdi2(3)*(rx(i1,i2,i3+1,3,3)*aj(i1,i2,i3+1)
     *                    -rx(i1,i2,i3-1,3,3)*aj(i1,i2,i3-1))
            amn=fact1*v(i1,i2,i3,1)
     *         +fact2*v(i1,i2,i3,2)
     *         +fact3*v(i1,i2,i3,3)
            ut(i1,i2,i3,1)=ut(i1,i2,i3,1)+amn*u(i1,i2,i3,1)
            ut(i1,i2,i3,2)=ut(i1,i2,i3,2)+amn*u(i1,i2,i3,2)
     *                                   +fact1*v(i1,i2,i3,4)
            ut(i1,i2,i3,3)=ut(i1,i2,i3,3)+amn*u(i1,i2,i3,3)
     *                                   +fact2*v(i1,i2,i3,4)
            ut(i1,i2,i3,4)=ut(i1,i2,i3,4)+amn*u(i1,i2,i3,4)
     *                                   +fact3*v(i1,i2,i3,4)
            ut(i1,i2,i3,5)=ut(i1,i2,i3,5)+amn*(u(i1,i2,i3,ie)
     *                                        +v(i1,i2,i3,4))
          end do
        end do
      end do
c
c calculate viscous flux contribution, do r-component first
      if( amu.ne.0. .or. akappa.ne.0. )then
        do i3=mrsab(3,1),mrsab(3,2)
          do i2=mrsab(2,1),mrsab(2,2)
            do i=1,3
              do j=1,3
                do i1=mrsab(1,1)-1,mrsab(1,2)
                  aa(i1,i,j)=.5*(rx(i1  ,i2,i3,i,j)*aj(i1  ,i2,i3)
     &                          +rx(i1+1,i2,i3,i,j)*aj(i1+1,i2,i3))
                end do
              end do
            end do
            do k=1,4
              do i1=mrsab(1,1)-1,mrsab(1,2)
                aja=.5*(aj(i1,i2,i3)+aj(i1+1,i2,i3))
c solution at cell center
                tmp(i1,k,0)=.5*(v(i1+1,i2,i3,k)+v(i1,i2,i3,k))
                tmpr=(v(i1+1,i2,i3,k)-v(i1,i2,i3,k))*hdi(1)
                tmps=(v(i1+1,i2+1,i3,k)+v(i1,i2+1,i3,k)
     &               -v(i1+1,i2-1,i3,k)-v(i1,i2-1,i3,k))*hdi4(2)
                tmpt=(v(i1+1,i2,i3+1,k)+v(i1,i2,i3+1,k)
     &               -v(i1+1,i2,i3-1,k)-v(i1,i2,i3-1,k))*hdi4(3)
c (x,y,z)-derivatives at cell center
                tmp(i1,k,1)=( aa(i1,1,1)*tmpr+aa(i1,2,1)*tmps
     &                       +aa(i1,3,1)*tmpt)/aja
                tmp(i1,k,2)=( aa(i1,1,2)*tmpr+aa(i1,2,2)*tmps
     &                       +aa(i1,3,2)*tmpt)/aja
                tmp(i1,k,3)=( aa(i1,1,3)*tmpr+aa(i1,2,3)*tmps
     &                       +aa(i1,3,3)*tmpt)/aja
              end do
            end do
c
            do i1=mrsab(1,1)-1,mrsab(1,2)
c
c temperature dependent viscosties
              amu0=amu*(cmu1+cmu2*abs(tmp(i1,4,0))**cmu3)
              amu23=2.*amu0/3.
              akappa0=akappa*(ckap1+ckap2*abs(tmp(i1,4,0))**ckap3)
c
c stress tensor and heat flux
              tau(1,1)=amu23*(2.*tmp(i1,1,1)-tmp(i1,2,2)-tmp(i1,3,3))
              tau(1,2)=amu0*(tmp(i1,1,2)+tmp(i1,2,1))
              tau(1,3)=amu0*(tmp(i1,1,3)+tmp(i1,3,1))
              tau(2,1)=tau(1,2)
              tau(2,2)=amu23*(2.*tmp(i1,2,2)-tmp(i1,3,3)-tmp(i1,1,1))
              tau(2,3)=amu0*(tmp(i1,2,3)+tmp(i1,3,2))
              tau(3,1)=tau(1,3)
              tau(3,2)=tau(2,3)
              tau(3,3)=amu23*(2.*tmp(i1,3,3)-tmp(i1,1,1)-tmp(i1,2,2))
              q(1)=-akappa0*tmp(i1,4,1)
              q(2)=-akappa0*tmp(i1,4,2)
              q(3)=-akappa0*tmp(i1,4,3)
c
              w(i1,ie)= aa(i1,1,1)*( tmp(i1,1,0)*tau(1,1)
     &                              +tmp(i1,2,0)*tau(2,1)
     &                              +tmp(i1,3,0)*tau(3,1)-q(1))
     &                 +aa(i1,1,2)*( tmp(i1,1,0)*tau(1,2)
     &                              +tmp(i1,2,0)*tau(2,2)
     &                              +tmp(i1,3,0)*tau(3,2)-q(2))
     &                 +aa(i1,1,3)*( tmp(i1,1,0)*tau(1,3)
     &                              +tmp(i1,2,0)*tau(2,3)
     &                              +tmp(i1,3,0)*tau(3,3)-q(3))
              w(i1,m1)=aa(i1,1,1)*tau(1,1)+aa(i1,1,2)*tau(1,2)
     &                +aa(i1,1,3)*tau(1,3)
              w(i1,m2)=aa(i1,1,1)*tau(2,1)+aa(i1,1,2)*tau(2,2)
     &                +aa(i1,1,3)*tau(2,3)
              w(i1,m3)=aa(i1,1,1)*tau(3,1)+aa(i1,1,2)*tau(3,2)
     &                +aa(i1,1,3)*tau(3,3)
            end do
            do j=2,5
              do i1=mrsab(1,1),mrsab(1,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+hdi(1)*(w(i1,j)-w(i1-1,j))
              end do
            end do
          end do
        end do
c
c do s-component
        do i1=mrsab(1,1),mrsab(1,2)
          do i3=mrsab(3,1),mrsab(3,2)
            do i=1,3
              do j=1,3
                do i2=mrsab(2,1)-1,mrsab(2,2)
                  aa(i2,i,j)=.5*(rx(i1,i2  ,i3,i,j)*aj(i1,i2  ,i3)
     &                          +rx(i1,i2+1,i3,i,j)*aj(i1,i2+1,i3))
                end do
              end do
            end do
            do k=1,4
              do i2=mrsab(2,1)-1,mrsab(2,2)
                aja=.5*(aj(i1,i2,i3)+aj(i1,i2+1,i3))
c solution at cell center
                tmp(i2,k,0)=.5*(v(i1,i2+1,i3,k)+v(i1,i2,i3,k))
                tmps=(v(i1,i2+1,i3,k)-v(i1,i2,i3,k))*hdi(2)
                tmpt=( v(i1,i2+1,i3+1,k)+v(i1,i2,i3+1,k)
     &               -v(i1,i2+1,i3-1,k)-v(i1,i2,i3-1,k))*hdi4(3)
                tmpr=( v(i1+1,i2+1,i3,k)+v(i1+1,i2,i3,k)
     &               -v(i1-1,i2+1,i3,k)-v(i1-1,i2,i3,k))*hdi4(1)
c derivative at cell center
                tmp(i2,k,1)=( aa(i2,1,1)*tmpr+aa(i2,2,1)*tmps
     &                       +aa(i2,3,1)*tmpt)/aja
                tmp(i2,k,2)=( aa(i2,1,2)*tmpr+aa(i2,2,2)*tmps
     &                       +aa(i2,3,2)*tmpt)/aja
                tmp(i2,k,3)=( aa(i2,1,3)*tmpr+aa(i2,2,3)*tmps
     &                       +aa(i2,3,3)*tmpt)/aja
              end do
            end do
c
            do i2=mrsab(2,1)-1,mrsab(2,2)
c
c temperature dependent viscosties
              amu0=amu*(cmu1+cmu2*abs(tmp(i2,4,0))**cmu3)
              amu23=2.*amu0/3.
              akappa0=akappa*(ckap1+ckap2*abs(tmp(i2,4,0))**ckap3)
c
c stress tensor and heat flux
              tau(1,1)=amu23*(2.*tmp(i2,1,1)-tmp(i2,2,2)-tmp(i2,3,3))
              tau(1,2)=amu0*(tmp(i2,1,2)+tmp(i2,2,1))
              tau(1,3)=amu0*(tmp(i2,1,3)+tmp(i2,3,1))
              tau(2,1)=tau(1,2)
              tau(2,2)=amu23*(2.*tmp(i2,2,2)-tmp(i2,3,3)-tmp(i2,1,1))
              tau(2,3)=amu0*(tmp(i2,2,3)+tmp(i2,3,2))
              tau(3,1)=tau(1,3)
              tau(3,2)=tau(2,3)
              tau(3,3)=amu23*(2.*tmp(i2,3,3)-tmp(i2,1,1)-tmp(i2,2,2))
              q(1)=-akappa0*tmp(i2,4,1)
              q(2)=-akappa0*tmp(i2,4,2)
              q(3)=-akappa0*tmp(i2,4,3)
c
              w(i2,ie)=aa(i2,2,1)*( tmp(i2,1,0)*tau(1,1)
     &                             +tmp(i2,2,0)*tau(2,1)
     &                             +tmp(i2,3,0)*tau(3,1)-q(1))
     &                +aa(i2,2,2)*( tmp(i2,1,0)*tau(1,2)
     &                             +tmp(i2,2,0)*tau(2,2)
     &                             +tmp(i2,3,0)*tau(3,2)-q(2))
     &                +aa(i2,2,3)*( tmp(i2,1,0)*tau(1,3)
     &                             +tmp(i2,2,0)*tau(2,3)
     &                             +tmp(i2,3,0)*tau(3,3)-q(3))
              w(i2,m1)=aa(i2,2,1)*tau(1,1)+aa(i2,2,2)*tau(1,2)
     &                +aa(i2,2,3)*tau(1,3)
              w(i2,m2)=aa(i2,2,1)*tau(2,1)+aa(i2,2,2)*tau(2,2)
     &                +aa(i2,2,3)*tau(2,3)
              w(i2,m3)=aa(i2,2,1)*tau(3,1)+aa(i2,2,2)*tau(3,2)
     &                +aa(i2,2,3)*tau(3,3)
            end do
            do j=2,5
              do i2=mrsab(2,1),mrsab(2,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+hdi(2)*(w(i2,j)-w(i2-1,j))
              end do
            end do
          end do
        end do
c
c do t-component
        do i2=mrsab(2,1),mrsab(2,2)
          do i1=mrsab(1,1),mrsab(1,2)
            do i=1,3
              do j=1,3
                do i3=mrsab(3,1)-1,mrsab(3,2)
                  aa(i3,i,j)=.5*(rx(i1,i2,i3  ,i,j)*aj(i1,i2,i3)
     &                          +rx(i1,i2,i3+1,i,j)*aj(i1,i2,i3+1))
                end do
              end do
            end do
            do k=1,4
              do i3=mrsab(3,1)-1,mrsab(3,2)
                aja=.5*(aj(i1,i2,i3)+aj(i1,i2,i3+1))
c solution at cell center
                tmp(i3,k,0)=.5*(v(i1,i2,i3+1,k)+v(i1,i2,i3,k))
                tmpt=(v(i1,i2,i3+1,k)-v(i1,i2,i3,k))*hdi(3)
                tmpr=(v(i1+1,i2,i3+1,k)+v(i1+1,i2,i3,k)
     &               -v(i1-1,i2,i3+1,k)-v(i1-1,i2,i3,k))*hdi4(1)
                tmps=(v(i1,i2+1,i3+1,k)+v(i1,i2+1,i3,k)
     &               -v(i1,i2-1,i3+1,k)-v(i1,i2-1,i3,k))*hdi4(2)
c derivative at cell center
                tmp(i3,k,1)=( aa(i3,1,1)*tmpr+aa(i3,2,1)*tmps
     &                       +aa(i3,3,1)*tmpt)/aja
                tmp(i3,k,2)=( aa(i3,1,2)*tmpr+aa(i3,2,2)*tmps
     &                       +aa(i3,3,2)*tmpt)/aja
                tmp(i3,k,3)=( aa(i3,1,3)*tmpr+aa(i3,2,3)*tmps
     &                       +aa(i3,3,3)*tmpt)/aja
              end do
            end do
c
            do i3=mrsab(3,1)-1,mrsab(3,2)
c
c temperature dependent viscosties
              amu0=amu*(cmu1+cmu2*abs(tmp(i3,4,0))**cmu3)
              amu23=2.*amu0/3.
              akappa0=akappa*(ckap1+ckap2*abs(tmp(i3,4,0))**ckap3)
c
c stress tensor and heat flux
              tau(1,1)=amu23*(2.*tmp(i3,1,1)-tmp(i3,2,2)-tmp(i3,3,3))
              tau(1,2)=amu0*(tmp(i3,1,2)+tmp(i3,2,1))
              tau(1,3)=amu0*(tmp(i3,1,3)+tmp(i3,3,1))
              tau(2,1)=tau(1,2)
              tau(2,2)=amu23*(2.*tmp(i3,2,2)-tmp(i3,3,3)-tmp(i3,1,1))
              tau(2,3)=amu0*(tmp(i3,2,3)+tmp(i3,3,2))
              tau(3,1)=tau(1,3)
              tau(3,2)=tau(2,3)
              tau(3,3)=amu23*(2.*tmp(i3,3,3)-tmp(i3,1,1)-tmp(i3,2,2))
              q(1)=-akappa0*tmp(i3,4,1)
              q(2)=-akappa0*tmp(i3,4,2)
              q(3)=-akappa0*tmp(i3,4,3)
c
              w(i3,ie)=aa(i3,3,1)*( tmp(i3,1,0)*tau(1,1)
     &                             +tmp(i3,2,0)*tau(2,1)
     &                             +tmp(i3,3,0)*tau(3,1)-q(1))
     &                +aa(i3,3,2)*( tmp(i3,1,0)*tau(1,2)
     &                             +tmp(i3,2,0)*tau(2,2)
     &                             +tmp(i3,3,0)*tau(3,2)-q(2))
     &                +aa(i3,3,3)*( tmp(i3,1,0)*tau(1,3)
     &                             +tmp(i3,2,0)*tau(2,3)
     &                             +tmp(i3,3,0)*tau(3,3)-q(3))
              w(i3,m1)=aa(i3,3,1)*tau(1,1)+aa(i3,3,2)*tau(1,2)
     &                +aa(i3,3,3)*tau(1,3)
              w(i3,m2)=aa(i3,3,1)*tau(2,1)+aa(i3,3,2)*tau(2,2)
     &                +aa(i3,3,3)*tau(2,3)
              w(i3,m3)=aa(i3,3,1)*tau(3,1)+aa(i3,3,2)*tau(3,2)
     &                +aa(i3,3,3)*tau(3,3)
            end do
            do j=2,5
              do i3=mrsab(3,1),mrsab(3,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+hdi(3)*(w(i3,j)-w(i3-1,j))
              end do
            end do
          end do
        end do
      end if
c
c calculate artificial viscosity (2nd and 4th order, r-component)

      if( av2.ne.0. .or. av4.ne.0. )then
        do i3=mrsab(3,1),mrsab(3,2)
          do i2=mrsab(2,1),mrsab(2,2)
            do i1=mrsab(1,1),mrsab(1,2)
              w(i1,1)=abs(
     &           (v(i1+1,i2,i3,5)-2.0*v(i1,i2,i3,5)+v(i1-1,i2,i3,5))
     &          /(v(i1+1,i2,i3,5)+2.0*v(i1,i2,i3,5)+v(i1-1,i2,i3,5)))
            end do
            i1=mrsab(1,1)-1
            w(i1  ,1)=0.
            w(i1-1,1)=0.
            i1=mrsab(1,2)+1
            w(i1  ,1)=0.
            w(i1+1,1)=0.
            do i1=mrsab(1,1)-1,mrsab(1,2)
              a1=.5*(rx(i1  ,i2,i3,1,1)*aj(i1  ,i2,i3)
     &              +rx(i1+1,i2,i3,1,1)*aj(i1+1,i2,i3))
              a2=.5*(rx(i1  ,i2,i3,1,2)*aj(i1  ,i2,i3)
     &              +rx(i1+1,i2,i3,1,2)*aj(i1+1,i2,i3))
              a3=.5*(rx(i1  ,i2,i3,1,3)*aj(i1  ,i2,i3)
     &              +rx(i1+1,i2,i3,1,3)*aj(i1+1,i2,i3))
c kkc 051116     &              +rx(i1+1,i2,i3,1,3)*aj(i+11,i2,i3)) 
              v1=.5*(v(i1,i2,i3,1)+v(i1+1,i2,i3,1))
              v2=.5*(v(i1,i2,i3,2)+v(i1+1,i2,i3,2))
              v3=.5*(v(i1,i2,i3,3)+v(i1+1,i2,i3,3))
              dist=sqrt(a1**2+a2**2+a3**2)
              vn=a1*v1+a2*v2+a3*v3
              csq=.5*gamma*(v(i1,i2,i3,4)+v(i1+1,i2,i3,4))
c..Bill: why is this here? Do you remember? Was there a problem with
c        gamma*p/rho being negative?
c*** fix this?
              if( csq.lt.0. )then
                if( mask(i1,i2,i3).gt.0 )then
                  write(*,
     &             '('' CNSDU: csq<0 mask>0 i1,i2,i3,csq=''3i5,e10.2)')
     &              i1,i2,i3,csq
                end if
                c=0.
              else
                c=sqrt(csq)
              end if
              alam=(abs(vn)+c*dist)*hdi(1)
              wmax=max(w(i1-1,1),w(i1,1),w(i1+1,1),w(i1+2,1))
              w(i1,2)=av2*alam*min(1.0,wmax/aw2)
              if (wmax.gt.1.e-6) then
                w(i1,3)=av4*alam*max(0.,1.0-wmax/aw4)
              else
                w(i1,3)=0.
              end if
*               isw(i1)=min(1,int(wmax/aw2))+ 2*(1-min(1,int(wmax/aw4)))
*               if( wmax.gt.aw2 )then
*                 isw(i1)=1
*               else
*                 isw(i1)=0
*               end if
            end do
            do j=1,5
              do i1=mrsab(1,1)-1,mrsab(1,2)
                w(i1,1)=w(i1,2)*(u(i1+1,i2,i3,j)-u(i1,i2,i3,j))
              end do
              i1=mrsab(1,1)-2
              w(i1,4)=ue(i1,i2,i3,-1,0,0,j)
              do i1=mrsab(1,1)-1,mrsab(1,2)+1
                w(i1,4)=u(i1,i2,i3,j)
              end do
              i1=mrsab(1,2)+2
              w(i1,4)=ue(i1,i2,i3,+1,0,0,j)
              do i1=mrsab(1,1)-1,mrsab(1,2)
                w(i1,1)=w(i1,1)-w(i1,3)*(w(i1+2,4)
     &                  -3.0*(w(i1+1,4)-w(i1,4))-w(i1-1,4))
              end do
              do i1=mrsab(1,1),mrsab(1,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+(w(i1,1)-w(i1-1,1))
              end do
            end do
          end do
        end do
c
c calculate artificial viscosity (2nd and 4th order, s-component)
        do i3=mrsab(3,1),mrsab(3,2)
          do i1=mrsab(1,1),mrsab(1,2)
            do i2=mrsab(2,1),mrsab(2,2)
              w(i2,1)=abs(
     &          (v(i1,i2+1,i3,5)-2.0*v(i1,i2,i3,5)+v(i1,i2-1,i3,5))
     &         /(v(i1,i2+1,i3,5)+2.0*v(i1,i2,i3,5)+v(i1,i2-1,i3,5)))
            end do
            i2=mrsab(2,1)-1
            w(i2  ,1)=0.
            w(i2-1,1)=0.
            i2=mrsab(2,2)+1
            w(i2  ,1)=0.
            w(i2+1,1)=0.
            do i2=mrsab(2,1)-1,mrsab(2,2)
              a1=.5*(rx(i1,i2  ,i3,2,1)*aj(i1,i2  ,i3)
     &              +rx(i1,i2+1,i3,2,1)*aj(i1,i2+1,i3))
              a2=.5*(rx(i1,i2  ,i3,2,2)*aj(i1,i2  ,i3)
     &              +rx(i1,i2+1,i3,2,2)*aj(i1,i2+1,i3))
              a3=.5*(rx(i1,i2  ,i3,2,3)*aj(i1,i2  ,i3)
     &              +rx(i1,i2+1,i3,2,3)*aj(i1,i2+1,i3))
              dist=sqrt(a1**2+a2**2+a3**2)
              v1=.5*(v(i1,i2,i3,1)+v(i1,i2+1,i3,1))
              v2=.5*(v(i1,i2,i3,2)+v(i1,i2+1,i3,2))
              v3=.5*(v(i1,i2,i3,3)+v(i1,i2+1,i3,3))
              vn=a1*v1+a2*v2+a3*v3
              csq=.5*gamma*(v(i1,i2,i3,4)+v(i1,i2+1,i3,4))
c..Bill: same comment as above
              if( csq.lt.0. )then
                if( mask(i1,i2,i3).gt.0 )then
                  write(*,
     &             '('' CNSDU: csq<0 mask>0 i1,i2,i3,csq=''3i5,e10.2)')
     &              i1,i2,i3,csq
                end if
                c=0.
              else
                c=sqrt(csq)
              end if
              alam=(abs(vn)+c*dist)*hdi(2)
              wmax=max(w(i2-1,1),w(i2,1),w(i2+1,1),w(i2+2,1))
              w(i2,2)=av2*alam*min(1.0,wmax/aw2)
              if (wmax.gt.1.e-6) then
                w(i2,3)=av4*alam*max(0.,1.0-wmax/aw4)
              else
                w(i2,3)=0.
              end if
            end do
            do j=1,5
              do i2=mrsab(2,1)-1,mrsab(2,2)
                w(i2,1)=w(i2,2)*(u(i1,i2+1,i3,j)-u(i1,i2,i3,j))
              end do
              i2=mrsab(2,1)-2
              w(i2,4)=ue(i1,i2,i3,0,-1,0,j)
              do i2=mrsab(2,1)-1,mrsab(2,2)+1
                w(i2,4)=u(i1,i2,i3,j)
              end do
              i2=mrsab(2,2)+2
              w(i2,4)=ue(i1,i2,i3,0,+1,0,j)
              do i2=mrsab(2,1)-1,mrsab(2,2)
                w(i2,1)=w(i2,1)-w(i2,3)*(w(i2+2,4)
     &                -3.0*(w(i2+1,4)-w(i2,4))-w(i2-1,4))
              end do
              do i2=mrsab(2,1),mrsab(2,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+(w(i2,1)-w(i2-1,1))
              end do
            end do
          end do
        end do
c
c calculate artificial viscosity (2nd and 4th order, t-component)
        do i1=mrsab(1,1),mrsab(1,2)
          do i2=mrsab(2,1),mrsab(2,2)
            do i3=mrsab(3,1),mrsab(3,2)
              w(i3,1)=abs(
     &          (v(i1,i2,i3+1,5)-2.0*v(i1,i2,i3,5)+v(i1,i2,i3-1,5))
     &         /(v(i1,i2,i3+1,5)+2.0*v(i1,i2,i3,5)+v(i1,i2,i3-1,5)))
            end do
            i3=mrsab(3,1)-1
            w(i3  ,1)=0.
            w(i3-1,1)=0.
            i3=mrsab(3,2)+1
            w(i3  ,1)=0.
            w(i3+1,1)=0.
            do i3=mrsab(3,1)-1,mrsab(3,2)
              a1=.5*(rx(i1,i2,i3  ,3,1)*aj(i1,i2,i3  )
     &              +rx(i1,i2,i3+1,3,1)*aj(i1,i2,i3+1))
              a2=.5*(rx(i1,i2,i3  ,3,2)*aj(i1,i2,i3  )
     &              +rx(i1,i2,i3+1,3,2)*aj(i1,i2,i3+1))
              a3=.5*(rx(i1,i2,i3  ,3,3)*aj(i1,i2,i3  )
     &              +rx(i1,i2,i3+1,3,3)*aj(i1,i2,i3+1))
              dist=sqrt(a1**2+a2**2+a3**2)
              v1=.5*(v(i1,i2,i3,1)+v(i1,i2,i3+1,1))
              v2=.5*(v(i1,i2,i3,2)+v(i1,i2,i3+1,2))
              v3=.5*(v(i1,i2,i3,3)+v(i1,i2,i3+1,3))
              vn=a1*v1+a2*v2+a3*v3
              csq=.5*gamma*(v(i1,i2,i3,4)+v(i1,i2,i3+1,4))
c..Bill: same comment as above
              if( csq.lt.0. )then
                if( mask(i1,i2,i3).gt.0 )then
                  write(*,
     &             '('' CNSDU: csq<0 mask>0 i1,i2,i3,csq=''3i5,e10.2)')
     &              i1,i2,i3,csq
                end if
                c=0.
              else
                c=sqrt(csq)
              end if
              alam=(abs(vn)+c*dist)*hdi(3)
              wmax=max(w(i3-1,1),w(i3,1),w(i3+1,1),w(i3+2,1))
              w(i3,2)=av2*alam*min(1.0,wmax/aw2)
              if (wmax.gt.1.e-6) then
                w(i3,3)=av4*alam*max(0.,1.0-wmax/aw4)
              else
                w(i3,3)=0.
              end if
            end do
            do j=1,5
              do i3=mrsab(3,1)-1,mrsab(3,2)
                w(i3,1)=w(i3,2)*(u(i1,i2,i3+1,j)-u(i1,i2,i3,j))
              end do
              i3=mrsab(3,1)-2
              w(i3,4)=ue(i1,i2,i3,0,0,-1,j)
              do i3=mrsab(3,1)-1,mrsab(3,2)+1
                w(i3,4)=u(i1,i2,i3,j)
              end do
              i3=mrsab(3,2)+2
              w(i3,4)=ue(i1,i2,i3,0,0,+1,j)
              do i3=mrsab(3,1)-1,mrsab(3,2)
                w(i3,1)=w(i3,1)-w(i3,3)*(w(i3+2,4)
     &                -3.0*(w(i3+1,4)-w(i3,4))-w(i3-1,4))
              end do
              do i3=mrsab(3,1),mrsab(3,2)
                ut(i1,i2,i3,j)=ut(i1,i2,i3,j)+(w(i3,1)-w(i3-1,1))
              end do
            end do
          end do
        end do
      end if
c
c divide by the Jacobian
      do i3=mrsab(3,1),mrsab(3,2)
        do i2=mrsab(2,1),mrsab(2,2)
          do i1=mrsab(1,1),mrsab(1,2)
            ajac=1.0/aj(i1,i2,i3)
            ut(i1,i2,i3,1)=ut(i1,i2,i3,1)*ajac
            ut(i1,i2,i3,2)=ut(i1,i2,i3,2)*ajac
            ut(i1,i2,i3,3)=ut(i1,i2,i3,3)*ajac
            ut(i1,i2,i3,4)=ut(i1,i2,i3,4)*ajac
            ut(i1,i2,i3,5)=ut(i1,i2,i3,5)*ajac
          end do
        end do
      end do
c
c add forcing to ut
**      call cnsfn23 (t,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
**     &             mrsab,mask,ut,xy)
c
      return
      end


      subroutine cnsdu22a (t,nd,nc,
     &                     ndra,ndrb,ndsa,ndsb,nrsab,
     &                     mrsab,mask,u,xy,rx,aj,ut,v,
     &                     nda,ndb,w,aa,tmp,
     &                     ipu,rpu,moving,gv )
c==================================================================
c     Calculates du/dt on conponent grid for CGCNS
c               (idmeth=2, 2 dimensions, axisymmetric with swirl)
c
c kkc 051115 : Initial version adapted from cnsdu22 

c
c Notes from cnsdu22:
c Note: the parameter akappa = (thermal diffusivity)/(gas constant)
c
c gv : grid velocity for a moving grid problem
c Notes:
c     --- temperature dependent viscosity is input as:
c    mu=amu*(RT/rt0)**betat, kappa=akappa*(RT/rt0)**betak
c       (RT = p/rho)
c     --- but it evaluated internally as:
c     amu0=amu*(cmu1+cmu2*abs(tp)**cmu3)
c     amu23=2.*amu0/3.
c     akappa0=akappa*(ckap1+ckap2*abs(tp)**ckap3)
c==================================================================
c     anonymous uiuc aae prof, circa 1994: 
c                    "use of explicit type declarations 
c                            is a sign of programmer weakness"
c
c
c     so i'm weak, kkc
      implicit none

c     INPUT
      integer nd,nc,ndra,ndrb,ndsa,ndsb,nda,ndb
      integer nrsab(2,2),mrsab(2,2),mask(ndra:ndrb,ndsa:ndsb),
     &        moving,ipu(*)
      real u(ndra:ndrb,ndsa:ndsb,nc),
     &     rx(ndra:ndrb,ndsa:ndsb,2,2),
     &     aj(ndra:ndrb,ndsa:ndsb),
     &     gv(ndra:ndrb,ndsa:ndsb,2)
      real xy(ndra:ndrb,ndsa:ndsb,2) 
      real t
      real rpu(*)

c     LOCALS SET BY rpu AND ipu
      integer grid,irad,irad0,irad1
      real amu,akappa,gamma,av2,aw2,av4,aw4,betat,betak,rt0,re
      real ama,hd(2),dt

c     WORKSPACE
      real v(ndra:ndrb,ndsa:ndsb,5),
     &     w(nda:ndb,5),aa(nda:ndb,2,2),tmp(nda:ndb,4,0:2)

c     OUTPUT
      real ut(ndra:ndrb,ndsa:ndsb,5)

c     LOCAL
      integer i,idebug,i1,i2,is1,is2,j,ir,m1,m2,m3,imr,imz,ie
      integer iv1,iv2,iv3,ivp,ivt,ivr,ivz
      integer kd,k,icount
      real hdi(2),hdi2(2),hdi4(2),tau(3,3),q(2)
      real cmu1,cmu2,cmu3,ckap1,ckap2,ckap3,a11,a12,a21,a22
      real q2,ugv,fact1,fact2,amn,aja,tmpr,tmps,amu0,amu23
      real akappa0,a1,a2,v1,v2,v3,dist,vn,csq,c,alam,ajac,wmax
      real rad,wrad,dmdt,dpdt,dedt
      real ur,vr,tr,wr,uror,vror,uzor,tror,vzor,rxa,r1,r2
      logical addaxiterms,axiv,axic
c...............local
ckkc      integer grid
c
c**      include 'cgcns2.h'
c**      include 'cgcns.h'
c*** plotting:
c**      include 'cnspl.h'
c     STATEMENT FUNCTIONS
      logical d
      real ue
c...start statement functions
      d(i)=mod(idebug/2**i,2).eq.1
c
c extrapolation formula
      ue(i1,i2,is1,is2,j)= 3.0*u(i1-is1  ,i2-is2  ,j)
     &                           -3.0*u(i1-2*is1,i2-2*is2,j)
     &                               +u(i1-3*is1,i2-3*is2,j)
c...end statement functions
c
c     do j=1,4
c       diff=0.
c       uchk=u(mrsab(1,1)-1,mrsab(2,1)-1,j)
c       do i1=mrsab(1,1)-1,mrsab(1,2)+1
c         do i2=mrsab(2,1)-1,mrsab(2,2)+1
c           diff=max(abs(u(i1,i2,j)-uchk),diff)
c         end do
c       end do
c       write(6,*)j,diff
c     end do
c     pause
c
      idebug=0
      addaxiterms=.true.
      axiv = addaxiterms .and. .true.
      axic = addaxiterms .and. .true.
      ! write(*,'(" ***** entering cnsdu22 t=",f6.3," *****")') t
      ! write(*,'("u=(",(8f5.2,1x))') u

c     ...extract parameters
      grid = ipu( 1)
      irad = ipu(11) ! the coord. direction for the radius
      irad0= ipu(12) ! axis index for side=0
      irad1= ipu(13) ! axis index for side=1

c      write(*,*) "cnsdu22a: ipu=",(ipu(i),i=1,13)
c      print *,"irad = ",irad,", irad0 = ",irad0,", irad1= ",irad1
c      print *,"mrsab1 = ",mrsab(1,1),mrsab(1,2)
c      print *,"mrsab2 = ",mrsab(2,1),mrsab(2,2)
c      print *,"ndrab = ",ndra,ndrb
c      print *,"ndsab = ",ndsa,ndsb

      amu   =rpu( 1)
      akappa=rpu( 2)
      gamma =rpu( 3)
      av2   =rpu( 4)
      aw2   =rpu( 5)
      av4   =rpu( 6)
      aw4   =rpu( 7)
      betat =rpu( 8)
      betak =rpu( 9)
      rt0   =rpu(10)
      re    =rpu(11)  ! For plot titles only
      ama   =rpu(12)  ! For plot titles only

      hd(1) = rpu(13) ! *wdh* 050311 -- change for parallel version
      hd(2) = rpu(14) 

      t     = rpu(16)
      dt    = rpu(17)

c     index positions of (rho,u,v,w,e) :    
      ir=1
      m1=2
      m2=3
      m3=4 !kkc
      ie=5 !kkc 

      iv1=1 ! z velocity
      iv2=2 ! r velocity
      iv3=3 ! theta velocity
      ivt=4 ! p/rho
      ivp=5 ! p

      imr = m2  ! r momentum
      imz = m1  ! z momentum
      ivr = iv2 ! r velocity
      ivz = iv1 ! z velocity


c     --- temperature dependent viscosity is input as:
c    mu=amu*(Tp/rt0)**betat, kappa=akappa*(Tp/rt0)**betak
c     --- but it evaluated internally as:
c     amu0=amu*(cmu1+cmu2*tp**cmu3)
c     amu23=2.*amu0/3.
c     akappa0=akappa*(ckap1+ckap2*tp**ckap3)
c
      if( betat.eq.0. )then
        cmu1=1.
        cmu2=0.
        cmu3=1.
      else
        cmu1=0.
        cmu2=1./rt0**betat
        cmu3=betat
      end if
      if( betak.eq.0. )then
        ckap1=1.
        ckap2=0.
        ckap3=1.
      else
        ckap1=0.
        ckap2=1./rt0**betak
        ckap3=betak
      end if
c
      if( d(4) )then
        write(31,'('' rho: '')')
        i2=1
        write(31,'( (10f7.3) )') (u(i1,i2,ir),
     &      i1=mrsab(1,1),mrsab(1,2))
        write(31,'('' m1: '')')
        i2=1
        write(31,'( (10f7.3) )') (u(i1,i2,m1),
     &      i1=mrsab(1,1),mrsab(1,2))
      end if
c
c check for zero or negative densities
      icount=0
      do i2=mrsab(2,1)-1,mrsab(2,2)+1
        do i1=mrsab(1,1)-1,mrsab(1,2)+1
          if (abs(u(i1,i2,ir)).lt.1.e-10) then
c           write(6,*)i1,i2,u(i1,i2,ir)
c*wdh            icount=icount+1
            if( mask(i1,i2).ne.0 )then
              icount=icount+1
              if( icount.lt.10 )then
                write(*,'("cnsdu22a: Small density: grid=",i4," u(",'//
     &            'i4,",",i4,") =",e10.2)') grid,i1,i2,u(i1,i2,ir)
              end if
              u(i1,i2,ir)=1.e-8
            else
              u(i1,i2,ir)=1.
            end if
          end if
        end do
      end do
      if (icount.gt.0) then
        write(6,*)'cnsdu22a: Small density detected, icount =',icount
        ! write(*,*) u
      end if
c
c calculate velocities, pressure and temperature
      do i2=mrsab(2,1)-2,mrsab(2,2)+2
        do i1=mrsab(1,1)-2,mrsab(1,2)+2
c..velocities
          v(i1,i2,iv1)=u(i1,i2,m1)/u(i1,i2,ir)
          v(i1,i2,iv2)=u(i1,i2,m2)/u(i1,i2,ir)
          v(i1,i2,iv3)=u(i1,i2,m3)/u(i1,i2,ir)
          q2=v(i1,i2,iv1)**2+v(i1,i2,iv2)**2 + v(i1,i2,iv3)**2
c..pressure and temperature*Rg
          v(i1,i2,ivp)=(gamma-1.0)*(u(i1,i2,ie)
     &                               -0.5d0*u(i1,i2,ir)*q2)
          v(i1,i2,ivt)=v(i1,i2,ivp)/u(i1,i2,ir)
          if ( v(i1,i2,ivt).lt.1e-10 ) then
             if ( mask(i1,i2).ne.0 ) then
                icount = icount+1
                if ( icount.lt.30) then
           write (*,'("cnsdu22a: Small temperature: grid=",i4," u(",'//
     &            'i4,",",i4,") = ",e10.2)') grid,i1,i2,v(i1,i2,ivt)
                endif
             endif
          endif
        end do
      end do
c
c mesh spacings
      do kd=1,nd
! *wdh* 050311    hd(kd)=1.0/(nrsab(kd,2)-nrsab(kd,1))
        hdi(kd)=1.0d0/hd(kd)
        hdi2(kd)=.5d0*hdi(kd)
        hdi4(kd)=.25d0*hdi(kd)
      end do
c
c calculate inviscid flux contributions, do r-component first
      do i2=mrsab(2,1),mrsab(2,2)
        do i1=mrsab(1,1)-1,mrsab(1,2)+1
          a11=rx(i1,i2,1,1)*aj(i1,i2)
          a12=rx(i1,i2,1,2)*aj(i1,i2)
          w(i1,ir)= a11*u(i1,i2,m1)+a12*u(i1,i2,m2)
          w(i1,ie)=(a11*v(i1,i2,iv1)+a12*v(i1,i2,iv2))
     &             *(u(i1,i2,ie)+v(i1,i2,ivp))
          w(i1,m1)= a11*(u(i1,i2,m1)*v(i1,i2,iv1)+v(i1,i2,ivp))
     &             +a12* u(i1,i2,m2)*v(i1,i2,iv1)
          w(i1,m2)= a12*(u(i1,i2,m2)*v(i1,i2,iv2)+v(i1,i2,ivp))
     &             +a11* u(i1,i2,m1)*v(i1,i2,iv2)
          w(i1,m3)= a11*(u(i1,i2,m1)*v(i1,i2,iv3)) 
     &             +a12*(u(i1,i2,m2)*v(i1,i2,iv3))
        end do
c
        if( moving.gt.0 )then
          do i1=mrsab(1,1)-1,mrsab(1,2)+1
            ugv=(rx(i1,i2,1,1)*gv(i1,i2,1)
     &          +rx(i1,i2,1,2)*gv(i1,i2,2))*aj(i1,i2)
            aa(i1,1,1)=-ugv
            w(i1,ir)=w(i1,ir)-ugv*u(i1,i2,ir)
            w(i1,ie)=w(i1,ie)-ugv*u(i1,i2,ie)
            w(i1,m1)=w(i1,m1)-ugv*u(i1,i2,m1)
            w(i1,m2)=w(i1,m2)-ugv*u(i1,i2,m2)

            !XXX kkc unimplemented
            w(i1,m3) = 0
            stop 4321

          end do
        end if
          
c
        do j=1,5 !XXX kkc explicit number of components
          do i1=mrsab(1,1),mrsab(1,2)
            ut(i1,i2,j)=-hdi2(1)*(w(i1+1,j)-w(i1-1,j))
c             ut(i1,i2,j) =0d0
          end do
        end do
      end do
c
c now do s-component
      do i1=mrsab(1,1),mrsab(1,2)
        do i2=mrsab(2,1)-1,mrsab(2,2)+1
          a21=rx(i1,i2,2,1)*aj(i1,i2)
          a22=rx(i1,i2,2,2)*aj(i1,i2)
          w(i2,ir)= a21*u(i1,i2,m1)+a22*u(i1,i2,m2)
          w(i2,ie)=(a21*v(i1,i2,iv1)+a22*v(i1,i2,iv2))
     &             *(u(i1,i2,ie)+v(i1,i2,ivp))
          w(i2,m1)= a21*(u(i1,i2,m1)*v(i1,i2,iv1)+v(i1,i2,ivp))
     &            +a22*u(i1,i2,m2)*v(i1,i2,iv1)
          w(i2,m2)= a22*(u(i1,i2,m2)*v(i1,i2,iv2)+v(i1,i2,ivp))
     &            +a21*u(i1,i2,m1)*v(i1,i2,iv2)
          w(i2,m3)= a21*(u(i1,i2,m1)*v(i1,i2,iv3)) +
     &              a22*(u(i1,i2,m2)*v(i1,i2,iv3))
          
        end do
c
        do j=1,5 !XXX kkc explicit number of components
          do i2=mrsab(2,1),mrsab(2,2)

c             if ( i2.eq.mrsab(2,1).and.j.eq.1 ) then 
c                print *,"diag f at ",i1,i2,
c     &  hdi2(2)*(w(i2+1,j)-w(i2-1,j))/aj(i1,i2)
c             end if
            ut(i1,i2,j)=ut(i1,i2,j)-hdi2(2)*(w(i2+1,j)-w(i2-1,j))
          end do
        end do
      end do
c
c free stream correction
      if ( .true. ) then
      do i1=mrsab(1,1),mrsab(1,2)
        do i2=mrsab(2,1),mrsab(2,2)
          fact1=hdi2(1)*(rx(i1+1,i2,1,1)*aj(i1+1,i2)
     *                  -rx(i1-1,i2,1,1)*aj(i1-1,i2))
     *         +hdi2(2)*(rx(i1,i2+1,2,1)*aj(i1,i2+1)
     *                 - rx(i1,i2-1,2,1)*aj(i1,i2-1))
          fact2=hdi2(1)*(rx(i1+1,i2,1,2)*aj(i1+1,i2)
     *                  -rx(i1-1,i2,1,2)*aj(i1-1,i2))
     *         +hdi2(2)*(rx(i1,i2+1,2,2)*aj(i1,i2+1)
     *                 - rx(i1,i2-1,2,2)*aj(i1,i2-1))
          amn=fact1*v(i1,i2,iv1)
     *       +fact2*v(i1,i2,iv2)
          ut(i1,i2,ir)=ut(i1,i2,ir)+amn*u(i1,i2,ir)
          ut(i1,i2,m1)=ut(i1,i2,m1)+amn*u(i1,i2,m1)
     *                           +fact1*v(i1,i2,ivp)
          ut(i1,i2,m2)=ut(i1,i2,m2)+amn*u(i1,i2,m2)
     *                           +fact2*v(i1,i2,ivp)
          ut(i1,i2,m3)=ut(i1,i2,m3)+amn*u(i1,i2,m3)
          ut(i1,i2,ie)=ut(i1,i2,ie)+amn*(u(i1,i2,ie)+v(i1,i2,ivp))
c          print *,"convective ut = ",ut(i1,i2,m3)
c         write(6,246)i1,i2,(ut(i1,i2,i),i=1,4)
c 246     format(2(1x,i3),4(1x,1pe9.2))
        end do
      end do
      endif
c     pause
c
c calculate viscous flux contribution, do r-component first
      if( amu.ne.0. .or. akappa.ne.0. )then
        do i2=mrsab(2,1),mrsab(2,2)
          do i=1,2
            do j=1,2
              do i1=mrsab(1,1)-1,mrsab(1,2)
                ! aa are the metrics*det|J| (jacobian matrix) at i1+1/2
                aa(i1,i,j)=.5d0*(rx(i1  ,i2,i,j)*aj(i1  ,i2)
     &                        +rx(i1+1,i2,i,j)*aj(i1+1,i2))
              end do
            end do
          end do
          ! tmp(:,k,0) is v_k at i+1/2
          ! tmp(:,k,x) is d{v_k}/d{r_x}
          do k=1,4 !XXX kkc
            do i1=mrsab(1,1)-1,mrsab(1,2)
              aja=.5d0*(aj(i1,i2)+aj(i1+1,i2))
c solution at cell center
              tmp(i1,k,0)=.5*(v(i1+1,i2,k)+v(i1,i2,k))
              tmpr=(v(i1+1,i2,k)-v(i1,i2,k))*hdi(1)
              tmps=(v(i1+1,i2+1,k)+v(i1,i2+1,k)
     &             -v(i1+1,i2-1,k)-v(i1,i2-1,k))*hdi4(2)
c (x,y)-derivatives at cell center
              tmp(i1,k,1)=(aa(i1,1,1)*tmpr+aa(i1,1,2)*tmps)/aja
              tmp(i1,k,2)=(aa(i1,2,1)*tmpr+aa(i1,2,2)*tmps)/aja
            end do
          end do
c
          do i1=mrsab(1,1)-1,mrsab(1,2)
c
c temperature dependent viscosties
            amu0=amu*(cmu1+cmu2*abs(tmp(i1,ivt,0))**cmu3)
            amu23=2.*amu0/3.
            akappa0=akappa*(ckap1+ckap2*abs(tmp(i1,ivt,0))**ckap3)
c
c stress tensor and heat flux
            tau(1,1)=amu23*(2.*tmp(i1,1,1)-tmp(i1,2,2))
            tau(1,2)=amu0*(tmp(i1,1,2)+tmp(i1,2,1))
            tau(2,1)=tau(1,2)
            tau(2,2)=amu23*(2.*tmp(i1,2,2)-tmp(i1,1,1))

            if (axiv) then
               tau(3,1)=amu0*tmp(i1,3,1)
               tau(3,2)=amu0*tmp(i1,3,2)
            if ( (irad.eq.2).and.( (i2.eq.irad0 .or. i2.eq.irad1)) )then
c               stop 6543
c     we are sitting on the axis, w/r -> w_r and v/r -> v_r
c     v/r contributions from the divergence
               tau(1,1) = tau(1,1) - amu23*tmp(i1,ivr,ivr)
               tau(2,2) = tau(2,2) - amu23*tmp(i1,ivr,ivr)
c     w/r contribution from strain rate
               tau(3,ivr) = tau(3,ivr) - amu0*tmp(i1,3,ivr)
            else if ( axiv ) then
               rad = .5d0 * (xy(i1+1,i2,2)+xy(i1,i2,2))
               if ( dabs(rad).lt.1e-10 ) then
                  print *,"irad = ",irad," rad = ",rad
                  stop 54321
               endif
c     v/r contributions from the divergence
               tau(1,1) = tau(1,1) - amu23*tmp(i1,ivr,0)/rad
               tau(2,2) = tau(2,2) - amu23*tmp(i1,ivr,0)/rad
c     w/r contribution from strain rate
               tau(3,ivr) = tau(3,ivr) - amu0*tmp(i1,iv3,0)/rad
               tau(3,3)=amu23*(2.*tmp(i1,ivr,0)/rad-
     &                         tmp(i1,ivr,ivr)-tmp(i1,ivz,ivz))
            end if

            else
               tau(3,1) = 0.d0
               tau(3,2) = 0.d0
            endif

            q(1)=-akappa0*tmp(i1,4,1)
            q(2)=-akappa0*tmp(i1,4,2)
c
            w(i1,ie)= aa(i1,1,1)*(tmp(i1,1,0)*tau(1,1)
     &                           +tmp(i1,2,0)*tau(2,1)
     &                           +tmp(i1,3,0)*tau(3,1)-q(1))
     &               +aa(i1,1,2)*(tmp(i1,1,0)*tau(1,2)
     &                           +tmp(i1,2,0)*tau(2,2)
     &                           +tmp(i1,3,0)*tau(3,2)-q(2))
            w(i1,m1)= aa(i1,1,1)*tau(1,1)+aa(i1,1,2)*tau(1,2)
            w(i1,m2)= aa(i1,1,1)*tau(2,1)+aa(i1,1,2)*tau(2,2)
            w(i1,m3)= aa(i1,1,1)*tau(3,1)+aa(i1,1,2)*tau(3,2)

          end do
          do j=2,5 !XXX kkc explicit number of equations here
            do i1=mrsab(1,1),mrsab(1,2)
              ut(i1,i2,j)=ut(i1,i2,j)+hdi(1)*(w(i1,j)-w(i1-1,j)) 
            end do
          end do
        end do
c
c do s-component
        do i1=mrsab(1,1),mrsab(1,2)
          do i=1,2
            do j=1,2
              do i2=mrsab(2,1)-1,mrsab(2,2)
                ! aa are the metrics*det|J| (jacobian matrix) at i2+1/2
                aa(i2,i,j)=.5d0*(rx(i1,i2  ,i,j)*aj(i1,i2)
     &                        +rx(i1,i2+1,i,j)*aj(i1,i2+1))
              end do
            end do
          end do
          do k=1,4 !XXX kkc
            do i2=mrsab(2,1)-1,mrsab(2,2)
              aja=.5d0*(aj(i1,i2)+aj(i1,i2+1))
c solution at cell center
              ! tmp(:,k,0) is v_k at j+1/2
              ! tmp(:,k,x) is d{v_k}/d{r_x}
              tmp(i2,k,0)=.5d0*(v(i1,i2+1,k)+v(i1,i2,k))
              tmps=(v(i1,i2+1,k)-v(i1,i2,k))*hdi(2)
              tmpr=(v(i1+1,i2+1,k)+v(i1+1,i2,k)
     &             -v(i1-1,i2+1,k)-v(i1-1,i2,k))*hdi4(1)
c derivative at cell center
              tmp(i2,k,1)=(aa(i2,1,1)*tmpr+aa(i2,1,2)*tmps)/aja
              tmp(i2,k,2)=(aa(i2,2,1)*tmpr+aa(i2,2,2)*tmps)/aja
            end do
          end do
c
          do i2=mrsab(2,1)-1,mrsab(2,2)
c
c temperature dependent viscosties
            amu0=amu*(cmu1+cmu2*abs(tmp(i2,ivt,0))**cmu3)
            amu23=2.*amu0/3.
            akappa0=akappa*(ckap1+ckap2*abs(tmp(i2,ivt,0))**ckap3)
c
c stress tensor and heat flux
            tau(1,1)=amu23*(2.*tmp(i2,1,1)-tmp(i2,2,2))
            tau(1,2)=amu0*(tmp(i2,1,2)+tmp(i2,2,1))
            tau(2,1)=tau(1,2)
            tau(2,2)=amu23*(2.*tmp(i2,2,2)-tmp(i2,1,1))

            if ( axiv ) then
               tau(3,1)=amu0*tmp(i2,3,1)
               tau(3,2)=amu0*tmp(i2,3,2)
            if ( (irad.eq.1).and.( (i1.eq.irad0 .or. i1.eq.irad1))) then
c               stop 6543
c     we are sitting on the axis, w/r -> w_r and v/r -> v_r
c     v/r contributions from the divergence
               tau(1,1) = tau(1,1) - amu23*tmp(i2,ivr,ivr)
               tau(2,2) = tau(2,2) - amu23*tmp(i2,ivr,ivr)
c     w/r contribution from strain rate
               tau(3,ivr) = tau(3,ivr) - amu0*tmp(i2,3,ivr)
            else if (axiv) then
               rad = .5d0 * (xy(i1,i2+1,2)+xy(i1,i2,2))
               if ( dabs(rad).lt.1e-10 ) then
                  print *,"irad = ",irad," rad = ",rad
                  stop 54321
               endif
c     v/r contributions from the divergence
               tau(1,1) = tau(1,1) - amu23*tmp(i2,ivr,0)/rad
               tau(2,2) = tau(2,2) - amu23*tmp(i2,ivr,0)/rad
c     w/r contribution from strain rate
               tau(3,ivr) = tau(3,ivr) - amu0*tmp(i2,iv3,0)/rad
            endif

            else
               tau(3,1) = 0.d0
               tau(3,2) = 0.d0
            endif

c            print *,"tau(3,1)= ",tau(3,1)
c            print *,"tau(3,2)= ",tau(3,2)
            q(1)=-akappa0*tmp(i2,4,1)
            q(2)=-akappa0*tmp(i2,4,2)
c
            w(i2,ie)=aa(i2,2,1)*(tmp(i2,1,0)*tau(1,1)
     &                          +tmp(i2,2,0)*tau(2,1)
     &                          +tmp(i2,3,0)*tau(3,1)-q(1))
     &              +aa(i2,2,2)*(tmp(i2,1,0)*tau(1,2)
     &                          +tmp(i2,2,0)*tau(2,2)
     &                          +tmp(i2,3,0)*tau(3,2)-q(2))
            w(i2,m1)= aa(i2,2,1)*tau(1,1)+aa(i2,2,2)*tau(1,2)
            w(i2,m2)= aa(i2,2,1)*tau(2,1)+aa(i2,2,2)*tau(2,2)
            w(i2,m3)= aa(i2,2,1)*tau(3,1)+aa(i2,2,2)*tau(3,2)
c            print *,"w(i2,m3)= ",w(i2,m3)
          end do
          do j=2,5 !XXX kkc explicit number of equations
            do i2=mrsab(2,1),mrsab(2,2)
              ut(i1,i2,j)=ut(i1,i2,j)+hdi(2)*(w(i2,j)-w(i2-1,j)) 
            end do
          end do
        end do
      end if


c     add contributions from artificial viscosity
      if( av2.ne.0. .or. av4.ne.0. )then

         if ( .true. ) then
            
            call avjst2d(nd,nc,ndra,ndrb,ndsa,ndsb,nda,ndb,
     &           mrsab,mask,u,xy,v,w,rx,aj,ipu,rpu,addaxiterms,
     &           ut)

         else if ( .false. ) then
            
            call avo2d(nd,nc,ndra,ndrb,ndsa,ndsb,nda,ndb,
     &           mrsab,mask,u,v,w,rx,aj,ipu,rpu,
     &           ut)
            
         else
c     use a simple scalar dissipation
            
            do j=1,5            !XXX  
               do i2=mrsab(2,1)+1,mrsab(2,2)-1
                  do i1=mrsab(1,1)+1,mrsab(1,2)-1
                     ut(i1,i2,j) = ut(i1,i2,j) +av2* ( -4d0*u(i1,i2,j) +
     &              u(i1+1,i2,j)+u(i1-1,i2,j)+u(i1,i2+1,j)+u(i1,i2-1,j))
                  end do
               end do
            end do

         endif
         
      endif

c
c divide by the Jacobian
      do i2=mrsab(2,1),mrsab(2,2)
        do i1=mrsab(1,1),mrsab(1,2)
          ajac=1.0/aj(i1,i2)
          ut(i1,i2,ir) =ut(i1,i2,ir)*ajac
          ut(i1,i2,m1)=ut(i1,i2,m1)*ajac
          ut(i1,i2,m2)=ut(i1,i2,m2)*ajac
          ut(i1,i2,m3)=ut(i1,i2,m3)*ajac
          ut(i1,i2,ie) =ut(i1,i2,ie)*ajac

       end do
      end do 

c     add the axisymmetric forcing terms

      do i2=mrsab(2,1),mrsab(2,2)
c            dmdt=0d0
        do i1=mrsab(1,1),mrsab(1,2)
c     convective part
           if ( ((irad.eq.1).and.(i1.eq.irad0 .or. i1.eq.irad1)).or.
     &         ( (irad.eq.2).and.(i2.eq.irad0 .or. i2.eq.irad1))) then

              tmpr=(v(i1+1,i2,ivr)-v(i1-1,i2,ivr))*hdi2(1)
              tmps=(v(i1,i2+1,ivr)-v(i1,i2-1,ivr))*hdi2(2)
              vr = (rx(i1,i2,2,1)*tmpr+rx(i1,i2,2,2)*tmps)
              tmpr=(v(i1+1,i2,iv3)-v(i1-1,i2,iv3))*hdi2(1)
              tmps=(v(i1,i2+1,iv3)-v(i1,i2-1,iv3))*hdi2(2)
              wr = (rx(i1,i2,2,1)*tmpr+rx(i1,i2,2,2)*tmps)

              ut(i1,i2,ir)  =ut(i1,i2,ir)-  u(i1,i2,ir) *vr
              ut(i1,i2,imz) =ut(i1,i2,imz) -u(i1,i2,imz)*vr
              ut(i1,i2,imr) =ut(i1,i2,imr) -u(i1,i2,imr)*vr 
     &                                     + u(i1,i2,m3)*wr
              ut(i1,i2,m3) =ut(i1,i2,m3) -2d0*u(i1,i2,m3)*vr 
              ut(i1,i2,ie)  =ut(i1,i2,ie) - 
     &             (u(i1,i2,ie) + v(i1,i2,ivp))*vr

          else if ( axic ) then

             rad = xy(i1,i2,2)
             if ( (10.d0+rad).eq.10.d0 ) then
                print *,"small radius = ",rad
                stop 321
             end if

             rad = 1./rad 
             if ( dabs(rad).gt.1e10 ) then
                print *,i1,i2,irad
                print *,xy(i1,i2,2)
                print *,rad
                stop 321
             end if
             

c             print *,i1,i2,v(i1,i2,iv2),rad
             if ( .false. ) then
             ut(i1,i2,ir)  =ut(i1,i2,ir)-  u(i1,i2,ir) *v(i1,i2,iv2)*rad
             ut(i1,i2,m1) =ut(i1,i2,m1) -u(i1,i2,m1)*v(i1,i2,iv2)*rad
             ut(i1,i2,m2) =ut(i1,i2,m2) -u(i1,i2,m2)*v(i1,i2,iv2)*rad
     &                                  +u(i1,i2,m3)*v(i1,i2,iv3)*rad
             ut(i1,i2,m3) =ut(i1,i2,m3)-2d0*u(i1,i2,m3)*v(i1,i2,iv2)*rad
             ut(i1,i2,ie)  =ut(i1,i2,ie) - 
     &                     (u(i1,i2,ie) +v(i1,i2,ivp))*v(i1,i2,iv2)*rad
             else
                aja = 1d0/aj(i1,i2)
                fact1=( (xy(i1+1,i2,2)-xy(i1,i2,2))*
     &                   aj(i1+1,i2)*rx(i1+1,i2,2,1)*u(i1+1,i2,m2) +
     &                   (xy(i1,i2,2)-xy(i1-1,i2,2))*
     &                   aj(i1-1,i2)*rx(i1-1,i2,2,1)*u(i1-1,i2,m2) )*
     &                   hdi2(1)

c                fact2=( (xy(i1,i2+1,2)-xy(i1,i2,2))*
c     &                   aj(i1,i2+1)*rx(i1,i2+1,2,2)*u(i1,i2+1,m2) +
c     &                   (xy(i1,i2,2)-xy(i1,i2-1,2))*
c     &                   aj(i1,i2-1)*rx(i1,i2-1,2,2)*u(i1,i2-1,m2) )*
c     &                   hdi2(2)

                fact2 = 
     & hdi2(2)*(xy(i1,i2+1,2)*aj(i1,i2+1)*rx(i1,i2+1,2,2)*u(i1,i2+1,m2)-
     &          xy(i1,i2-1,2)*aj(i1,i2-1)*rx(i1,i2-1,2,2)*u(i1,i2-1,m2))
     & -hdi2(2)*xy(i1,i2,2)*(aj(i1,i2+1)*rx(i1,i2+1,2,2)*u(i1,i2+1,m2)-
     &                       aj(i1,i2-1)*rx(i1,i2-1,2,2)*u(i1,i2-1,m2))
                   
             ut(i1,i2,ir)  =ut(i1,i2,ir)-  (fact1+fact2)*rad*aja
c             ut(i1,i2,ir) = ut(i1,i2,ir)-rad*aja*fact2 

             do j=m1,m3
                fact1=( (xy(i1+1,i2,2)-xy(i1,i2,2))*
     &                   aj(i1+1,i2)*rx(i1+1,i2,2,1)*
     &                   u(i1+1,i2,j)*v(i1+1,i2,iv2)+
     &                   (xy(i1,i2,2)-xy(i1-1,i2,2))*
     &                   aj(i1-1,i2)*rx(i1-1,i2,2,1)*
     &                 u(i1-1,i2,j)*v(i1-1,i2,iv2 ))*
     &                   hdi2(1)

c                fact2=( (xy(i1,i2+1,2)-xy(i1,i2,2))*
c     &                   aj(i1,i2+1)*rx(i1,i2+1,2,2)*
c     &                   u(i1,i2+1,j)*v(i1,i2+1,iv2)+
c     &                   (xy(i1,i2,2)-xy(i1,i2-1,2))*
c     &                   aj(i1,i2-1)*rx(i1,i2-1,2,2)*
c     &                 u(i1,i2-1,j)*v(i1,i2-1,iv2 ))*
c     &                   hdi2(2)
                fact2 = 
     & hdi2(2)*(xy(i1,i2+1,2)*aj(i1,i2+1)*rx(i1,i2+1,2,2)
     &             *u(i1,i2+1,j)*v(i1,i2+1,iv2)-
     &          xy(i1,i2-1,2)*aj(i1,i2-1)*rx(i1,i2-1,2,2)
     &             *u(i1,i2-1,j)*v(i1,i2-1,iv2))
     & -hdi2(2)*xy(i1,i2,2)*(aj(i1,i2+1)*rx(i1,i2+1,2,2)
     &                        *u(i1,i2+1,j)*v(i1,i2+1,iv2)-
     &                       aj(i1,i2-1)*rx(i1,i2-1,2,2)
     &                        *u(i1,i2-1,j)*v(i1,i2-1,iv2))
                   
                ut(i1,i2,j) = ut(i1,i2,j) - (fact1+fact2)*rad*aja

            if ( j.eq.m3 ) ut(i1,i2,j)=ut(i1,i2,j)-(fact1+fact2)*rad*aja
c             ut(i1,i2,m1) =ut(i1,i2,m1) -u(i1,i2,m1)*v(i1,i2,iv2)*rad
c             ut(i1,i2,m2) =ut(i1,i2,m2) -u(i1,i2,m2)*v(i1,i2,iv2)*rad
c             ut(i1,i2,m3) =ut(i1,i2,m3)-2d0*u(i1,i2,m3)*v(i1,i2,iv2)*rad
             end do

                fact1=( (xy(i1+1,i2,2)-xy(i1,i2,2))*
     &                   aj(i1+1,i2)*rx(i1+1,i2,2,1)*
     &                   u(i1+1,i2,m3)*v(i1+1,i2,iv3)+
     &                   (xy(i1,i2,2)-xy(i1-1,i2,2))*
     &                   aj(i1-1,i2)*rx(i1-1,i2,2,1)*
     &                 u(i1-1,i2,m3)*v(i1-1,i2,iv3 ))*
     &                   hdi2(1)

c                fact2=( (xy(i1,i2+1,2)-xy(i1,i2,2))*
c     &                   aj(i1,i2+1)*rx(i1,i2+1,2,2)*
c     &                   u(i1,i2+1,m3)*v(i1,i2+1,iv3)+
c     &                   (xy(i1,i2,2)-xy(i1,i2-1,2))*
c     &                   aj(i1,i2-1)*rx(i1,i2-1,2,2)*
c     &                 u(i1,i2-1,m3)*v(i1,i2-1,iv3 ))*
c     &                   hdi2(2)
                fact2 = 
     & hdi2(2)*(xy(i1,i2+1,2)*aj(i1,i2+1)*rx(i1,i2+1,2,2)
     &             *u(i1,i2+1,m3)*v(i1,i2+1,iv3)-
     &          xy(i1,i2-1,2)*aj(i1,i2-1)*rx(i1,i2-1,2,2)
     &             *u(i1,i2-1,m3)*v(i1,i2-1,iv3))
     & -hdi2(2)*xy(i1,i2,2)*(aj(i1,i2+1)*rx(i1,i2+1,2,2)
     &                        *u(i1,i2+1,m3)*v(i1,i2+1,iv3)-
     &                       aj(i1,i2-1)*rx(i1,i2-1,2,2)
     &                        *u(i1,i2-1,m3)*v(i1,i2-1,iv3))

                ut(i1,i2,m2) = ut(i1,i2,m2) 
     &                                  +(fact1+fact2)*rad*aja

                fact1=( (xy(i1+1,i2,2)-xy(i1,i2,2))*
     &                   aj(i1+1,i2)*rx(i1+1,i2,2,1)*
     &                   (u(i1+1,i2,ie) +v(i1+1,i2,ivp))*v(i1+1,i2,iv2)+
     &                   (xy(i1,i2,2)-xy(i1-1,i2,2))*
     &                   aj(i1-1,i2)*rx(i1-1,i2,2,1)*
     &                 (u(i1-1,i2,ie) +v(i1-1,i2,ivp))*v(i1-1,i2,iv2 ))*
     &                   hdi2(1)

c                fact2=( (xy(i1,i2+1,2)-xy(i1,i2,2))*
c     &                   aj(i1,i2+1)*rx(i1,i2+1,2,2)*
c     &                   (u(i1,i2+1,ie) +v(i1,i2+1,ivp))*v(i1,i2+1,iv2)+
c     &                   (xy(i1,i2,2)-xy(i1,i2-1,2))*
c     &                   aj(i1,i2-1)*rx(i1,i2-1,2,2)*
c     &                 (u(i1,i2-1,ie) +v(i1,i2-1,ivp))*v(i1,i2-1,iv2 ))*
c     &                   hdi2(2)
                r1 = u(i1,i2+1,ie) +v(i1,i2+1,ivp)
                r2 = u(i1,i2-1,ie) +v(i1,i2-1,ivp)
                fact2 = 
     & hdi2(2)*(xy(i1,i2+1,2)*aj(i1,i2+1)*rx(i1,i2+1,2,2)
     &             *r1*v(i1,i2+1,iv2)-
     &          xy(i1,i2-1,2)*aj(i1,i2-1)*rx(i1,i2-1,2,2)
     &             *r2*v(i1,i2-1,iv2))
     & -hdi2(2)*xy(i1,i2,2)*(aj(i1,i2+1)*rx(i1,i2+1,2,2)
     &                        *r1*v(i1,i2+1,iv2)-
     &                       aj(i1,i2-1)*rx(i1,i2-1,2,2)
     &                        *r2*v(i1,i2-1,iv2))

             ut(i1,i2,ie)  =ut(i1,i2,ie) - 
     &                      (fact1+fact2)*rad*aja
c     &                     (u(i1,i2,ie) +v(i1,i2,ivp))*v(i1,i2,iv2)*rad


             end if

          endif

c     viscous part (only if needed)
          if( axiv .and.(amu.ne.0. .or. akappa.ne.0.) )then
c temperature dependent viscosties
             amu0=amu*(cmu1+cmu2*abs(v(i1,i2,ivt))**cmu3)
             amu23=2.*amu0/3.
             akappa0=akappa*(ckap1+ckap2*abs(v(i1,i2,ivt))**ckap3)
             
             if (((irad.eq.1).and.(i1.eq.irad0 .or. i1.eq.irad1)).or.
     &           ( (irad.eq.2).and.(i2.eq.irad0 .or. i2.eq.irad1))) then

c     we need some 2nd and cross derivatives here
c                stop 6543
                tmpr = (v(i1+1,i2,ivt)-v(i1,i2,ivt))*hdi(1)
                tmps = (v(i1+1,i2+1,ivt)-v(i1+1,i2-1,ivt) +
     &                  v(i1,i2+1,ivt)-v(i1,i2-1,ivt))*hdi4(2)
                rxa = .5d0*(rx(i1,i2,1,2)*aj(i1,i2)+
     &                      rx(i1+1,i2,1,2)*aj(i1+1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1+1,i2))
                a11 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2,2,2)*aj(i1,i2)+
     &                      rx(i1+1,i2,2,2)*aj(i1+1,i2))
                a11 = (a11 + rxa*tmps)/aja ! t_r at i+1/2

                tmpr = (v(i1,i2,ivt)-v(i1-1,i2,ivt))*hdi(1)
                tmps = (v(i1,i2+1,ivt)-v(i1,i2-1,ivt) +
     &                  v(i1-1,i2+1,ivt)-v(i1-1,i2-1,ivt))*hdi4(2)
                rxa = .5d0*(rx(i1,i2,1,2)*aj(i1,i2)+
     &                      rx(i1-1,i2,1,2)*aj(i1-1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1-1,i2))
                a12 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2,2,2)*aj(i1,i2)+
     &                      rx(i1-1,i2,2,2)*aj(i1-1,i2))
                a12 = (a12 + rxa*tmps)/aja ! t_r at i-1/2

                tmpr = (v(i1+1,i2,ivt)-v(i1-1,i2,ivt) +
     &                  v(i1+1,i2+1,ivt)-v(i1-1,i2+1,ivt))*hdi4(1)
                tmps = (v(i1,i2+1,ivt)-v(i1,i2,ivt))*hdi(2)
                rxa = .5d0*(rx(i1,i2+1,1,2)*aj(i1,i2+1)+
     &                      rx(i1,i2,1,2)*aj(i1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1,i2+1))
                a21 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2+1,2,2)*aj(i1,i2+1)+
     &                      rx(i1,i2,2,2)*aj(i1,i2))
                a21 = (a21 + rxa*tmps)/aja ! t_r at j+1/2

                tmpr = (v(i1+1,i2,ivt)-v(i1-1,i2,ivt) +
     &                  v(i1+1,i2-1,ivt)-v(i1-1,i2-1,ivt))*hdi4(1)
                tmps = (v(i1,i2,ivt)-v(i1,i2-1,ivt))*hdi(2)
                rxa = .5d0*(rx(i1,i2-1,1,2)*aj(i1,i2-1)+
     &                      rx(i1,i2,1,2)*aj(i1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1,i2-1))
                a22 = rxa*tmpr ! t_r at j-1/2
                rxa = .5d0*(rx(i1,i2-1,2,2)*aj(i1,i2-1)+
     &                      rx(i1,i2,2,2)*aj(i1,i2))
                a22 = (a22 + rxa*tmps)/aja

                tmpr = (a11-a12)*hdi(1)
                tmps = (a21-a22)*hdi(2)
                tror = (rx(i1,i2,1,2)*tmpr+rx(i1,i2,2,2)*tmps) ! T_r/r -> T_{rr}


                tmpr = (v(i1+1,i2,ivr)-v(i1,i2,ivr))*hdi(1)
                tmps = (v(i1+1,i2+1,ivr)-v(i1+1,i2-1,ivr) +
     &                  v(i1,i2+1,ivr)-v(i1,i2-1,ivr))*hdi4(2)
                rxa = .5d0*(rx(i1,i2,1,2)*aj(i1,i2)+
     &                      rx(i1+1,i2,1,2)*aj(i1+1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1+1,i2))
                a11 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2,2,2)*aj(i1,i2)+
     &                      rx(i1+1,i2,2,2)*aj(i1+1,i2))
                a11 = (a11 + rxa*tmps)/aja ! v_r at i+1/2

                tmpr = (v(i1,i2,ivr)-v(i1-1,i2,ivr))*hdi(1)
                tmps = (v(i1,i2+1,ivr)-v(i1,i2-1,ivr) +
     &                  v(i1-1,i2+1,ivr)-v(i1-1,i2-1,ivr))*hdi4(2)
                rxa = .5d0*(rx(i1,i2,1,2)*aj(i1,i2)+
     &                      rx(i1-1,i2,1,2)*aj(i1-1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1-1,i2))
                a12 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2,2,2)*aj(i1,i2)+
     &                      rx(i1-1,i2,2,2)*aj(i1-1,i2))
                a12 = (a12 + rxa*tmps)/aja ! v_r at i-1/2

                tmpr = (v(i1+1,i2,ivr)-v(i1-1,i2,ivr) +
     &                  v(i1+1,i2+1,ivr)-v(i1-1,i2+1,ivr))*hdi4(1)
                tmps = (v(i1,i2+1,ivr)-v(i1,i2,ivr))*hdi(2)
                rxa = .5d0*(rx(i1,i2+1,1,2)*aj(i1,i2+1)+
     &                      rx(i1,i2,1,2)*aj(i1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1,i2+1))
                a21 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2+1,2,2)*aj(i1,i2+1)+
     &                      rx(i1,i2,2,2)*aj(i1,i2))
                a21 = (a21 + rxa*tmps)/aja ! v_r at j+1/2

                tmpr = (v(i1+1,i2,ivr)-v(i1-1,i2,ivr) +
     &                  v(i1+1,i2-1,ivr)-v(i1-1,i2-1,ivr))*hdi4(1)
                tmps = (v(i1,i2,ivr)-v(i1,i2-1,ivr))*hdi(2)
                rxa = .5d0*(rx(i1,i2-1,1,2)*aj(i1,i2-1)+
     &                      rx(i1,i2,1,2)*aj(i1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1,i2-1))
                a22 = rxa*tmpr ! v_r at j-1/2
                rxa = .5d0*(rx(i1,i2-1,2,2)*aj(i1,i2-1)+
     &                      rx(i1,i2,2,2)*aj(i1,i2))
                a22 = (a22 + rxa*tmps)/aja

                tmpr = (a11-a12)*hdi(1)
                tmps = (a21-a22)*hdi(2)
                vror = (rx(i1,i2,1,2)*tmpr+rx(i1,i2,2,2)*tmps) ! v_r/r -> v_{rr}

                tmpr = (v(i1+1,i2,ivz)-v(i1,i2,ivz))*hdi(1)
                tmps = (v(i1+1,i2+1,ivz)-v(i1+1,i2-1,ivz) +
     &                  v(i1,i2+1,ivz)-v(i1,i2-1,ivz))*hdi4(2)
                rxa = .5d0*(rx(i1,i2,1,2)*aj(i1,i2)+
     &                      rx(i1+1,i2,1,2)*aj(i1+1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1+1,i2))
                a11 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2,2,2)*aj(i1,i2)+
     &                      rx(i1+1,i2,2,2)*aj(i1+1,i2))
                a11 = (a11 + rxa*tmps)/aja ! u_r at i+1/2

                tmpr = (v(i1,i2,ivz)-v(i1-1,i2,ivz))*hdi(1)
                tmps = (v(i1,i2+1,ivz)-v(i1,i2-1,ivz) +
     &                  v(i1-1,i2+1,ivz)-v(i1-1,i2-1,ivz))*hdi4(2)
                rxa = .5d0*(rx(i1,i2,1,2)*aj(i1,i2)+
     &                      rx(i1-1,i2,1,2)*aj(i1-1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1-1,i2))
                a12 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2,2,2)*aj(i1,i2)+
     &                      rx(i1-1,i2,2,2)*aj(i1-1,i2))
                a12 = (a12 + rxa*tmps)/aja ! u_r at i-1/2

                tmpr = (v(i1+1,i2,ivz)-v(i1-1,i2,ivz) +
     &                  v(i1+1,i2+1,ivz)-v(i1-1,i2+1,ivz))*hdi4(1)
                tmps = (v(i1,i2+1,ivz)-v(i1,i2,ivz))*hdi(2)
                rxa = .5d0*(rx(i1,i2+1,1,2)*aj(i1,i2+1)+
     &                      rx(i1,i2,1,2)*aj(i1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1,i2+1))
                a21 = rxa*tmpr
                rxa = .5d0*(rx(i1,i2+1,2,2)*aj(i1,i2+1)+
     &                      rx(i1,i2,2,2)*aj(i1,i2))
                a21 = (a21 + rxa*tmps)/aja ! u_r at j+1/2

                tmpr = (v(i1+1,i2,ivz)-v(i1-1,i2,ivz) +
     &                  v(i1+1,i2-1,ivz)-v(i1-1,i2-1,ivz))*hdi4(1)
                tmps = (v(i1,i2,ivz)-v(i1,i2-1,ivz))*hdi(2)
                rxa = .5d0*(rx(i1,i2-1,1,2)*aj(i1,i2-1)+
     &                      rx(i1,i2,1,2)*aj(i1,i2))
                aja = .5d0*(aj(i1,i2) + aj(i1,i2-1))
                a22 = rxa*tmpr ! u_r at j-1/2
                rxa = .5d0*(rx(i1,i2-1,2,2)*aj(i1,i2-1)+
     &                      rx(i1,i2,2,2)*aj(i1,i2))
                a22 = (a22 + rxa*tmps)/aja

                tmpr = (a11-a12)*hdi(1)
                tmps = (a21-a22)*hdi(2)
                uror = (rx(i1,i2,1,2)*tmpr+rx(i1,i2,2,2)*tmps) ! u_r/r -> u_{rr}

                vzor = (rx(i1,i2,1,1)*tmpr+rx(i1,i2,2,1)*tmps) ! v_z/r -> v_{rz}

c      this is a funny one, double check it
                uzor = 0
             
                tmpr=(v(i1+1,i2,ivr)-v(i1-1,i2,ivr))*hdi2(1)
                tmps=(v(i1,i2+1,ivr)-v(i1,i2-1,ivr))*hdi2(2)
                vr = (rx(i1,i2,1,2)*tmpr+rx(i1,i2,2,2)*tmps)

                   ut(i1,i2,imz) =ut(i1,i2,imz) + 
     &                   amu0*( vzor + uror )

                   ut(i1,i2,imr) =ut(i1,i2,imr) +
     &                   amu23*(2*vror-uzor-vr)

c                   ut(i1,i2,m3) = correction cancels out here

c              ok really check this one
                ut(i1,i2,ie)  =ut(i1,i2,ie) +
     &                  akappa0*tror + 
     &                  amu0*( v(i1,i2,ivz)*( vzor+uror ) +
     &                         2*v(i1,i2,ivr)*vror) -
     &                  amu23*( uzor + vror+vr)

             else

                rad = xy(i1,i2,2)
                if ( (10.d0+rad).eq.10.d0 ) then
                   stop 21
                end if
                
                rad = 1./rad    

                do k=1,4 !XXX
                   tmpr=(v(i1+1,i2,k)-v(i1-1,i2,k))*hdi2(1)
                   tmps=(v(i1,i2+1,k)-v(i1,i2-1,k))*hdi2(2)
                   tmp(1,k,ivz) =(rx(i1,i2,1,1)*tmpr+rx(i1,i2,1,2)*tmps)
                   tmp(1,k,ivr) =(rx(i1,i2,2,1)*tmpr+rx(i1,i2,2,2)*tmps)
c                   if ( k.eq.4)        print *,i1,i2,tmps,tmp(1,ivt,ivr)
                end do

                ut(i1,i2,imz) =ut(i1,i2,imz) + 
     &               amu0*(tmp(1,ivr,ivz)+tmp(1,ivz,ivr))*rad

                ut(i1,i2,imr) =ut(i1,i2,imr) +
     &                amu23*(2.*tmp(1,ivr,ivr)- tmp(1,ivz,ivz)
     &                       - v(i1,i2,ivr)*rad
     &                       -2.*v(1,i2,ivr)*rad + tmp(1,ivr,ivr) 
     &                                  + tmp(1,ivz,ivz))*rad

                ut(i1,i2,m3) =ut(i1,i2,m3) + 
     &                2.*amu0*(tmp(1,iv3,2)-v(i1,i2,iv3)*rad)*rad
c                print *,i1,i2,tmp(1,iv3,ivr),v(i1,i2,iv3)*rad !v(i1,i2,iv3)
!                print *,"   ",rad,v(i1,i2,iv3)*rad

                ut(i1,i2,ie)  =ut(i1,i2,ie) +
     &                akappa0*tmp(1,ivt,ivr)*rad +
     &             (amu0*( v(i1,i2,ivz)*(tmp(1,ivz,ivr)+tmp(1,ivr,ivz))+
     &                       2d0*v(i1,i2,ivr)*tmp(1,ivr,ivr) +
     &                v(i1,i2,iv3)*(tmp(1,iv3,ivr)-v(i1,i2,iv3)*rad)) -
     &                v(i1,i2,ivr)*
     &           amu23*(tmp(1,ivz,ivz)+tmp(1,ivr,ivr)+v(i1,i2,ivr)*rad))
     &           *rad
C                    ut(i1,i2,ie) = ut(i1,i2,ie) +
C      &                  akappa0*tmp(1,ivt,ivr)*rad +
C      &                  amu0*v(i1,i2,ivr)*rad* 
C      &                  (tmp(1,ivr,ivz) + tmp(1,ivz,ivr) +
C      &                   tmp(1,iv3,ivr) - v(i1,i2,iv3)*rad) +
C      &                  amu23*v(i1,i2,ivr)*rad*
C      &                (2*tmp(1,ivr,ivr)-tmp(1,ivz,ivz)-v(i1,i2,ivr)*rad)


             endif
          end if ! if viscous or thermal

       end do !i1
      end do !i2

c      print *,"column sum = ",dmdt
      if ( .false. ) then
      dmdt=0d0
      dpdt=0d0
      dedt=0d0
        do i1=mrsab(1,1),mrsab(1,2)
         ajac=0d0 ! col or row sum for mass 
      do i2=mrsab(2,1),mrsab(2,2)
           fact1 = 1d0
           if ( i1.eq.mrsab(1,1) ) fact1=.5d0
           if ( i1.eq.mrsab(1,2) ) fact1=fact1*.5d0
           if ( i2.eq.mrsab(2,1) ) fact1=fact1*.5d0
           if ( i2.eq.mrsab(2,2) ) fact1=fact1*.5d0
           fact1=fact1*xy(i1,i2,2)
           dmdt = dmdt + fact1*aj(i1,i2)*ut(i1,i2,ir)*hd(1)*hd(2)
           dedt = dedt + fact1*aj(i1,i2)*ut(i1,i2,ie)*hd(1)*hd(2)
           dpdt = dpdt + fact1*aj(i1,i2)*hd(1)*hd(2)*(
     &           ut(i1,i2,m1)+ut(i1,i2,m2)+ut(i1,i2,m2))
c           print *,"    ",fact1*aj(i1,i2)*ut(i1,i2,ir)*hd(1)*hd(2)
           ajac = ajac+fact1*aj(i1,i2)*ut(i1,i2,ir)*hd(1)*hd(2)
c           if ( i2.eq.0 ) then
c              print *,"0 : ",fact1*aj(i1,i2)*ut(i1,i2,ir)*hd(1)*hd(2)
c           else if (i2.eq.9) then
c              print *,"9 : ",fact1*aj(i1,i2)*ut(i1,i2,ir)*hd(1)*hd(2)
c           endif
        end do
c       print *,"dM/dt row (",i2,") = ",ajac
c       print *,"dM/dt col (",i1,") = ",ajac
      end do
      print *,"dM/dt = ",dmdt
      print *,"dE/dt = ",dedt
      print *,"dP/dt = ",dpdt
      endif

c         write(6,246)i1,i2,(ut(i1,i2,i),i=1,4)
c 246     format(2(1x,i3),4(1x,1pe9.2))
c     pause
c
c add forcing to ut
c**      call cnsfn22 (t,nd,ndra,ndrb,ndsa,ndsb,mrsab,mask,ut,xy)
c
      return
      end

      subroutine avo2d(nd,nc,ndra,ndrb,ndsa,ndsb,nda,ndb,
     &                 mrsab,mask,u,v,w,rx,aj,ipu,rpu,
     &                 ut)
c=============================================================================
c
c     avo2d : calculates the original artifical viscosity used in cnsdu22 and cnsdu22a
c     kkc 051213 : Initial version 
c
c     XXX this only works for cnsdu22a right now since the number of components is hard-coded
c
c=============================================================================
c     anonymous uiuc aae prof, circa 1994: 
c                    "use of explicit type declarations 
c                            is a sign of programmer weakness"
c
c
c     so i'm weak, kkc
      implicit none

c     INPUT
      integer nd,nc,ndra,ndrb,ndsa,ndsb,nda,ndb
      integer mrsab(2,2),ipu(*),mask(ndra:ndrb,ndsa:ndsb)
      real u(ndra:ndrb,ndsa:ndsb,nc),
     &     rx(ndra:ndrb,ndsa:ndsb,2,2),
     &     aj(ndra:ndrb,ndsa:ndsb)
      real v(ndra:ndrb,ndsa:ndsb,5)
      real rpu(*)

c     WORKSPACE
      real w(nda:ndb,5)

c     OUTPUT
      real ut(ndra:ndrb,ndsa:ndsb,*)

c     LOCAL
      integer i1,i2,j,ir,m1,m2,m3,ie,iv1,iv2,iv3,ivt,ivp,imr,imz,ivr,ivz
      integer grid
      real av2,aw2,av4,aw4,a1,a2,v1,v2,v3,vn,c,dist,csq,gamma
      real hd(2),hdi(2),alam,wmax,t,dt

c     STATEMENT FUNCTIONS
c extrapolation formula
      real ue
      integer is1,is2
      ue(i1,i2,is1,is2,j)= 3.0*u(i1-is1  ,i2-is2  ,j)
     &                           -3.0*u(i1-2*is1,i2-2*is2,j)
     &                               +u(i1-3*is1,i2-3*is2,j)


c     The following code was cut and pasted from cnsdu22/cnsdu22a
      grid = ipu( 1)

      gamma =rpu( 3)
      av2   =rpu( 4)
      aw2   =rpu( 5)
      av4   =rpu( 6)
      aw4   =rpu( 7)

      hd(1) = rpu(13)           ! *wdh* 050311 -- change for parallel version
      hd(2) = rpu(14) 
      hdi(1) = 1d0/hd(1)
      hdi(2) = 1d0/hd(2)

      t     = rpu(16)
      dt    = rpu(17)

      ir=1
      m1=2
      m2=3
      m3=4                      !kkc
      ie=5                      !kkc 
      
      iv1=1                     ! z velocity
      iv2=2                     ! r velocity
      iv3=3                     ! theta velocity
      ivt=4                     ! p/rho
      ivp=5                     ! p
      
      imr = m2                  ! r momentum
      imz = m1                  ! z momentum
      ivr = iv2                 ! r velocity
      ivz = iv1                 ! z velocity

c
c calculate artificial viscosity (2nd and 4th order, r-component)
      if( av2.ne.0. .or. av4.ne.0. )then
        do i2=mrsab(2,1),mrsab(2,2)
          do i1=mrsab(1,1),mrsab(1,2)
            w(i1,1)=abs(
     &         (v(i1+1,i2,ivp)-2.0*v(i1,i2,ivp)+v(i1-1,i2,ivp))
     &        /(v(i1+1,i2,ivp)+2.0*v(i1,i2,ivp)+v(i1-1,i2,ivp)))

          end do
          if ( nc.gt.5 ) then
             do i1=mrsab(1,1),mrsab(1,2)
                u(i1,i2,6) = w(i1,1)
                ut(i1,i2,6) = 0
             end do
          end if
             
          ! *wdh* 050323 --- fix this for parallel ---
          i1=mrsab(1,1)-1
          w(i1  ,1)=0.
          w(i1-1,1)=0.
          i1=mrsab(1,2)+1
          w(i1  ,1)=0.
          w(i1+1,1)=0.
          do i1=mrsab(1,1)-1,mrsab(1,2)
            a1=.5*(rx(i1,i2,1,1)*aj(i1,i2)+rx(i1+1,i2,1,1)*aj(i1+1,i2))
            a2=.5*(rx(i1,i2,1,2)*aj(i1,i2)+rx(i1+1,i2,1,2)*aj(i1+1,i2))
            v1=.5*(v(i1,i2,iv1)+v(i1+1,i2,iv1))
            v2=.5*(v(i1,i2,iv2)+v(i1+1,i2,iv2))
            v3=.5*(v(i1,i2,iv3)+v(i1+1,i2,iv3))
            dist=sqrt(a1**2+a2**2)
            vn=a1*v1+a2*v2
c **** why doesn't the next line have a .5* ... ????
c*wdh 981110           csq=gamma*(v(i1,i2,3)+v(i1+1,i2,3))
            csq=.5*gamma*(v(i1,i2,ivt)+v(i1+1,i2,ivt))
c..Bill: why is this here? Do you remember? Was there a problem with
c        gamma*p/rho being negative?
c*** fix this?
            if( csq.lt.0. )then
              if( mask(i1,i2).gt.0 )then
                write(*,9000) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
     &            u(i1,i2,3)
 9000           format('cnsdu22: csq<0, mask>0 i1=',i3,' i2=',i3,
     &           ' csq=',e8.2,' rho=',f5.2,' p=',f6.2,' e=',f6.2,
     &           ' (u,v)=',2f5.2,' (m1,m2)=',2f5.2)
                write(*,9020) grid,t,dt,ndra,ndrb,ndsa,ndsb,mrsab(1,1),
     &            mrsab(1,2),mrsab(2,1),mrsab(2,2)
 9020  format('  grid=',i3,' t=',f6.3,' dt=',e8.2,' ndra,...=',4i5,
     &        ' mrsab=',4i5)

              end if
              c=0.
            else
c$$$             if( mask(i1,i2).gt.0 )then
c$$$                write(*,9010) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
c$$$     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
c$$$     &            u(i1,i2,3),gv(i1,i2,1),gv(i1,i2,2)
c$$$ 9010           format('CNSDT: csq>0, mask>0 i1+=',i3,' i2=',i3,
c$$$     &           ' csq=',e8.2,' rho=',f5.2,' p=',f6.2,' e=',f6.2,
c$$$     &           ' (u,v)=',2f5.2,' (m1,m2)=',2f5.2,' gv=',2f5.2)
c$$$              end if
              c=sqrt(csq)
            end if
            alam=(abs(vn)+c*dist)*hdi(1)
            wmax=max(w(i1-1,1),w(i1,1),w(i1+1,1),w(i1+2,1))
            w(i1,2)=av2*alam*min(1.0,wmax/aw2)
            w(i1,3)=av4*alam*max(0.,1.0-wmax/aw4)
c*wdh            if (wmax.gt.1.e-6) then
c*wdh              w(i1,3)=av4*alam*max(0.,1.0-wmax/aw4)
c*wdh            else
c*wdh              w(i1,3)=0.
c*wdh            end if
          end do
          do j=1,5 !XXX kkc explicit number of equations
            do i1=mrsab(1,1)-1,mrsab(1,2)
              w(i1,1)=w(i1,2)*(u(i1+1,i2,j)-u(i1,i2,j))
            end do
            ! *wdh* 050323 -- check this too ---
            i1=mrsab(1,1)-2
            w(i1,4)=ue(i1,i2,-1,0,j)
            do i1=mrsab(1,1)-1,mrsab(1,2)+1
              w(i1,4)=u(i1,i2,j)
            end do
            i1=mrsab(1,2)+2
            w(i1,4)=ue(i1,i2,+1,0,j)
            do i1=mrsab(1,1)-1,mrsab(1,2)
              w(i1,1)=w(i1,1)-w(i1,3)*(w(i1+2,4)
     &                -3.0*(w(i1+1,4)-w(i1,4))-w(i1-1,4))
            end do
ckkc MASS FLUX FIX XXX
            w(mrsab(1,1)-1,1) = -w(mrsab(1,1),1)
            w(mrsab(1,2),1) = -w(mrsab(1,2)-1,1)

            do i1=mrsab(1,1),mrsab(1,2)
              ut(i1,i2,j)=ut(i1,i2,j)+(w(i1,1)-w(i1-1,1))
            end do
            if ( nc.gt.5 ) then
               do i1=mrsab(1,1),mrsab(1,2)
                  u(i1,i2,6+j) = u(i1,i2,j)!(w(i1,1)-w(i1-1,1))
                  ut(i1,i2,6+j) = 0
               end do
            end if
          end do
        end do
c
c calculate artificial viscosity (2nd and 4th order, s-component)
        do i1=mrsab(1,1),mrsab(1,2)
          do i2=mrsab(2,1),mrsab(2,2)
            w(i2,1)=abs(
     &        (v(i1,i2+1,ivp)-2.0*v(i1,i2,ivp)+v(i1,i2-1,ivp))
     &       /(v(i1,i2+1,ivp)+2.0*v(i1,i2,ivp)+v(i1,i2-1,ivp)))

          end do

          ! *wdh* 050323 --- fix this for parallel ---
          i2=mrsab(2,1)-1
          w(i2  ,1)=0.
          w(i2-1,1)=0.
          i2=mrsab(2,2)+1
          w(i2  ,1)=0.
          w(i2+1,1)=0.
          if ( nc.gt.5 ) then
             do i2=mrsab(2,1)-2,mrsab(2,2)+2
                u(i1,i2,12) = w(i2,1)
                ut(i1,i2,12) = 0
             end do
          end if
          do i2=mrsab(2,1)-1,mrsab(2,2)
            a1=.5*(rx(i1,i2,2,1)*aj(i1,i2)+rx(i1,i2+1,2,1)*aj(i1,i2+1))
            a2=.5*(rx(i1,i2,2,2)*aj(i1,i2)+rx(i1,i2+1,2,2)*aj(i1,i2+1))
            dist=sqrt(a1**2+a2**2)
            v1=.5*(v(i1,i2,iv1)+v(i1,i2+1,iv1))
            v2=.5*(v(i1,i2,iv2)+v(i1,i2+1,iv2))
            v3=.5*(v(i1,i2,iv3)+v(i1,i2+1,iv3))
            vn=a1*v1+a2*v2
            csq=.5*gamma*(v(i1,i2,ivt)+v(i1,i2+1,ivt))
c..Bill: same comment as above
            if( csq.lt.0. )then
              if( mask(i1,i2).gt.0 )then
                write(*,9000) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
     &            u(i1,i2,3)
                write(*,9020) grid,t,dt,ndra,ndrb,ndsa,ndsb,mrsab(1,1),
     &            mrsab(1,2),mrsab(2,1),mrsab(2,2)
              end if
              c=0.
            else
c$$$             if( mask(i1,i2).gt.0 )then
c$$$                write(*,9010) i1,i2,csq,u(i1,i2,ir),v(i1,i2,4),
c$$$     &            u(i1,i2,ie),v(i1,i2,1),v(i1,i2,2),u(i1,i2,2),
c$$$     &            u(i1,i2,3),gv(i1,i2,1),gv(i1,i2,2)
c$$$              end if
              c=sqrt(csq)
            end if
            alam=(abs(vn)+c*dist)*hdi(2)
            wmax=max(w(i2-1,1),w(i2,1),w(i2+1,1),w(i2+2,1))

            w(i2,2)=av2*alam*min(1.0,wmax/aw2)
            w(i2,3)=av4*alam*max(0.,1.0-wmax/aw4)

ckkc            if ( i1.eq.11 ) then
ckkc               write (*,'(i4,2x,6(e12.7,2x))')
ckkc     &         i2,w(i2,2),w(i2,3),alam,c,vn,hdi(2)
ckkc            endif
            
c*wdh            if (wmax.gt.1.e-6) then
c*wdh              w(i2,3)=av4*alam*max(0.,1.0-wmax/aw4)
c*wdh            else
c*wdh              w(i2,3)=0.
c*wdh            end if
          end do
          do j=1,5 !XXX kkc explicit number of equations
            do i2=mrsab(2,1)-1,mrsab(2,2)
              w(i2,1)=w(i2,2)*(u(i1,i2+1,j)-u(i1,i2,j))
            end do
            i2=mrsab(2,1)-2
            w(i2,4)=ue(i1,i2,0,-1,j)
            do i2=mrsab(2,1)-1,mrsab(2,2)+1
              w(i2,4)=u(i1,i2,j)
            end do
            i2=mrsab(2,2)+2
            w(i2,4)=ue(i1,i2,0,+1,j)
            do i2=mrsab(2,1)-1,mrsab(2,2)
              w(i2,1)=w(i2,1)-w(i2,3)*(w(i2+2,4)
     &              -3.0*(w(i2+1,4)-w(i2,4))-w(i2-1,4))
            end do

ckkc MASS FLUX FIX XXX
            w(mrsab(2,1)-1,1) = -w(mrsab(2,1),1)
            w(mrsab(2,2),1) = -w(mrsab(2,2)-1,1)

            do i2=mrsab(2,1),mrsab(2,2)
              ut(i1,i2,j)=ut(i1,i2,j)+(w(i2,1)-w(i2-1,1))
            end do
            if ( nc.gt.5 ) then
               do i2=mrsab(2,1),mrsab(2,2)
                  u(i1,i2,12+j) = (w(i2,1)-w(i2-1,1))
                  ut(i1,i2,12+j) = 0
               end do
            end if          
         end do
        end do
      end if

      return 
      end

      subroutine avjst2d(nd,nc,ndra,ndrb,ndsa,ndsb,nda,ndb,
     &                   mrsab,mask,u,xy,v,w,rx,aj,ipu,rpu,isaxi,
     &                   ut)
c=============================================================================
c
c     avjst2d : calculates a simple Jameson, Schmidt, and Turkel scalar dissipation

c     kkc 051213 : Initial version 
c
c     Notes :- Most of this scheme comes from a modern version specified in
c              Jameson, Antony, "Analysis and Design of Numerical Schemes for Gas Dynamics 1
c                               Artificial Diffusion, Upwind Biasing, Limiters and Their Effects
c                               on Accuracy and Multigrid Convergence", 1995, available on Jameson's
c                               website.
c
c            - also added a grid scaling for high-aspect ratio zones found in 
c              Swanson, Radespiel, and Turkel, "Comparison of Several Dissipation Algorithms for 
c                                              Central Difference Schemes", also from the web
c            - but this scaling probably goes back to whenever Jameson and Martinelli started doing
c               viscous calculations in the late 80s (and sits in some AIAA paper)
c            - the solution values in the ghost boundaries are used as set by the BCs, no
c              extrapolation is done as in avo2d 
c            
c            - 060308 this subroutine could be called from a primitive variable CNS code with both
c              u and v as in primitive variable form.  The dissipation would then be applied to 
c              rho, u, v, w and T (or e).
c
c     XXX this only works for cnsdu22a right now since the number of components is hard-coded
c
c=============================================================================
c     anonymous uiuc aae prof, circa 1994: 
c                    "use of explicit type declarations 
c                            is a sign of programmer weakness"
c
c
c     so i'm weak, kkc
      implicit none

c     INPUT
      integer nd,nc,ndra,ndrb,ndsa,ndsb,nda,ndb
      integer mrsab(2,2),ipu(*),mask(ndra:ndrb,ndsa:ndsb)
      real u(ndra:ndrb,ndsa:ndsb,nc),
     &     rx(ndra:ndrb,ndsa:ndsb,2,2),
     &     aj(ndra:ndrb,ndsa:ndsb)
      real xy(ndra:ndrb,ndsa:ndsb,2) 
      real v(ndra:ndrb,ndsa:ndsb,nc)
      real rpu(*)
      logical isaxi

c     WORKSPACE
      real w(nda:ndb,nc)

c     OUTPUT
      real ut(ndra:ndrb,ndsa:ndsb,nc)

c     LOCAL
      integer i1,i2,j,ir,m1,m2,m3,ie,iv1,iv2,iv3,ivt,ivp,imr,imz,ivr,ivz
      integer grid,irad,irad0,irad1
      real av2,aw2,av4,aw4,a1,a2,v1,v2,v3,vn,c,dist,csq,gamma,sm
      real hd(2),hdi(2),alam1,alam2,wmax,t,dt,eps,eps1,eps2,psi
      real r1,r2

c     LOCAL PARAMETERS
c     q is the exponent on the (dis)continuity sensor
c     bt is the exponent on the stretched-grid scaling
c     epsr is the scaling for the epsilon in the sensor's denominator
      real q,bt,epsr
      parameter(q=1d0,bt=.6d0,epsr=1e-20)

c     STATEMENT FUNCTIONS
      real rsens1,rsens2,rsensp1,rsensp2
      rsens1(i1,i2,j,eps)=(
     &                  abs(u(i1+1,i2,j)-2d0*u(i1,i2,j)+u(i1-1,i2,j))/
     &                  max(abs(u(i1+1,i2,j)-u(i1,i2,j))+
     &                      abs(u(i1,i2,j)-u(i1-1,i2,j)),eps))**q
      rsensp1(i1,i2,j,eps)=(
     &             abs(v(i1+1,i2,ivp)-2d0*v(i1,i2,ivp)+v(i1-1,i2,ivp))/
     &              max(abs(v(i1+1,i2,ivp)+2d0*v(i1,i2,ivp)+
     &                      v(i1-1,i2,ivp)),eps))
      rsens2(i1,i2,j,eps)=(
     &                  abs(u(i1,i2+1,j)-2d0*u(i1,i2,j)+u(i1,i2-1,j))/
     &                  max(abs(u(i1,i2+1,j)-u(i1,i2,j))+
     &                      abs(u(i1,i2,j)-u(i1,i2-1,j)),eps))**q
      rsensp2(i1,i2,j,eps)=(
     &             abs(v(i1,i2+1,ivp)-2d0*v(i1,i2,ivp)+v(i1,i2-1,ivp))/
     &              max(abs(v(i1,i2+1,ivp)+2d0*v(i1,i2,ivp)+
     &                      v(i1,i2-1,ivp)),eps))

c     The following code was cut and pasted from cnsdu22/cnsdu22a
      grid = ipu( 1)
      irad = ipu(11) ! the coord. direction for the radius
      irad0= ipu(12) ! axis index for side=0
      irad1= ipu(13) ! axis index for side=1

      gamma =rpu( 3)
      av2   =rpu( 4)
      aw2   =rpu( 5)
      av4   =rpu( 6)
      aw4   =rpu( 7)

      hd(1) = rpu(13)           ! *wdh* 050311 -- change for parallel version
      hd(2) = rpu(14) 
      hdi(1) = 1d0/hd(1)
      hdi(2) = 1d0/hd(2)

      t     = rpu(16)
      dt    = rpu(17)

      ir=1
      m1=2
      m2=3
      m3=4                      !kkc
      ie=5                      !kkc 

      iv1=1                     ! z velocity
      iv2=2                     ! r velocity
      iv3=3                     ! theta velocity
      ivt=4                     ! p/rho
      ivp=5                     ! p
      
      if ( nc.eq.4 ) then
         ie=m3
         m3=m2
         ivp=ivt
         ivt=iv3
         iv3=iv2
      endif

      imr = m2                  ! r momentum
      imz = m1                  ! z momentum
      ivr = iv2                 ! r velocity
      ivz = iv1                 ! z velocity

c      write(*,*) iv1,iv2,ivt,ivp
c      write(*,*) av2,av4
c      write(*,*) hd(1),hd(2)
c      write(*,*) mrsab(1,1),mrsab(1,2)
c      write(*,*) mrsab(2,1),mrsab(2,2)
c calculate artificial viscosity (2nd and 4th order, r-component)

c     eps sits in the denominator of the sensor, it both guards against
c         small denominators and makes sure the dissipation remains small near
c         smooth discontinuities
      eps1 = epsr*hd(1)**(3d0/q)
      eps2 = epsr*hd(2)**(3d0/q)

      do j=1,nc

      do i2=mrsab(2,1),mrsab(2,2)

c     compute the sensor function and stick it into w(:,1)
         w(mrsab(1,1)-2,1) = 0d0
         do i1=mrsab(1,1)-1,mrsab(1,2)+1
            w(i1,1) = rsens1(i1,i2,j,eps1)
c            w(i1,1) = rsensp1(i1,i2,j,eps1)
         end do
         w(mrsab(1,2)+2,1) = 0d0

         do i1=mrsab(1,1)-1,mrsab(1,2)
            wmax = max(w(i1-1,1),w(i1,1),w(i1+1,1),w(i1+2,1))
c     estimate the spectral radius in the r direction
            a1=.5*(rx(i1,i2,1,1)*aj(i1,i2)+rx(i1+1,i2,1,1)*aj(i1+1,i2))
            a2=.5*(rx(i1,i2,1,2)*aj(i1,i2)+rx(i1+1,i2,1,2)*aj(i1+1,i2))
            
c         some people use Roe-averaged values for the velocity here...
            v1=.5*(v(i1,i2,iv1)+v(i1+1,i2,iv1))
            v2=.5*(v(i1,i2,iv2)+v(i1+1,i2,iv2))
            v3=.5*(v(i1,i2,iv3)+v(i1+1,i2,iv3))
            
            dist=sqrt(a1**2+a2**2)
            vn=a1*v1+a2*v2
c     ... and also roe averages for the enthalpy and energy
            csq=.5*gamma*(v(i1,i2,ivt)+v(i1+1,i2,ivt))
            c = sqrt(csq)
            alam1 = abs(vn) + c*dist

c     estimate the spectral radius in the s direction
            a1=.5*(rx(i1,i2,2,1)*aj(i1,i2)+rx(i1+1,i2,2,1)*aj(i1+1,i2))
            a2=.5*(rx(i1,i2,2,2)*aj(i1,i2)+rx(i1+1,i2,2,2)*aj(i1+1,i2))
            dist=sqrt(a1**2+a2**2)
            vn=a1*v1+a2*v2
            alam2 = abs(vn) + c*dist

c     calculate the 2nd order damping coeff. and stick it into w(i1,2)
c            psi = (1d0+(alam2/alam1)**bt)
            psi = 1d0

            if ( isaxi ) psi = psi*.5d0*(xy(i1,i2,2)+xy(i1+1,i2,2))

            w(i1,2) = av2*alam1*psi*wmax
c     the fourth order coefficient gets stuck into w(i1,3)
            w(i1,3) = alam1*psi*max(0d0,(av4-av2*wmax))
c            if ( i2.eq.18 ) write(*,*) "i1 : ",i1,alam1,wmax

c            w(i1,2) = av2*aj(i1,i2)/hd(1)
c            w(i1,3) = av4*aj(i1,i2)
            
c     the flux itself we put into w(:,4), i1 on the lhs is really i+1/2
            w(i1,4) = w(i1,2)*( u(i1+1,i2,j)-u(i1,i2,j) ) 
     &               -w(i1,3)*(
     &          u(i1+2,i2,j)-3.0*(u(i1+1,i2,j)-u(i1,i2,j))-u(i1-1,i2,j))
c            print *,j,i1,i2,wmax,w(i1,1)
         end do !i1

ckkc MASS FLUX FIX XXX
c         w(mrsab(1,1)-1,4) = -w(mrsab(1,1),4)
c         w(mrsab(1,2),4) = -w(mrsab(1,2)-1,4)

c     and here we actually add the fluxes
         if ( .not. isaxi) then
            do i1=mrsab(1,1),mrsab(1,2)
               ut(i1,i2,j) = ut(i1,i2,j) + ( w(i1,4)-w(i1-1,4) )
            end do              !i1
         else
            do i1=mrsab(1,1),mrsab(1,2)
               if ( (1d0+xy(i1,i2,2)).gt.1d0 ) then
            ut(i1,i2,j) = ut(i1,i2,j) + ( w(i1,4)-w(i1-1,4))/xy(i1,i2,2)
               else
c     r1=0.5d0*(xy(i1,i2,2)+xy(i1+1,i2,2))
c            r2=0.5d0*(xy(i1,i2,2)+xy(i1-1,i2,2))
c     ut(i1,i2,j) = ut(i1,i2,j) + ( w(i1,4)/r1-w(i1-1,4)/r2)
            endif
            end do              !i1
         end if
      end do                    ! i2
      
c
c calculate artificial viscosity (2nd and 4th order, s-component)
      do i1=mrsab(1,1),mrsab(1,2)

         w(mrsab(2,1)-2,1) = 0d0
c     compute the sensor function and stick it into w(:,1)
         do i2=mrsab(2,1)-1,mrsab(2,2)+1
            w(i2,1) = rsens2(i1,i2,j,eps2)
c            w(i2,1) = rsensp2(i1,i2,j,eps2)
         end do
         w(mrsab(2,2)+2,1) = 0d0

         do i2=mrsab(2,1)-1,mrsab(2,2)
            wmax = max(w(i2-1,1),w(i2,1),w(i2+1,1),w(i2+2,1))
            
c     compute the spectral radius in the s direction
            a1=.5*(rx(i1,i2,2,1)*aj(i1,i2)+rx(i1,i2+1,2,1)*aj(i1,i2+1))
            a2=.5*(rx(i1,i2,2,2)*aj(i1,i2)+rx(i1,i2+1,2,2)*aj(i1,i2+1))
            
c         some people use roe-averaged values for the velocity here...
            v1=.5*(v(i1,i2,iv1)+v(i1,i2+1,iv1))
            v2=.5*(v(i1,i2,iv2)+v(i1,i2+1,iv2))
            v3=.5*(v(i1,i2,iv3)+v(i1,i2+1,iv3))
            
            dist=sqrt(a1**2+a2**2)
            vn=a1*v1+a2*v2
c     ... and also roe averages for the enthalpy and energy
            csq=.5*gamma*(v(i1,i2,ivt)+v(i1,i2+1,ivt))
            c = sqrt(csq)
            alam2 = abs(vn) + c*dist

c     compute the spectral radius in the r direction
            a1=.5*(rx(i1,i2,1,1)*aj(i1,i2)+rx(i1,i2+1,1,1)*aj(i1,i2+1))
            a2=.5*(rx(i1,i2,1,2)*aj(i1,i2)+rx(i1,i2+1,1,2)*aj(i1,i2+1))
            dist=sqrt(a1**2+a2**2)
            vn=a1*v1+a2*v2
            alam1 = abs(vn) + c*dist
            
c     calculate the 2nd order damping coeff. and stick it into w(i2,2)
c            psi = (1d0+(alam1/alam2)**bt)
            psi=1d0

            if ( isaxi ) psi = psi*.5d0*(xy(i1,i2,2)+xy(i1,i2+1,2))
            w(i2,2) = av2*alam2*psi*wmax
c     the fourth order coefficient gets stuck into w(i2,3)
            w(i2,3) = alam2*psi*max(0d0,(av4-av2*wmax))

c            if ( i1.eq.1 ) write(*,*) "i2 : ",i2,alam2,wmax,c,dist

c            w(i2,2) = av2*aj(i1,i2)
c            w(i2,3) = av4*aj(i1,i2)

c     the flux itself we put into w(:,4), i2 on the lhs is really i+1/2
            w(i2,4) = w(i2,2)*( u(i1,i2+1,j)-u(i1,i2,j) ) 
     &               -w(i2,3)*(
     &          u(i1,i2+2,j)-3.0*(u(i1,i2+1,j)-u(i1,i2,j))-u(i1,i2-1,j))

         end do !i2


ckkc MASS FLUX FIX XXX
c            w(mrsab(2,1)-1,4) = -w(mrsab(2,1),4)
c            w(mrsab(2,2),4) = -w(mrsab(2,2)-1,4)
c     and here we actually add the fluxes
         if ( .not. isaxi) then
            do i2=mrsab(2,1),mrsab(2,2)
               ut(i1,i2,j) = ut(i1,i2,j) + ( w(i2,4)-w(i2-1,4) )
            end do              !i2
         else

            do i2=mrsab(2,1),mrsab(2,2)
               if ( (1d0+xy(i1,i2,2)).gt.1d0 ) then
            ut(i1,i2,j) = ut(i1,i2,j) + ( w(i2,4)-w(i2-1,4))/xy(i1,i2,2)
c            if ( j.eq.ir ) then 
c               print *,i2,w(i2,4),w(i2-1,4),(w(i2,4)-w(i2-1,4))
c            endif

               else
c            r1=0.5d0*(xy(i1,i2,2)+xy(i1,i2+1,2))
c            r2=0.5d0*(xy(i1,i2,2)+xy(i1,i2-1,2))
c               ut(i1,i2,j) = ut(i1,i2,j) + 
c     &              ( w(i2,4)/r1-w(i2-1,4)/r2)
               endif
            end do              !i2
         endif
      end do                    ! i1

      end do                    !j

      return
      end
