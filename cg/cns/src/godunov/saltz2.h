c
c calculate numerical flux given solutions on the left and right,
c ul and ur, using Saltzman's Riemann solver.
c
c al(k), el(k,.), er(.,k), k=1,2,..,m are the kth eigenvalue, left
c eigenvector and right eigenvector, respectively.
c
      a10=a1/a0
      a20=a2/a0
      a30=a3/a0
c
c..calculate left and right eigenvectors at the midpoint
      do i=1,m
        f(i)=.5d0*(ul(i)+ur(i))
      end do
      call eigenv (m,a20,a30,f,al(1,1),el,er,1)
c
c..calculate eigenvalues belonging to the left and right states
      call eigenv (m,a20,a30,ul,al(1,2),el,er,0)
      call eigenv (m,a20,a30,ur,al(1,3),el,er,0)
      do i=1,m
        al(i,2)=a10+al(i,2)
        al(i,3)=a10+al(i,3)
        almax=max(dabs(al(i,2)),dabs(al(i,3)),almax)
      end do
c
c..calculate sigma (see Saltzman paper)
      call flux (m,a2,a3,ul,f)
      call flux (m,a2,a3,ur,alpha)
      sigma=0.d0
      do i=1,m
        f(i)=a1*ul(i)+f(i)
        alpha(i)=a1*ur(i)+alpha(i)
        sigma=sigma+(alpha(i)-f(i))*(ur(i)-ul(i))
      end do
      if (a0*sigma.lt.0.d0) then
        isig=-1
        do i=1,m
          f(i)=alpha(i)
        end do
      else
        isig=1
      end if
c
c..more stuff
      do i=1,m
        sum=0.d0
        do j=1,m
          sum=sum+el(i,j)*(ur(j)-ul(j))
        end do
        al1=min(isig*al(i,2),isig*al(i,3))
        al2=max(isig*al(i,2),isig*al(i,3))
        if (dabs(al1-al2).lt.eps) then
          s=1.d0
        else
          s=al1/(al1-al2)
        end if
        alpha(i)=a0*sum*max(min(s,1.d0),0.d0)
     *                  *(min(al1,0.d0)+min(al2,0.d0))/2.d0
      end do
c
c..calculate numerical flux
      do i=1,m
        sum=0.d0
        do j=1,m
          sum=sum+alpha(j)*er(i,j)
        end do
        f(i)=f(i)+sum
      end do
