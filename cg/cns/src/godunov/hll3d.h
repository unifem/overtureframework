c
c calculate numerical flux given solutions on the left and right,
c ul and ur, using HLL approximate Reimann solver.
c
c..calculate min/max wave speeds based on Roe average
      call roespd3d (md,m,a1,a2,a3,ul,ur,spl,spr,ier)
c
      spl=spl+a0
      spr=spr+a0
c
c..find max eigenvalue
      spmax=max(dabs(spl),dabs(spr))
      almax=max(spmax,almax)
c
      if (spr.le.0.d0) then
c
c..compute flux at right state
        call flux3d (md,m,a1,a2,a3,ur,f)
        do i=1,m
          f(i)=aj*(a0*ur(i)+f(i))
        end do
c
      else
c
c..compute flux at left state
        call flux3d (md,m,a1,a2,a3,ul,f)
        do i=1,m
          f(i)=aj*(f(i)+a0*ul(i))
        end do
c
        if (spl.lt.0.d0) then
c
c..compute flux at right state
          call flux3d (md,m,a1,a2,a3,ur,alpha)
          do i=1,m
            alpha(i)=aj*(a0*ur(i)+alpha(i))
          end do
c
c..compute flux at HLL average state
          t0=1.d0/(spr-spl)
          t1=spr*t0
          t2=spl*t0
          t3=spl*t1*aj
          do i=1,m
            f(i)=t1*f(i)-t2*alpha(i)+t3*(ur(i)-ul(i))
          end do
c
        end if
      end if

