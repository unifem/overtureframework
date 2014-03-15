      subroutine cgesrd( solver,neq,ia,ja,a,perm,iperm,u,f,r,
     & resmx,idebug,ierr )
c================================================================
c    Double Precision Residual Computation
c compile on IBM mainframe for auto-double : ( autodbl(dbl)
c
c  Determine the residual in the equations
c
c          r <- f - A u
c
c Input
c  solver : solver being used
c  neq,nze,ia,ja,a : matrix in sparse form
c  u,f :  solution and right hand side
c Output
c  r(i)  : residual
c  resmx : maximum residual
c
c==================================================================
      integer solver,ia(neq+1),ja(*),perm(*),iperm(*)
      real a(*),u(neq),f(neq),r(neq)
c........local
      integer yale,harwell,bcg,sor
      parameter( yale=1,harwell=2,bcg=3,sor=4 )
      double precision res,resmx2
      logical d,first
      d(i)=mod(idebug/2**i,2).eq.1
      data first/.true./

      ierr=0
      if( first.and.d(3) )then
        write(1,*) 'CGESRD: Matrix:'
        do i=1,neq
          write(1,9200) i,(ja(j),a(j),j=ia(i),ia(i+1)-1)
        end do
      end if
 9200 format(1x,'Row i=',i4,/,(4(1x,'j=',i4,' a=',e8.2)) )

      resmx2=0.
      if( solver.eq.yale .or. solver .eq.harwell )then
        do i=1,neq
          iai=ia(i)
          res=f(i)
          do j=iai,ia(i+1)-1
            res=res-dble(a(j))*dble(u(ja(j)))
          end do
          r(i)=res
          resmx2=max(resmx2,abs(res))
*           write(*,'('' i,res,u,f ='',i6,3e12.4)') i,r(i),u(i),f(i)
        end do
      else
        write(*,*) 'CGESRD: Not ready for solver =',solver
        stop 'CGESRD'
      end if
*       write(*,*) 'CGESRD: resmx2 =',resmx2
      resmx=resmx2

      first=.false.
      end
