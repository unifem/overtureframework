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
      call roeavg (m,a2,a3,ul,ur,al(1,1),el,er)
      do i=1,m
        al(i,1)=a1+al(i,1)
        almax=max(dabs(al(i,1)),almax)
      end do
      if( almax.gt.1.e5 )then
        write(*,*) 'ERROR: alamx=',almax
        write(*,*) 'ul',ul
        write(*,*) 'ur',ur
        stop 1
      end if
c
c..check sign of particle path
      if (al(2,1).ge.0.d0) then
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
c
c..compute flux at left state
        call flux (m,a2,a3,ul,f)
c
c..compute C- characteristic for both states
        call eigenr (m,a1,a2,a3,ul,ur,all,alr,-1)
c
c..decide whether to take ul, ur, or sonic fix
        if (all.lt.0.d0.and.alr.gt.0.d0) then
          theta=(alr-al(1,1))/(alr-all)
          theta=max(min(theta,1.d0),0.d0)
c         write(6,*)'C-, theta'
        else
          if (al(1,1).ge.0.d0) then
            theta=0.d0
c         write(6,*)'C-, theta=0'
          else
            theta=1.d0
c         write(6,*)'C-, theta=1'
          end if
        end if
c
c..compute flux
        alp=alph*al(1,1)*theta
        do i=1,m
          f(i)=a0*(a1*ul(i)+f(i)+alp*er(i,1))
        end do
c
      else
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
c
c..compute flux at right state
        call flux (m,a2,a3,ur,f)
c
c..compute C+ characteristic for both states
        call eigenr (m,a1,a2,a3,ul,ur,all,alr,+1)
c
c..decide whether to take ul, ur, or sonic fix
        if (all.lt.0.d0.and.alr.gt.0.d0) then
          theta=(al(m,1)-all)/(alr-all)
          theta=max(min(theta,1.d0),0.d0)
c         write(6,*)'C+, theta'
        else
          if (al(m,1).ge.0.d0) then
            theta=1.d0
c         write(6,*)'C+, theta=1'
          else
            theta=0.d0
c         write(6,*)'C+, theta=0'
          end if
        end if
c
c..compute flux
        alp=alph*al(m,1)*theta
        do i=1,m
          f(i)=a0*(a1*ur(i)+f(i)-alp*er(i,m))
        end do
c
      end if
