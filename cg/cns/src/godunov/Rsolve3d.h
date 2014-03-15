c
c calculate numerical flux given solutions on the left and right,
c ul and ur, using exact Reimann solver assuming an ideal gas.
c
      rad=dsqrt(a1**2+a2**2+a3**2)
      an0=a0/rad
      an1=a1/rad
      an2=a2/rad
      an3=a3/rad
c
c compute primitive variables for each state
      call prim3d (md,ul,ur,an1,an2,an3)
c
c compute middle pressure and velocity, and min/max wave speeds
      call middle (pm,vm,spl,spr)
c
c compute max eigenvalue
      spmax=max(dabs(a0+rad*spl),dabs(a0+rad*spr))
      almax=max(spmax,almax)
c
c compute "star" state at x/t=-an0 based on the exact solution
      if (vm+an0.gt.0.d0) then
        call upstar3d (md,ul,pm,vm,an0,an1,an2,an3,alpha,1)
      else
        call upstar3d (md,ur,pm,vm,an0,an1,an2,an3,alpha,2)
      end if
c
c compute Godunov flux
      call flux3d (md,m,a1,a2,a3,alpha,f)
      do i=1,m
        f(i)=aj*(a0*alpha(i)+f(i))
      end do
