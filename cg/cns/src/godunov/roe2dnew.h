c
c calculate numerical flux given solutions on the left and right,
c ul and ur, using Roe's Reimann solver with a sonic entropy fix.
c
c al(k), el(k,.), er(.,k), k=1,2,..,m are the kth eigenvalue, left
c eigenvector and right eigenvector, respectively.
c
c version assuming eigenvalues u-c,u,..,u,u+c where all the u
c eigenvalues are linear and u+-c are nonlinear and may require
c a sonic fix.
c
c     idummy=0
c
c..calculate Roe average
      ier=0
c
cx
c     write(6,*)'before roeavg'
      if (iflg.eq.1) then
        write(6,*)ul,ur
c       stop
      end if
cx
c
      call roeavg2d (md,m,a1,a2,ul,ur,al(1,1),el,er,pl,pr,
     *               dpl,dpr,ier)
      if (ier.ne.0) then
        write(17,*)'Error (gdflux) : ul, ur ='
        write(17,101)(ul(i),i=1,md),(ur(i),i=1,md)
  101   format(2(7(1x,f11.8),/))
        return
      end if
c
cx
c     write(6,*)'after roeavg'
cx
c
      do i=1,m
        al(i,1)=a0+al(i,1)
        almax=max(dabs(al(i,1)),almax)
      end do
c
c..check sign of particle path
      if (al(2,1).ge.0.d0) then
c
c..compute flux at left state
        call flux2d (md,m,a1,a2,ul,pl,f)
c
cx
c       write(6,*)'after flux2d'
cx
c
        do i=1,m
          f(i)=aj*(f(i)+a0*ul(i))
        end do
c
c..calculate first component of L(ur-ul), where L=R^{-1}
        alph=0.d0
        do k=1,m
          alph=alph+el(1,k)*(ur(k)-ul(k))
        end do
c
c..calculate state to the right of ul, call it ur
        do i=1,m
          ur(i)=ul(i)+alph*er(i,1)
        end do
        do i=m+1,md
          ur(i)=ul(i)
        end do
c
c update eos for this state
        ier=-1
        call getp2d (md,ur,pr,dpr,mr+2,ier)
c
c if update is okay, then compute C- characteristic for both states,
c and if update fails, then sonic fix cannot be checked.
        if (ier.eq.0) then
          call eigenr2d (md,m,a0,a1,a2,ul,ur,pl,pr,dpl,dpr,
     *                   all,alr,-1)
        else
          ier=0
          all=0.d0
          alr=0.d0
        end if
c
c..decide whether to take ul, ur, or sonic fix, compute flux
        if (all.lt.0.d0.and.alr.gt.0.d0) then
          theta=(alr-al(1,1))/(alr-all)
c         theta=max(min(theta,1.d0),0.d0)
c         write(6,*)'C-, theta'
          alp=aj*alph*all*theta
          do i=1,m
            f(i)=f(i)+alp*er(i,1)
          end do
        else
          if (al(1,1).lt.0.d0) then
c           write(6,*)'C-, theta=1'
            alp=aj*alph*al(1,1)
            do i=1,m
              f(i)=f(i)+alp*er(i,1)
            end do
          end if
        end if
c
      else
c
c..compute flux at right state
        call flux2d (md,m,a1,a2,ur,pr,f)
c
cx
c       write(6,*)'after flux2d'
cx
c
        do i=1,m
          f(i)=aj*(a0*ur(i)+f(i))
        end do
c
c..calculate last component of L(ur-ul), where L=R^{-1}
        alph=0.d0
        do k=1,m
          alph=alph+el(m,k)*(ur(k)-ul(k))
        end do
c
c..calculate state to the left of ur, call it ul
        do i=1,m
          ul(i)=ur(i)-alph*er(i,m)
        end do
        do i=m+1,md
          ul(i)=ur(i)
        end do
c
c update eos for this state
        ier=-1
        call getp2d (md,ur,pl,dpl,mr+2,ier)
c
c if update is okay, then compute C+ characteristic for both states,
c and if update fails, then sonic fix cannot be checked.
        if (ier.eq.0) then
          call eigenr2d (md,m,a0,a1,a2,ul,ur,pl,pr,dpl,dpr,
     *                   all,alr,+1)
        else
          ier=0
          all=0.d0
          alr=0.d0
        end if
c
c..decide whether to take ul, ur, or sonic fix, compute flux
        if (all.lt.0.d0.and.alr.gt.0.d0) then
          theta=(al(m,1)-all)/(alr-all)
c         theta=max(min(theta,1.d0),0.d0)
c         write(6,*)'C+, theta'
          alp=aj*alph*alr*theta
          do i=1,m
            f(i)=f(i)-alp*er(i,m)
          end do
        else
          if (al(m,1).ge.0.d0) then
c           write(6,*)'C+, theta=1'
            alp=aj*alph*al(m,1)
            do i=1,m
              f(i)=f(i)-alp*er(i,m)
            end do
          end if
        end if
c
      end if
