c
c calculate numerical flux given solutions on the left and right,
c ul and ur, using Roe's Reimann solver with a sonic entropy fix.
c
c al(k), el(k,.), er(.,k), k=1,2,..,m are the kth eigenvalue, left
c eigenvector and right eigenvector, respectively.
c
c     idummy=0
c
      a10=a1/a0
      a20=a2/a0
      a30=a3/a0
      do i=1,m
        ul(i)=ul(i)/a0
        ur(i)=ur(i)/a0
      end do
c
c..calculate Roe average
      call roeavg (m,a20,a30,ul,ur,al(1,1),el,er)
      do i=1,m
        al(i,1)=a10+al(i,1)
        almax=max(dabs(al(i,1)),almax)
      end do
c
c..calculate L(ur-ul), where L=R^{-1}
      do j=1,m
        sum=0.d0
        do k=1,m
          sum=sum+el(j,k)*(ur(k)-ul(k))
        end do
        alpha(j)=sum
      end do
c
c..keep running total for flux f and states u (stored in el(.,1))
      call flux (m,a2,a3,ul,f)
      do i=1,m
        f(i)=a1*ul(i)+f(i)
        el(i,1)=ul(i)
      end do
c
      call eigenv (m,a20,a30,el(1,1),al(1,2),el,er,0)
      do i=1,m
        al(i,2)=a10+al(i,2)
      end do
c
c..find flux by working from the left state to the right
      do j=1,m
        do i=1,m
          el(i,1)=el(i,1)+alpha(j)*er(i,j)
        end do
        call eigenv (m,a20,a30,el(1,1),al(1,3),el,er,0)
        do i=1,m
          al(i,3)=a10+al(i,3)
        end do
        if (al(j,3).gt.0.d0.and.nonlin(j)) then
          if (al(j,2).ge.0.d0) then
c..shock or fan completely to the right => all done
            return
          else
c..fan with sonic fix then all done
            theta=(al(j,3)-al(j,1))/(al(j,3)-al(j,2))
c           write(6,*)'  Sonic fix : theta =',theta
            if (theta.lt.0.d0.or.theta.gt.1.d0) then
c             idummy=1
              theta=.5d0*(dabs(theta)-dabs(theta-1.d0)+1.d0)
c             write(6,*)'WARNING : setting theta =',theta
            end if
            alp=a0*alpha(j)*al(j,1)*theta
            do i=1,m
              f(i)=f(i)+alp*er(i,j)
            end do
            return
          end if
        else
          if (al(j,1).ge.0.d0) then
c..shock or contact to the right => all done
            return
          else
c..shock or contact or fan to the left => keep going...
            alp=a0*alpha(j)*al(j,1)
            do i=1,m
              f(i)=f(i)+alp*er(i,j)
              al(i,2)=al(i,3)
            end do
          end if
        end if
      end do
