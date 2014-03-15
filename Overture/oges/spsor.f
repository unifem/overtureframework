c------------------------------------------------------------------
c
c------------------------------------------------------------------
      subroutine spsor( n,nz,ia,ja,a,b,x,omega,init,nit,tol,
     & iter,cormx,debug,ierr )
c================================================================
c Sparse SOR solver
c
c Input-
c  init : =1 on first call
c  omega : SOR acceleration parameter, typically 1 < omega < 2
c          omega=1 is Gauss Seidel
c  nit   : maximum number of iterations
c  tol   : iterate until maximum residual is less than tol
c  n,nz,ia,ja,a,b,x
c
c Output -
c  init  : =0 if init=1
c  ierr  :  1 : error, no diagonal entry
c           2 : error, tolerance not reached in nit iterations
c
c  iter  : number of iterations used
c  cormx : maximum correction on last iteration
c
c  NOTES
c   Matrix is stored in the following format
c
c      a(ia(i)),  a(ia(i)+1),  ..., a(ia(i+1)-1),
c    and the corresponding column indices are stored consecutively in
c      ja(ia(i)), ja(ia(i)+1), ..., ja(ia(i+1)-1).
c==================================================================
      integer ia(n+1),ja(nz),nit,debug
      real a(nz),b(n),x(n),tol,cormx

c.......local
      logical d
c.......pass the error message to cgeser by common
      character*80 errmes
      common/cgescb/ errmes
c.......start statement functions
      d(i)=mod(debug/2**i,2).eq.1
c.......end statement functions

      ierr=0
c........make the diagonal element the first entry
      if( init.eq.1 )then
        if( d(2) )then
          write(*,'('' SPSOR: Initialization'')')
        end if
        do i=1,n
          do j=ia(i),ia(i+1)-1
            if( ja(j).eq.i )then
c             swap with first element
              tmp=a(j)
              ja(j)=ja(ia(i))
              a(j)=a(ia(i))
              ja(ia(i))=i
              a(ia(i))=tmp
              goto 120
            end if
          end do
          write(*,*) 'SPSOR: Error No diagonal entry in row i=',i
          ierr=1
          return
 120      continue
        end do
        init=0
      end if

      itp=10
      if( d(2) )then
        write(*,'('' SPSOR: SOR iteration, nit,omega ='',i6,f6.3)')
     &   nit,omega
      end if
      resmx0=1.
      cormx0=1.
      do it=1,nit
        cormx=0.
        resmx=0.
        do i=1,n
          iai=ia(i)
          res=b(i)
          do j=iai+1,ia(i+1)-1
            res=res-a(j)*x(ja(j))
          end do
          cor=omega*( res/a(iai)-x(i) )
          cormx=max(cormx,abs(cor))
          resmx=max(resmx,abs(res-a(iai)*x(i)))
*           x(i)=(1.-omega)*x(i)+omega*res/a(iai)
          x(i)=x(i)+cor
        end do
        if(d(2)) write(*,*) 'SPSOR: it,cormx =',it,cormx
*         if( cormx.lt.tol )then
        if( resmx.lt.tol )then
          iter=it
          goto 200
        end if
        if( it.eq.1 )then
          cormx0=cormx
          resmx0=resmx
        end if

        if( d(2) .and. mod(it,itp).eq.0 )then
          cr=(cormx/cormx0)**(1./itp)
          cormx0=cormx
          write(*,
     &    '('' SOR:it ='',i4,'' resmx='',e10.2,'' cormx ='',e10.2,'//
     &    ''' Conv Rate='',e10.2)') it,resmx,cormx,cr
        end if
      end do
      write(*,*) 'SPSOR: unable to achieve tol...'
      ierr=0
 200  continue
      iter=nit
      if( d(1) )then
        cr=(resmx/resmx0)**(1./it)
        write(*,
     &  '('' SOR:it ='',i4,'' resmx='',e10.2,'' cormx ='',e10.2,'//
     &  ''' Conv Rate='',e10.2)') it,resmx,cormx,cr
      end if
      end
