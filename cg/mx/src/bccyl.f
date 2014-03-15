c*wdh* This version takes the bounds on p and f as input
      subroutine bccyld(nda,ndb,p,f,ploc,c,r,dt,n,m,md,ns,ord,fold,phi,
     &                 npoles,alpha,beta,amc,fftsave,bcinit)
c
c  this routine uses an exponential adams-moulton formula to compute -
c  in Fourier variables - rational approximation to the cylinder kernel
c
c     (d/dt - c beta_j/R) phi_j = c alpha_j phat/R^2 
c
c     fhat = sum phi_j
c
c  double precision: p(n,m) - m fields to which the operator should be applied
c
c  double precision: f(n,m) - the m results
c
c  double precision: ploc(n) - workspace 
c
c  double precision: c - the wave speed
c
c  double precision: r - the radius
c
c  double precision: dt - the time step
c
c  integer: n the number of grid points - most efficient if it has small
c           prime factors, preferably even
c
c  integer: m the number of fields
c
c  integer: md the maximum mode used in the bc md < n/2
c
c  integer: ns>=2*n+15 
c
c  integer: ord - time-stepping order - note that the stability domain for
c                 Adams-Moulton methods gets small if this is too big
c 
c  complex*16: fold(0:ord-2,0:md,44,m) - stored values for time-stepping
c
c  complex*16: phi(0:md,44,m) - the auxiliary functions computed here
c
c  integer: npoles(0:md) - #poles - computed here
c
c  complex*16: alpha(0:md,44) - the amplitudes computed here
c
c  complex*16: beta(0:md,44) - the poles computed here
c
c  double precision: amc(-1:ord-2) - Adams-Moulton coefficients (computed here)
c                              use amcof.f
c
c  double precision: fftsave(ns) - used by fftpack - link to rffti,rfftf,rfftb
c
c  integer bcinit: initialize to zero
c
c  reads pole data from the file polec8.dat 
c
c  NOTE: USES FORT.90 FOR INPUT ON FIRST CALL  
c
      implicit none
c
      integer n,m,bcinit,i,j,k,kk,l,ord,ns,md,npoles(0:md)
      integer nda,ndb
      complex*16 alpha(0:md,44),beta(0:md,44),phi(0:md,44,m)
      complex*16 fold(0:ord-2,0:md,44,m),adon,phat,xfact 
      double precision p(nda:ndb,m),f(nda:ndb,m),ploc(n),fftsave(ns)
      double precision amc(-1:ord-2),dt,r,c,xx,yy
c
      if (bcinit.eq.0) then
c  initialize      
        open(unit=90,file="polec8.dat",status="old")
        do k=0,md
          read(90,*)kk,npoles(k)
          do j=1,npoles(k)
            read(90,*)xx,yy
            alpha(k,j)=DCMPLX(xx,yy)
          end do
          do j=1,npoles(k)
            read(90,*)xx,yy
            beta(k,j)=DCMPLX(xx,yy)
          end do
        end do
        close(unit=90) 
        call amcof(amc,ord)
        do i=1,m
          do j=1,44
            do k=0,md
              phi(k,j,i)=0.d0
              do l=0,ord-2
                fold(l,k,j,i)=0.d0
              end do
            end do
          end do
        end do
        CALL rffti(n,fftsave)
        bcinit=1
      end if
c
      do i=1,m
c
c loop over fields
c
        do k=1,n
          ploc(k)=p(k,i)
        end do
        CALL rfftf(n,ploc,fftsave)
        do k=0,md
          do j=1,npoles(k)
            xfact=exp(c*beta(k,j)*dt/r)
            adon=xfact*phi(k,j,i)
            do l=0,ord-2
              adon=adon+xfact*amc(l)*fold(l,k,j,i)
              xfact=xfact*exp(c*beta(k,j)*dt/r)
            end do
            if (k.eq.0) then
              phat=DCMPLX(ploc(1),0.d0)
            else
              phat=DCMPLX(ploc(2*k),ploc(2*k+1)) 
            end if
            adon=adon-c*dt*amc(-1)*alpha(k,j)*phat/(r*r)  
            phi(k,j,i)=adon
            if (ord.gt.2) then
            do l=ord-2,1,-1
              fold(l,k,j,i)=fold(l-1,k,j,i)
            end do
            end if
            fold(0,k,j,i)=-c*dt*alpha(k,j)*phat/(r*r) 
          end do
        end do
        do k=1,n
          ploc(k)=0.d0
        end do
        do k=0,md
          do j=1,npoles(k)
            if (k.eq.0) then
              ploc(1)=ploc(1)+DREAL(phi(k,j,i))
            else
              ploc(2*k)=ploc(2*k)+DREAL(phi(k,j,i))
              ploc(2*k+1)=ploc(2*k+1)+DIMAG(phi(k,j,i))
            end if 
          end do
        end do 
        CALL rfftb(n,ploc,fftsave)
        do k=1,n
          f(k,i)=ploc(k)/DBLE(n)
        end do 
      end do
c
      return
      end

