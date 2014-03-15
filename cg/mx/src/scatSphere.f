! This file automatically generated from scatSphere.bf with bpp.
c ============================================================================================
c Compute the scattered field solution of electromagnetic scattering from a sphere
c          a : radius of the sphere
c          k : wavelength of the incident light
c
c  This solution is taken from 
c    Bowman, Senior and Uslemghi, "Electromagnetic And Acoustic Scattering by Simple Shapes"
c
c  The solution is for an incident wave traveling in the positive z-direction (The opposite direction
c  to the above ref):   
c                     Ex =    exp(i(k*x-w*t))
c                     Hy = -Y*exp(i(k*x-w*t)) 
c =============================================================================================
      subroutine scatSphere(nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,
     & nd2b,nd3a,nd3b,nd4a,nd4b,xy,u,ipar,rpar )

      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer ipar(0:*)
      real rpar(0:*)

c.............local variables
      integer ntermMax
      parameter( ntermMax=50 )
      integer i1,i2,i3,nterm,ncalc,nb,ex,ey,ez,option,staggeredGrid,
     & debug
      real k,a,ka,r,theta,kr,x,y,z,alpha,twoPi,pi
      real jnp,ynp,jnpka,ynpka
      real jnka(0:ntermMax),ynka(0:ntermMax)
      real jn(0:ntermMax),yn(0:ntermMax)

      real psin(0:ntermMax),psinp(0:ntermMax),phin(0:ntermMax),phinp(
     & 0:ntermMax)
      real ar(0:ntermMax),ai(0:ntermMax),br(0:ntermMax),bi(0:ntermMax)
      real pn(0:ntermMax),pnp(0:ntermMax)

      real s,pm,cnt,cnp1t,sMax,eps,factor,denom,phi
      real cosPhi,sinPhi,ct,st,t1,t2,ci
      real err,etr,epr,eri,eti,epi
      real err0,etr0,epr0,eri0,eti0,epi0
      integer n,np1
      real psinka,psinpka,phinka,phinpka
      integer x1,x2,x3,e1,e2,e3

c...............end local variables      

      option = ipar(6)
      if( option.eq.1 )then
        ! dielectric case:
        call scatDielectricSphere(nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,xy,u,ipar,rpar )
        return
      end if

      k = rpar(0)  !
      a = rpar(1)  ! radius

      staggeredGrid=ipar(9)
      debug= ipar(10)

      ci = 0.   ! set to 1. to compute incident field too

      ! a=.001

      ex=ipar(0)
      ey=ipar(1)
      ez=ipar(2)

      !  Incident wave : Ex = exp(i*k*z - i*w*t ) positive z-direction
      x1=0
      x2=1
      x3=2
      e1=ex
      e2=ey
      e3=ez


      ! rotate these values to illuminate in a different direction
      ! This is for Ey = exp(i*k*x - i*w*t )  ( x -> y -> z -> x)
      x1=1
      x2=2
      x3=0
      e1=ey
      e2=ez
      e3=ex

      ka=k*a
      write(*,'(" scatSphere: k,a=",2f10.6," ex,ey,ez=",3i3)') k,a,ex,
     & ey,ez
      if( ka.le.0 )then
        stop 11223
      end if

      ! I estimate that the number of terms, N, should satisfy
      !        ??? N * eps**(1/N) > e*k*a/2 * 1/(2*pi)**(1/N)
      ! where eps=size of the final term (series is alternating)
      ! Take N = max( e*k*a, log(1/eps) )

      eps = 1.e-16
      ! nterm = max( abs(7.*ka), 16. )

      ! er takes a lot of terms if there is an incident field
      nterm=25+20*ci ! do this while testing

      nterm=min(nterm,ntermMax-2)
      nterm = nterm - mod(nterm,2) + 1   ! nterm should be odd
      ! nterm = 25  ! nterm should be odd
      pi=atan2(1.,1.)*4.
      twoPi=pi*2.

      ! First evaluate J(n+.5,ka), Y(n+.5,ka) n=0,1,...,nterm
      alpha=.5 ! fractional part of Bessel order

      nb = nterm+1  ! eval J0, J1, ... J(nb)  -- compute one extra
      call rjbesl(ka, alpha, nb, jnka, ncalc)
      call rybesl(ka, alpha, nb, ynka, ncalc)

      ! convert to spherical bessel:
      factor=sqrt(pi/(2.*ka))
      do n=0,nterm
        jnka(n)=jnka(n)*factor
        ynka(n)=ynka(n)*factor
      end do

      ! compute the derivatives 
      do n=1,nterm-1
        jnpka = ( n*jnka(n-1)-(n+1)*jnka(n+1) )/(2*n+1) ! no need to save in arrays
        ynpka = ( n*ynka(n-1)-(n+1)*ynka(n+1) )/(2*n+1)

        psinka  =ka*jnka(n)
        psinpka =jnka(n)+ka*jnpka

        phinka  =ka*ynka(n)
        phinpka =ynka(n)+ka*ynpka

        ! compute real and im parts of an and bn:
        denom=psinka**2+phinka**2
        ar(n)= psinka**2/denom
        ai(n)=-psinka*phinka/denom

        denom=psinpka**2+phinpka**2
        br(n)= psinpka**2/denom
        bi(n)=-psinpka*phinpka/denom

        ! write(*,'(" i=",3i4," n=",i3," ar,ai,br,bi=",4e11.2)') i1,i2,i3,n,ar(n),ai(n),br(n),bi(n)
        ! ar(n)=0.  ! --> this should result in a plane wave
        ! ai(n)=0.
        ! br(n)=0.
        ! bi(n)=0.
      end do

      sMax=0. ! keep track of the size of the last term for monitoring convergence

      do i3=n3a,n3b
      do i2=n2a,n2b
      do i1=n1a,n1b

        x= -xy(i1,i2,i3,x1)   ! note minus since we use exp(-i*w*t)
        y= -xy(i1,i2,i3,x2)
        z= -xy(i1,i2,i3,x3)

        r=sqrt(x**2+y**2+z**2)
        r=max(r,.75*a)    !  don't allow r to get too small -- not valid and convergence is poor

        if( abs(x).lt.eps .and. abs(y).lt.eps )then
          x=eps  ! avoid atan(0,0)
          y=eps
        end if
        phi=atan2(y,x)

        theta=acos(z/r)

        kr=k*r
        ! evaluate J(n+.5,kr), Y(n+.5,kr) n=0,1,...,nterm
        call rjbesl(kr, alpha, nb, jn, ncalc)
        call rybesl(kr, alpha, nb, yn, ncalc)
          ! convert to spherical bessel:
        factor=sqrt(pi/(2.*kr))
        do n=0,nterm
          jn(n)=jn(n)*factor
          yn(n)=yn(n)*factor
        end do
          ! compute the derivatives 
        do n=1,nterm-1
          jnp = ( n*jn(n-1)-(n+1)*jn(n+1) )/(2*n+1)          ! no need to save in arrays
          ynp = ( n*yn(n-1)-(n+1)*yn(n+1) )/(2*n+1)

          psin(n) =kr*jn(n)
          psinp(n)=jn(n)+kr*jnp

          phin(n) =kr*yn(n)
          phinp(n)=yn(n)+kr*ynp
        end do

        ! compute Legendre polynomials divided by sin(theta)
        !       pn(n) =  P_n^1(cos(theta))/sin(theta)
        ! Plus the derivative of P_n^1(cos(theta)) w.r.t theta:
        !       pnp(n) = (d/d(theta)) P_n^1(cos(theta))
        ct = z/r             ! cos(theta)
        st = sqrt(1.-ct**2)  !  sin(theta)
        pn(1)=1.             ! P_1^1 = sin(theta)
        pn(2)=3.*ct          ! P_2^1 = 3*cos(theta)*sin(theta)
        pnp(1)=ct
        pnp(2)=2.*ct*pn(2)-3.
        do n=3,nterm
          pn(n)=( (2*n-1)*ct*pn(n-1) - n*pn(n-2) )/(n-1)
          pnp(n)= n*ct*pn(n) - (n+1)*pn(n-1)
        end do

        ! compute the E field
        !   (err,eri) : Er : radial component
        !   (etr,eti) : theta component
        !   (epr,epi) : phi component
        err=0.
        eri=0.
        etr=0.
        eti=0.
        epr=0.
        epi=0.
        pm=1.  ! +1 or -1

        do n=1,nterm-2,2   ! nterm should be odd

          err=err + pm*(2*n+1)*( (ci-br(n))*psin(n) +bi(n)*phin(n) )*
     & pn(n)
          eri=eri - pm*(2*n+1)*( bi(n)*psin(n)+br(n)*phin(n) )*pn(n)

          t1 = -(ai(n)*psin(n)+ar(n)*phin(n))
          t2 = (ci-br(n))*psinp(n)+bi(n)*phinp(n)
          etr=etr + pm*(2*n+1.)/(n*(n+1))*( t1*pn(n) + t2*pnp(n) )
          epr=epr + pm*(2*n+1.)/(n*(n+1))*( t1*pnp(n)+ t2*pn(n) )

          t1 = (ci-ar(n))*psin(n)+ai(n)*phin(n)
          t2 = bi(n)*psinp(n)+br(n)*phinp(n)
          eti=eti - pm*(2*n+1.)/(n*(n+1))*( t1*pn(n) + t2*pnp(n) )
          epi=epi - pm*(2*n+1.)/(n*(n+1))*( t1*pnp(n)+ t2*pn(n) )


          np1=n+1
          err=err - pm*(2*np1+1)*( bi(np1)*psin(np1)+br(np1)*phin(np1) 
     & )*pn(np1)
          eri=eri - pm*(2*np1+1)*( (ci-br(np1))*psin(np1) +bi(np1)*
     & phin(np1) )*pn(np1)

          t1 = (ci-ar(np1))*psin(np1)+ai(np1)*phin(np1)
          t2 = bi(np1)*psinp(np1)+br(np1)*phinp(np1)
          etr=etr - pm*(2*np1+1.)/(np1*(np1+1))*( t1*pn(np1) + t2*pnp(
     & np1) )
          epr=epr - pm*(2*np1+1.)/(np1*(np1+1))*( t1*pnp(np1)+ t2*pn(
     & np1) )

          t1 = -(ai(np1)*psin(np1)+ar(np1)*phin(np1))
          t2 = (ci-br(np1))*psinp(np1)+bi(np1)*phinp(np1)
          eti=eti - pm*(2*np1+1.)/(np1*(np1+1))*( t1*pn(np1) + t2*pnp(
     & np1) )
          epi=epi - pm*(2*np1+1.)/(np1*(np1+1))*( t1*pnp(np1)+ t2*pn(
     & np1) )


          if( n.eq.(nterm-4) )then
            ! save second to last solution
            err0=err
            eri0=eri
            etr0=etr
            eti0=eti
            epr0=epr
            epi0=epi
          end if

          pm=-pm
        end do

        cosPhi=cos(phi)
        sinPhi=sin(phi)

        ! check the size of the last terms
        sMax = max(sMax,max(abs((cosPhi/kr**2)*st*(err-err0)),max(abs((
     & cosPhi/kr)*(etr-etr0)),abs((sinPhi/kr)*(epr-epr0)))))
        sMax = max(sMax,max(abs((cosPhi/kr**2)*st*(eri-eri0)),max(abs((
     & cosPhi/kr)*(eti-eti0)),abs((sinPhi/kr)*(epi-epi0)))))


        err=  (cosPhi/kr**2)*err*st
        etr = (cosPhi/kr)*etr
        epr =-(sinPhi/kr)*epr

        eri=  (cosPhi/kr**2)*eri*st
        eti = (cosPhi/kr)*eti
        epi =-(sinPhi/kr)*epi

        !  rHat     = ( sint*cosp, sint*sinp, cost )
        !  ThetaHat = ( cost*cosp, cost*sinp, -sint )
        !  phiHat   = (     -sinp,      cosp, 0 )
        ! u(i1,i2,i3,ex)= st*cosPhi*err + ct*cosPhi*etr -sinPhi*epr - cos(k*z)  ! subtract off incident field

        u(i1,i2,i3,e1  )= st*cosPhi*err + ct*cosPhi*etr -sinPhi*epr
        u(i1,i2,i3,e2  )= st*sinPhi*err + ct*sinPhi*etr +cosPhi*epr
        u(i1,i2,i3,e3  )= ct       *err - st       *etr

        u(i1,i2,i3,e1+3)= st*cosPhi*eri + ct*cosPhi*eti -sinPhi*epi
        u(i1,i2,i3,e2+3)= st*sinPhi*eri + ct*sinPhi*eti +cosPhi*epi
        u(i1,i2,i3,e3+3)= ct       *eri - st       *eti

        ! write(*,'(" i=",3i4," x=",3f6.2," err,etr,epr=",3f7.3," ex,ey,ez=",3f7.3)') i1,i2,i3,x,y,z,err,etr,epr,!          u(i1,i2,i3,ex),u(i1,i2,i3,ey),u(i1,i2,i3,ez)

        ! u(i1,i2,i3,ex)= cos(k*z) 
        ! u(i1,i2,i3,ey)=0.
        ! u(i1,i2,i3,ez)=0.

      end do
      end do
      end do

      write(*,'(" >>>scatSphere: nterm=",i3," largest final term 
     & sMax=",e10.2)') nterm,sMax
      ! '

      return
      end



c ============================================================================================
c Compute the scattered field solution of electromagnetic scattering from a dielectric sphere
c          a : radius of the sphere
c          k : wavelength of the incident light
c
c  This solution is taken from 
c    Light Scattering by Small Particles by van de Hulst~\cite{vanDeHulst57}. 
c
c  The solution is for an incident wave traveling in the positive z-direction 
c    NOTE the time factor of exp( + i w t )
c                     Ex =  exp(i(-k*z+w*t))
c                     Ey = 0 
c                     Ez = 0   
c =============================================================================================
      subroutine scatDielectricSphere(nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,xy,u,ipar,rpar )

      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer ipar(0:*)
      real rpar(0:*)

c.............local variables
      integer ntermMax
      parameter( ntermMax=50 )
      integer i1,i2,i3,nterm,ncalc,nb,ex,ey,ez,hx,hy,hz,staggeredGrid,
     & numEdges,edge,debug
      real k,a,ka,r,theta,kr,x,y,z,alpha,twoPi,pi
      real jnp,ynp,jnpka,ynpka
      real jnka(0:ntermMax),ynka(0:ntermMax)
      real jn(0:ntermMax),yn(0:ntermMax)

      real jnmka(0:ntermMax),ynmka(0:ntermMax)

      ! real psin(0:ntermMax),psinp(0:ntermMax),phin(0:ntermMax),phinp(0:ntermMax)
      real pn(0:ntermMax),pnp(0:ntermMax)

      real s,pm,cnt,cnp1t,sMax,eps,factor,denom,phi
      real cosPhi,sinPhi,ct,st,t1,t2,ci
      real err,etr,epr,eri,eti,epi
      real hrr,htr,hpr,hri,hti,hpi
      real err0,etr0,epr0,eri0,eti0,epi0
      integer n,np1
      real psinka,psinpka,phinka,phinpka
      integer x1,x2,x3,e1,e2,e3,h1,h2,h3
      integer exr,eyr,ezr, exi,eyi,ezi

      integer option,inOut,computeIncidentField
      real am,mka,factorm,sc
      complex*16 ai,aimn,ct1,ct2
      complex*16 an(0:ntermMax),bn(0:ntermMax),cn(0:ntermMax),dn(
     & 0:ntermMax)

      real jnpmka,ynpmka,psinmka,psinpmka
      complex*16 zetanka,zetanpka, denom1,denom2
      real psinkr,psinpkr,phinkr,phinpkr
      complex*16 zetankr,zetanpkr
      complex*16 er,et,ep, er0,et0,ep0, hr,ht,hp,hr0,ht0,hp0

      logical inside,outside,computeMagneticField,computeElectricField

c...............end local variables      


      k = rpar(0)  !
      a = rpar(1)  ! radius
      am= rpar(2)  ! "m" ratio of dielectrics

      ! It is better to compute the incident field explicit since the series
      ! converges slowly 
      ci=0.

      exr=ipar(0)  ! holds Re(Ex)
      eyr=ipar(1)
      ezr=ipar(2)
      exi=ipar(3)  ! holds Im(Ex)
      eyi=ipar(4)
      ezi=ipar(5)
      option=ipar(6) ! should be 1
      inOut=ipar(7)  ! 0 = outside, 1=inside
      if( inOut.eq.0 )then
        inside = .false.
        outside = .true.
      else
        inside = .true.
        outside = .false.
      end if
      computeIncidentField=ipar(8)
      staggeredGrid=ipar(9)
      debug= ipar(10)

      ka=k*a
      mka=am*k*a
      ai=cmplx(0.,1.)  ! i

      sc=1.     ! set to 1 for exp( i*w*t) or -1 for exp(i*w*t)
      ! a=.001

      ! OLD WAY:
      ex=ipar(0)
      ey=ipar(1)
      ez=ipar(2)

      hx=ez+1  ! assume this for now
      hy=hx+1
      hz=hy+1

      !  Incident wave : Ex = exp(i*k*z - i*w*t ) positive z-direction
      x1=0
      x2=1
      x3=2
      e1=ex
      e2=ey
      e3=ez
      h1=hx
      h2=hy
      h3=hz


      ! rotate these values to illuminate in a different direction
      ! This is for Ey = exp(i*k*x - i*w*t )  ( x -> y -> z -> x)
      x1=1
      x2=2
      x3=0
      e1=ey  ! staggered grid version assumes this ordering
      e2=ez
      e3=ex
      h1=hy  ! staggered grid version assumes this ordering
      h2=hz
      h3=hx

      pi=atan2(1.,1.)*4.
      twoPi=pi*2.

      ka=k*a
      write(*,'(" scatSphere: k,k/2*pi,a,am=",4f10.6," ex,ey,ez=",3i3)
     & ') k,k/twoPi,a,am,ex,ey,ez
      write(*,'(" scatSphere: inOut=",i2,"(1=inside), ci=",f3.1,"(
     & 1=compute incident field)")') inOut,ci
      write(*,'(" scatSphere: ex,ey,ez, hx,hy,hz =",6i3)') ex,ey,ez, 
     & hx,hy,hz
      ! ' 
      if( ka.le.0 )then
        stop 11223
      end if

      ! I estimate that the number of terms, N, should satisfy
      !        ??? N * eps**(1/N) > e*k*a/2 * 1/(2*pi)**(1/N)
      ! where eps=size of the final term (series is alternating)
      ! Take N = max( e*k*a, log(1/eps) )

      ! eps = 1.e-16
      eps = 1.e-15
      ! nterm = max( abs(7.*ka), 16. )

      ! *** NOTE: er takes a lot of terms if there is an incident field *** 
      nterm=25+15*ci ! do this while testing

      nterm=min(nterm,ntermMax-2)
      nterm = nterm - mod(nterm,2) + 1   ! nterm should be odd
      ! nterm = 25  ! nterm should be odd ??

      ! First evaluate J(n+.5,ka), Y(n+.5,ka) n=0,1,...,nterm
      alpha=.5 ! fractional part of Bessel order

      nb = nterm+1  ! eval J0, J1, ... J(nb)  -- compute one extra
      call rjbesl(ka, alpha, nb, jnka, ncalc)
      call rybesl(ka, alpha, nb, ynka, ncalc)

      ! eval Jn(mka), Yn(mka)
      call rjbesl(mka, alpha, nb, jnmka, ncalc)
      call rybesl(mka, alpha, nb, ynmka, ncalc)

      ! convert to spherical bessel:
      factor=sqrt(pi/(2.*ka))
      factorm=sqrt(pi/(2.*mka))
      do n=0,nterm
        jnka(n)=jnka(n)*factor
        ynka(n)=ynka(n)*factor

        jnmka(n)=jnmka(n)*factorm
        ynmka(n)=ynmka(n)*factorm
      end do

      ! psin(z) = z*jn(z) 
      ! phin(z) = z*yn(z) 
      ! zetan(z)= psin(z) - i phin(z)   

      ! Precompute coefficients an,bn,cn,dn 
      do n=1,nterm-1
        ! derivatives: 
        jnpka = ( n*jnka(n-1)-(n+1)*jnka(n+1) )/(2*n+1) ! no need to save in arrays
        ynpka = ( n*ynka(n-1)-(n+1)*ynka(n+1) )/(2*n+1)

        psinka  =ka*jnka(n)            ! psin(ka)
        psinpka =jnka(n)+ka*jnpka      ! psin'(ka)

        phinka  =ka*ynka(n)            ! phin(ka)
        phinpka =ynka(n)+ka*ynpka      ! phin'(ka)

        zetanka = cmplx(psinka, sc*phinka)  ! zetan(ka)
        zetanpka= cmplx(psinpka,sc*phinpka) ! zetan'(ka)

        ! eval at m*k*a
        jnpmka = ( n*jnmka(n-1)-(n+1)*jnmka(n+1) )/(2*n+1) ! no need to save in arrays
        ynpmka = ( n*ynmka(n-1)-(n+1)*ynmka(n+1) )/(2*n+1)

        psinmka  =mka*jnmka(n)             ! psin(mka)
        psinpmka =jnmka(n)+mka*jnpmka      ! psin'(mka)

        ! compute complex valued an,bn,cn,dn
        denom1 =    psinpmka*zetanka - am*psinmka*zetanpka
        denom2 = am*psinpmka*zetanka -    psinmka*zetanpka

        if( outside )then
          an(n) = (   psinpmka*psinka - am*psinmka*psinpka)/denom1
          bn(n) = (am*psinpmka*psinka -    psinmka*psinpka)/denom2
        else
          cn(n) = -sc*ai*am/denom1  ! include factor of "m" here
          dn(n) = -sc*ai*am/denom2
        end if
      end do


      numEdges=1
      if( staggeredGrid.eq.1 )then
       numEdges=6
       n1b=min(n1b,nd1b-1)
       n2b=min(n2b,nd2b-1)
       n3b=min(n3b,nd3b-1)
      end if

      do edge=1,numEdges  ! for a staggered grid we need to evaluate along edges and the cell-centers
       computeElectricField=edge.le.3
       computeMagneticField=edge.gt.3

       sMax=0. ! keep track of the size of the last term for monitoring convergence

       ! -----------------------------------------------
       ! ---------------- Loop over points -------------
       ! -----------------------------------------------
       do i3=n3a,n3b
       do i2=n2a,n2b
       do i1=n1a,n1b

        ! note minus since we use exp(-i*w*t) 
        x= -xy(i1,i2,i3,x1)   ! this is really y
        y= -xy(i1,i2,i3,x2)   ! this is really z
        z= -xy(i1,i2,i3,x3)   ! this is really x

        if( staggeredGrid.eq.1 )then
          if( edge.eq.1 )then
            ! Ex lives on this edge
            z=-.5*(xy(i1,i2,i3,x3)+xy(i1+1,i2,i3,x3) )   ! x
          else if( edge.eq.2 )then
            ! Ey lives on this edge
            x=-.5*(xy(i1,i2,i3,x1)+xy(i1,i2+1,i3,x1) )   ! y
          else if( edge.eq.3 )then
            ! Ez lives on this edge
            y=-.5*(xy(i1,i2,i3,x2)+xy(i1,i2,i3+1,x2) )   ! z

          else if( edge.eq.4 )then
            ! Hx lives on a face
            x=-.5*(xy(i1,i2,i3,x1)+xy(i1,i2+1,i3,x1) )   ! y
            y=-.5*(xy(i1,i2,i3,x2)+xy(i1,i2,i3+1,x2) )   ! z
          else if( edge.eq.5 )then
            ! Hy lives on a face
            y=-.5*(xy(i1,i2,i3,x2)+xy(i1,i2,i3+1,x2) )   ! z
            z=-.5*(xy(i1,i2,i3,x3)+xy(i1+1,i2,i3,x3) )   ! x
          else if( edge.eq.6 )then
            ! Hz lives on a face
            z=-.5*(xy(i1,i2,i3,x3)+xy(i1+1,i2,i3,x3) )   ! x
            x=-.5*(xy(i1,i2,i3,x1)+xy(i1,i2+1,i3,x1) )   ! y
          end if
        end if

        r=sqrt(x**2+y**2+z**2)

        if( outside )then
          ! r=max(r,.75*a)    !  don't allow r to get too small -- not valid and convergence is poor
          if( r.lt. .75*a )then ! *wdh* 090522
            ! we are computing the outside solution but the point is far inside -- skip this point
            goto 100
          end if
        else
          if( r.gt. 1.2*a )then
            ! we are computing the inside solution but the point is far outside -- skip this point
            goto 100
          end if
        end if

        if( r.lt.eps )then ! avoid division by zero
          x=eps  ! avoid atan(0,0)
          y=eps
          z=eps
          r=sqrt(3.)*eps  ! r=sqrt(x**2+y**2+z**2)
        end if

        phi=atan2(y,x)
        theta=acos(z/r)

        if( outside )then
          kr=k*r
        else
          kr = am*k*r
        end if

        ! evaluate J(n+.5,kr), Y(n+.5,kr) n=0,1,...,nterm
        call rjbesl(kr, alpha, nb, jn, ncalc)
          ! convert to spherical bessel:
        factor=sqrt(pi/(2.*kr))
        do n=0,nterm
          jn(n)=jn(n)*factor
        end do

        if( outside )then
          call rybesl(kr, alpha, nb, yn, ncalc)
          ! convert to spherical bessel:
          do n=0,nterm
            yn(n)=yn(n)*factor
          end do
        end if

        ! compute Legendre polynomials divided by sin(theta)
        !       pn(n) =  P_n^1(cos(theta))/sin(theta)
        ! Plus the derivative of P_n^1(cos(theta)) w.r.t theta:
        !       pnp(n) = (d/d(theta)) P_n^1(cos(theta))

        ct = z/r             ! cos(theta)
        st = sqrt(1.-ct**2)  !  sin(theta)
        pn(1)=1.             ! P_1^1 = sin(theta)
        pn(2)=3.*ct          ! P_2^1 = 3*cos(theta)*sin(theta)
        pnp(1)=ct
        pnp(2)=2.*ct*pn(2)-3.
        do n=3,nterm
          pn(n)=( (2*n-1)*ct*pn(n-1) - n*pn(n-2) )/(n-1)
          pnp(n)= n*ct*pn(n) - (n+1)*pn(n-1)
        end do

        ! compute the E field
        !   er : radial component
        !   et : theta component
        !   ep : phi component

        er=0.
        et=0.
        ep=0.

        hr=0. ! magnetic field
        ht=0.
        hp=0.

        aimn=1. ! aimn = (i)^{-n} = (-i)^n
        do n=1,nterm-1

          if( mod(n,4).eq.0 )then
           aimn=1.
          else
           aimn=-aimn*ai
          end if

          ! jn'
          jnp = ( n*jn(n-1)-(n+1)*jn(n+1) )/(2*n+1)          ! no need to save in arrays
          ! psi(z) = z jn(z), phi(z)=z*yn(z)
          psinkr =kr*jn(n)
          psinpkr=jn(n)+kr*jnp

          ! er : radial component (prefix added below)
          ! et : theta component (prefix added below)
          ! ep : phi component (prefix added below)
          if( outside )then
            ! yn'
            ynp = ( n*yn(n-1)-(n+1)*yn(n+1) )/(2*n+1)
            phinkr =kr*yn(n)
            phinpkr=yn(n)+kr*ynp

            zetankr = cmplx(psinkr ,sc*phinkr)   ! zetan(ka)
            zetanpkr= cmplx(psinpkr,sc*phinpkr)  ! zetan'(ka)

           ! ci=1 to include incident field: 
           if( computeElectricField )then
            er = er + aimn*(2*n+1)*pn(n)*( ci*psinkr - an(n)*zetankr )
            ct1 = ci*psinkr  - bn(n)*zetankr
            ct2 = ci*psinpkr - an(n)*zetanpkr
            et = et + aimn*(2*n+1.)/(n*(n+1))*( pn(n) *ct1 + ai*pnp(n)*
     & ct2 )
            ep = ep + aimn*(2*n+1.)/(n*(n+1))*( pnp(n)*ct1 + ai*pn(n) *
     & ct2 )
           end if
           if( computeMagneticField )then
            ! assume ci=0. 
            ! E -> H/m : change ?? 
            hr = hr + aimn*(2*n+1)*pn(n)*(           - bn(n)*zetankr ) 
     &   ! - +
            ct1 =            - an(n)*zetankr
            ct2 =            - bn(n)*zetanpkr
            ht = ht + aimn*(2*n+1.)/(n*(n+1))*( pn(n) *ct1 + ai*pnp(n)*
     & ct2 )
            hp = hp + aimn*(2*n+1.)/(n*(n+1))*(-pnp(n)*ct1 - ai*pn(n) *
     & ct2 )  ! - added  E -> H

           end if

          else
           ! inside 
           if( computeElectricField )then
            er = er + aimn*(2*n+1)*pn(n)*cn(n)*psinkr
            ct1 = dn(n)*psinkr
            ct2 = cn(n)*psinpkr
            et = et + aimn*(2*n+1.)/(n*(n+1))*( pn(n) *ct1 + ai*pnp(n)*
     & ct2 )
            ep = ep + aimn*(2*n+1.)/(n*(n+1))*( pnp(n)*ct1 + ai* pn(n)*
     & ct2 )
           end if
           if( computeMagneticField )then
            ! assume ci=0. 
            ! E -> H/m : change ?? 
            hr = hr + aimn*(2*n+1)*pn(n)*dn(n)*psinkr  ! - +
            ct1 = cn(n)*psinkr
            ct2 = dn(n)*psinpkr
            ht = ht + aimn*(2*n+1.)/(n*(n+1))*( pn(n) *ct1 + ai*pnp(n)*
     & ct2 )
            hp = hp + aimn*(2*n+1.)/(n*(n+1))*(-pnp(n)*ct1 - ai* pn(n)*
     & ct2 ) ! - added  E -> H
           end if
          end if

          if( n.eq.nterm-2 )then
            ! save second to last solution
            er0=er
            et0=et
            ep0=ep
            hr0=hr
            ht0=ht
            hp0=hp
          end if

        end do
        ! check the size of the last terms
        cosPhi=cos(phi)
        sinPhi=sin(phi)

        sMax = max(sMax,max(abs((cosPhi/kr**2)*st*(er-er0)),max(abs((
     & cosPhi/kr)*(et-et0)),abs((sinPhi/kr)*(ep-ep0)))))

        sMax = max(sMax,max(abs((cosPhi/kr**2)*st*(hr-hr0)),max(abs((
     & cosPhi/kr)*(ht-ht0)),abs((sinPhi/kr)*(hp-hp0)))))


        if( computeElectricField )then
          ! note: er is multiplied by sin(theta) since we used pn(n) which includes a factor 1/sin(theta)
          ! er = -ai*(cosPhi/kr**2)*st*er
          er = sc*ai*(cosPhi/kr**2)*st*er   ! change sign to match exp(-i w t )
          et =       (cosPhi/kr   )*et
          ep =      -(sinPhi/kr   )*ep

          !  rHat     = ( sint*cosp, sint*sinp, cost )
          !  ThetaHat = ( cost*cosp, cost*sinp, -sint )
          !  phiHat   = (     -sinp,      cosp, 0 )
          ! u(i1,i2,i3,ex)= st*cosPhi*err + ct*cosPhi*etr -sinPhi*epr 

          err = real(er)
          eri = aimag(er)
          etr = real(et)
          eti = aimag(et)
          epr = real(ep)
          epi = aimag(ep)
        end if
        if( computeMagneticField )then
          ! Switch cosPhi <-> sinPhi
          hr = sc*ai*(sinPhi/kr**2)*st*hr   ! change sign to match exp(-i w t )
          ht =       (sinPhi/kr   )*ht
          hp =      -(cosPhi/kr   )*hp
          hrr = real(hr)
          hri = aimag(hr)
          htr = real(ht)
          hti = aimag(ht)
          hpr = real(hp)
          hpi = aimag(hp)
          if( inside )then
           ! Is this correct ? 
           hrr = hrr*am
           hri = hri*am
           htr = htr*am
           hti = hti*am
           hpr = hpr*am
           hpi = hpi*am
          end if
        end if

        ! Convert Er, Etheta, Ephi to Ex Ey Ez
        !  (Ex,Ey,Ez) = Er rHat + Etheta thetaHat + Ephi phiHat

        if( staggeredGrid.eq.0 )then
          ! node centered values -- only save E 
          u(i1,i2,i3,e1  )= st*cosPhi*err + ct*cosPhi*etr -sinPhi*epr
          u(i1,i2,i3,e1+3)= st*cosPhi*eri + ct*cosPhi*eti -sinPhi*epi

          u(i1,i2,i3,e2  )= st*sinPhi*err + ct*sinPhi*etr +cosPhi*epr
          u(i1,i2,i3,e2+3)= st*sinPhi*eri + ct*sinPhi*eti +cosPhi*epi

          u(i1,i2,i3,e3  )= ct       *err - st       *etr
          u(i1,i2,i3,e3+3)= ct       *eri - st       *eti

          if( outside .and. computeIncidentField.eq.1 )then
           ! add on the incident field -- note z -> -z 
           u(i1,i2,i3,e1  )=u(i1,i2,i3,e1  )+cos(k*z)
           u(i1,i2,i3,e1+3)=u(i1,i2,i3,e1+3)-sin(k*z)
          end if

        ! else: --- Staggered grid ---
        !       edge/face centered values: save E and H 
        else if( edge.eq.1 )then
          ! Ex -- assumes e3=ex
          u(i1,i2,i3,e3  )= ct       *err - st       *etr
          u(i1,i2,i3,e3+6)= ct       *eri - st       *eti
        else if( edge.eq.2 )then
          ! Ey
          u(i1,i2,i3,e1  )= st*cosPhi*err + ct*cosPhi*etr -sinPhi*epr
          u(i1,i2,i3,e1+6)= st*cosPhi*eri + ct*cosPhi*eti -sinPhi*epi
          if( outside .and. computeIncidentField.eq.1 )then
           ! add on the incident field -- note z -> -z 
           u(i1,i2,i3,e1  )=u(i1,i2,i3,e1  )+cos(k*z)
           u(i1,i2,i3,e1+6)=u(i1,i2,i3,e1+6)-sin(k*z)
          end if
        else if( edge.eq.3 )then
          ! Ez
          u(i1,i2,i3,e2  )= st*sinPhi*err + ct*sinPhi*etr +cosPhi*epr
          u(i1,i2,i3,e2+6)= st*sinPhi*eri + ct*sinPhi*eti +cosPhi*epi

        else if( edge.eq.4 )then
          ! Hx 
          u(i1,i2,i3,h3  )= ct       *hrr - st       *htr
          u(i1,i2,i3,h3+6)= ct       *hri - st       *hti

        else if( edge.eq.5 )then
          ! Hy 
          u(i1,i2,i3,h1  )= st*cosPhi*hrr + ct*cosPhi*htr -sinPhi*hpr
          u(i1,i2,i3,h1+6)= st*cosPhi*hri + ct*cosPhi*hti -sinPhi*hpi

        else if( edge.eq.6 )then
          ! Hz 
          u(i1,i2,i3,h2  )= st*sinPhi*hrr + ct*sinPhi*htr +cosPhi*hpr
          u(i1,i2,i3,h2+6)= st*sinPhi*hri + ct*sinPhi*hti +cosPhi*hpi
          if( outside .and. computeIncidentField.eq.1 )then
           ! add on the incident field -- note z -> -z 
           u(i1,i2,i3,h2  )=u(i1,i2,i3,h2  )+cos(k*z)
           u(i1,i2,i3,h2+6)=u(i1,i2,i3,h2+6)-sin(k*z)
          end if

        end if

        ! write(*,'(" i=",3i4," x=",3f6.2," err,etr,epr=",3f7.3," ex,ey,ez=",3f7.3)') i1,i2,i3,x,y,z,err,etr,epr,!          u(i1,i2,i3,ex),u(i1,i2,i3,ey),u(i1,i2,i3,ez)

        ! u(i1,i2,i3,ex)= cos(k*z) 
        ! u(i1,i2,i3,ey)=0.
        ! u(i1,i2,i3,ez)=0.

 100     continue
      end do
      end do
      end do

      write(*,'(" >>>scatSphere: edge=",i1," nterm=",i3," largest 
     & final term sMax=",e10.2)') edge,nterm,sMax
      write(*,'(" >>>scatSphere: computeElectricField=",l1," 
     & computeMagneticField=",l1)') computeElectricField,
     & computeMagneticField
      ! '
      end do ! numEdges

      return
      end







