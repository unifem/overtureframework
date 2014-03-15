      subroutine bcperq31(p,f,ploc,c,len,dt,n,m,md,ns,ord,fold,phi,amc,
     &                    fftsave,bcinit)
      call  bcperq31d(1,n,p,f,ploc,c,len,dt,n,m,md,ns,
     &                ord,fold,phi,amc,fftsave,bcinit)
      return
      end 


c*wdh* This version takes the bounds on p and f as input

      subroutine bcperq31d(nda,ndb,p,f,ploc,c,len,dt,n,m,md,ns,
     &                     ord,fold,phi,amc,fftsave,bcinit)
c
c  this routine uses an exponential adams-moulton formula to compute -
c  in Fourier variables - 31-pole approximation to the
c  planar kernel
c
c     (d/dt - c beta_j w) phi_j = c alpha_j w^2 phat 
c
c     fhat = sum phi_j
c
c     w = k*scl , k=1, ... 
c      
c  double precision: p(n,m) - m fields to which the operator should be applied
c
c  double precision: f(n,m) - the m results
c
c  double precision: ploc(n) - workspace
c
c  double precision: c - the wave speed
c
c  double precision: len - the period
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
c  integer: ns >= 2*n+15
c
c  integer: ord - time-stepping order - note that the stability domain for
c                 Adams-Moulton methods gets small if this is too big
c 
c  complex*16: fold(0:ord-2,md,31,m) - stored values for time-stepping
c
c  complex*16: phi(md,31,m) - the auxiliary functions computed here
c
c  double precision: amc(-1:ord-2) - Adams-Moulton coefficients (computed here)
c                              use amcof.f
c
c  double precision: fftsave(ns) - used by fftpack - link to rffti,rfftf,rfftb
c
c  integer bcinit: initialize to zero 
c
      implicit none
c
      integer n,m,bcinit,i,j,k,l,ord,ns,md
      complex*16 alpha(31),beta(31),phi(md,31,m)
      complex*16 fold(0:ord-2,md,31,m),adon,phat,xfact
c*wdh      double precision p(n,m),f(n,m),ploc(n),fftsave(ns)
      integer nda,ndb
      double precision p(nda:ndb,m),f(nda:ndb,m),ploc(n),fftsave(ns)
      double precision amc(-1:ord-2),scl,dt,w,len,c
c
      alpha(1)=(-.7248698190879261E-07,-.7266856459887401E-07)
      alpha(2)=(-.7248698190879261E-07, .7266856459887401E-07)
      alpha(3)=(-.3761817129252043E-06,-.3769314065715886E-06)
      alpha(4)=(-.3761817129252043E-06, .3769314065715886E-06)
      alpha(5)=(-.1264056865588544E-05,-.1266668072310468E-05)
      alpha(6)=(-.1264056865588544E-05, .1266668072310468E-05)
      alpha(7)=(-.3738466040644283E-05,-.3758052767416740E-05)
      alpha(8)=(-.3738466040644283E-05, .3758052767416740E-05)
      alpha(9)=(-.1053054276380811E-04,-.1064172701253104E-04)
      alpha(10)=(-.1053054276380811E-04, .1064172701253104E-04)
      alpha(11)=(-.2902787565325144E-04,-.2948662740491706E-04)
      alpha(12)=(-.2902787565325144E-04, .2948662740491706E-04)
      alpha(13)=(-.7907555696587643E-04,-.8101521788775704E-04)
      alpha(14)=(-.7907555696587643E-04, .8101521788775704E-04)
      alpha(15)=(-.2136714788000509E-03,-.2223118737627421E-03)
      alpha(16)=(-.2136714788000509E-03, .2223118737627421E-03)
      alpha(17)=(-.5718490556660803E-03,-.6120497162181238E-03)
      alpha(18)=(-.5718490556660803E-03, .6120497162181238E-03)
      alpha(19)=(-.1500648374605461E-02,-.1702427459760661E-02)
      alpha(20)=(-.1500648374605461E-02, .1702427459760661E-02)
      alpha(21)=(-.3777846647633256E-02,-.4822679646038999E-02)
      alpha(22)=(-.3777846647633256E-02, .4822679646038999E-02)
      alpha(23)=(-.8612337122259847E-02,-.1397170663788553E-01)
      alpha(24)=(-.8612337122259847E-02, .1397170663788553E-01)
      alpha(25)=(-.1436234309979386E-01,-.4057035360912333E-01)
      alpha(26)=(-.1436234309979386E-01, .4057035360912333E-01)
      alpha(27)=(.5115479570066042E-02,-.1052205533094523E+00)
      alpha(28)=(.5115479570066042E-02, .1052205533094523E+00)
      alpha(29)=(.1353590959088723E+00,-.1619011637672531E+00)
      alpha(30)=(.1353590959088723E+00, .1619011637672531E+00)
      alpha(31)=(.2773935622389255E+00, .0000000000000000E+00)
c
      beta(1)=(-.2295682215845089E-04, .9999999803841022E+00)
      beta(2)=(-.2295682215845089E-04,-.9999999803841022E+00)
      beta(3)=(-.1023267035480556E-03, .9999999247031288E+00)
      beta(4)=(-.1023267035480556E-03,-.9999999247031288E+00)
      beta(5)=(-.2744661422580783E-03, .9999998277644731E+00)
      beta(6)=(-.2744661422580783E-03,-.9999998277644731E+00)
      beta(7)=(-.6185331183100160E-03, .9999993934564934E+00)
      beta(8)=(-.6185331183100160E-03,-.9999993934564934E+00)
      beta(9)=(-.1293014263135570E-02, .9999973246515538E+00)
      beta(10)=(-.1293014263135570E-02,-.9999973246515538E+00)
      beta(11)=(-.2607493030160015E-02, .9999910719461364E+00)
      beta(12)=(-.2607493030160015E-02,-.9999910719461364E+00)
      beta(13)=(-.5163854131302370E-02, .9999735586993596E+00)
      beta(14)=(-.5163854131302370E-02,-.9999735586993596E+00)
      beta(15)=(-.1013381010454508E-01, .9999195856921692E+00)
      beta(16)=(-.1013381010454508E-01,-.9999195856921692E+00)
      beta(17)=(-.1979469547015627E-01, .9997465694790964E+00)
      beta(18)=(-.1979469547015627E-01,-.9997465694790964E+00)
      beta(19)=(-.3855416231134041E-01, .9991412761425118E+00)
      beta(20)=(-.3855416231134041E-01,-.9991412761425118E+00)
      beta(21)=(-.7487966608350244E-01, .9968954030259501E+00)
      beta(22)=(-.7487966608350244E-01,-.9968954030259501E+00)
      beta(23)=(-.1446954921242388E+00, .9885229324572602E+00)
      beta(24)=(-.1446954921242388E+00,-.9885229324572602E+00)
      beta(25)=(-.2757301824230285E+00, .9580367554955076E+00)
      beta(26)=(-.2757301824230285E+00,-.9580367554955076E+00)
      beta(27)=(-.5026369164103634E+00, .8538756767749224E+00)
      beta(28)=(-.5026369164103634E+00,-.8538756767749224E+00)
      beta(29)=(-.8041374504940593E+00, .5573022536695248E+00)
      beta(30)=(-.8041374504940593E+00,-.5573022536695248E+00)
      beta(31)=(-.9694745780110367E+00, .0000000000000000E+00)
c
      if (bcinit.eq.0) then
c  initialize      
        call amcof(amc,ord)
        do i=1,m
          do j=1,31
            do k=1,md
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
      scl=2.d0*3.1415926535897932384d0/len
      do i=1,m
c
c loop over fields
c
        do k=1,n
          ploc(k)=p(k,i)
        end do
        CALL rfftf(n,ploc,fftsave) 
        do j=1,31
          do k=1,md
            w=scl*DBLE(k)        
            xfact=exp(c*beta(j)*dt*w)
            adon=xfact*phi(k,j,i)
            do l=0,ord-2
              adon=adon+xfact*amc(l)*fold(l,k,j,i)
              xfact=xfact*exp(c*beta(j)*dt*w)
            end do
            phat=DCMPLX(ploc(2*k),ploc(2*k+1)) 
            adon=adon+c*dt*amc(-1)*alpha(j)*w*w*phat
            phi(k,j,i)=adon
            if (ord.gt.2) then
            do l=ord-2,1,-1
              fold(l,k,j,i)=fold(l-1,k,j,i)
            end do
            end if
            fold(0,k,j,i)=c*dt*w*w*alpha(j)*phat
          end do
        end do
        do k=1,n
          ploc(k)=0.d0
        end do
        do j=1,31
          do k=1,md
            ploc(2*k)=ploc(2*k)+DREAL(phi(k,j,i))
            ploc(2*k+1)=ploc(2*k+1)+DIMAG(phi(k,j,i))
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

