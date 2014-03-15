c #include "OvertureDefine.h"
#ifdef OV_USE_DOUBLE
#define sslubc  dslubc
#define ssdbcg1 dsdbcg1
#define sslubc1 dslubc1
#define ssdcgs1 dsdcgs1
#define sslucs1 dslucs1
#define ssdgmr1 dsdgmr1
#define sslugm1 dslugm1
#endif


      subroutine cgesl1c( id,rd,job,neq,nqs,ndia,ia,ndja,ja,nda,a,
     & perm,iperm,sol,rhs,nsp,isp,rsp,esp,fratio,nze,debug,fratio0,
     & ierr )
c=====================================================================
c   CGESL1: Sparse Direct Solve using the Yale Sparse Matrix Routines
c
c Purpose:
c  Solve (and possibly factor) the equations
c
c   job = 2 :factor
c Input
c  pperm,iperm : coloumn permutation and inverse
c
c Output
c  ierr < 0 : insufficient storage
c=====================================================================
      integer id(*),neq,ia(ndia),ja(ndja),perm(neq),
     & iperm(neq),isp(nsp),nze,debug,esp

      real rd(*),a(nda),sol(neq),rhs(neq),rsp(nsp),fratio,fratio0
c.......local
      integer path,flag
      logical d
      character em*80
c.......pass the error message to cgeser by common
      character*80 errmes
      common/cgescb/ errmes
c........start statement functions
      d(i)=mod(debug/2**i,2).eq.1
c........end statement functions

      ierr=0
      if(d(2)) write(*,*) 'CGESL1: neq,nqs =',neq,nqs
      if(d(2)) write(*,*) 'CGESL1: ndia,ndja,nda,nsp=',ndia,ndja,nda,nsp
      tcgesl1=0.
      tyale=0.
      call secondf(time1)
      if( mod(job,2).eq.1 )then
c       ...initialize the column permutation arrays
        do m=1,neq
          perm(m)=m
          iperm(m)=m
        end do
      end if
      if( mod(job/4,2).eq.1 )then
c       ....determine a new ordering of the matrix unless the
c           rows were re-ordered by cgesodr
c       ...Reorder the matrix  (path=0 => ODRV)
        path=0
        flag=0
        call secondf(time3)
        if(d(2)) write(*,*) 'CGESL1: reordering matrix'

        call yalesp(neq,perm,perm,iperm,ia,ja,a,rhs,sol,
     &   nsp,isp,rsp,esp,path,flag,em)

        call secondf(time4)
        if(d(1)) write(*,*) 'CGESL1: time to reorder =',time4-time3
        if(d(1)) write(*,*) 'CGESL1: flag =',flag
        tyale=tyale+time4-time3
        if(flag.ne.0)then
          ierr=100
          errmes='CGESL1: '//em

          write(*,*) 'CGESL1:Error in ODRV: flag =',flag
          if( flag.le.10*neq )then
            write(*,*) '9n+k  insufficient storage in md'
            write(*,*) 'k = ',flag-9*neq
            write(*,*) 'neq=',neq,' nqs=',nqs
            write(*,*) 'nsp=',nsp,' >? 3*neq+2*k=',3*neq+2*(flag-9*neq)
          elseif( flag.eq.10*neq+1 )then
            write(*,*) ' 10n+1  insufficient storage in odrv'
          elseif( flag.eq.11*neq+1 )then
            write(*,*) '11n+1  illegal path specification'
          else
            write(*,*) 'unknown error'
          end if

          return
        end if
      elseif( mod(job/2,2).eq.1 )then
c       ===factor but no reorder -
c       ...reorder elements in the matrix to be increasing in ja()
c          in terms of the current column ordering
        call nroc( neq,iperm,ia,ja,a,rsp(1),rsp(neq+1),rsp(2*neq+1),
     &   flag )
        if( flag.ne.0 )then
          write(*,'('' CGES:CGESL:CGESL1: Error from NROC flag='''//
     &     ',i8)') flag
        end if

      end if

      if( mod(job/2,2).eq.1 )then
c       ---factor
        path=1
      else
c       ---Back-substitute only.
        path=3
        if( mod(job/32,2).eq.1 )then
c          solve transposed system
          path=4
        end if
      end if

      if( d(3) )then
        write(*,*) ' ***** CGESL1: INFO written to fort.1 **** '
      end if
      if( d(3) .and. mod(job/2,2).eq.1 )then
        write(1,9020) (m,perm(m),m=1,neq)
 9020   format(2x,'(m,perm(m)),m=1,neq   =',/,(1x,15('(',i6,i6,')')) )
        write(1,9030) (m,iperm(m),m=1,neq)
 9030   format(2x,'(m,iperm(m)),m=1,neq   =',/,(1x,15('(',i6,i6,')')))
      end if
      if( d(3) .and. mod(job/2,2).eq.1  )then
        write(1,*) '====CGESL: matrix and rhs before solve'
        do i=1,neq
          write(1,9200) i,rhs(i),(ja(j),a(j),j=ia(i),ia(i+1)-1)
        end do
      end if
 9200 format(1x,'Row i=',i4,' rhs =',e12.4,/,
     & (4(1x,'j=',i4,' a=',e8.2)) )

c.......Factor and solve or just backsolve
      call secondf(time3)

 100  continue
      flag=0
      call yalesp(neq,perm,perm,iperm,ia,ja,a,rhs,sol,
     &       nsp,isp,rsp,esp,path,flag,em)
      if(flag.ne.0)then
        write(*,*) 'CGESL1: YALE '//em
        write(*,*) 'CGESL1: flag=',flag
        write(*,*) 'CGESL1: neq =',neq
        write(*,*) 'CGESL1: nsp= ',nsp
        write(*,*) 'CGESL1: path= ',path
        write(*,'('' CGESL1: Work spaces used fillinRatio ='',f6.2)') 
     &   fratio
        
        irem=mod(flag,neq)
        itype=(flag-irem)/neq
        write(*,*) 'CGESL1: irem=mod(flag,neq)=',irem
        write(*,*) 'CGESL1: itype=(flag-irem)/neq=',itype
        if( itype.eq.3 .or. itype.eq.4 .or. 
     &      itype.eq.6 .or. itype.eq.7 .or. 
     &      itype.eq.10 )then
c         insufficient storage  
          ierr=-1
          return
        else
          stop 'CGESL1C'
        end if
      endif

      if( path.eq.1 .and. mod(job/32,2).eq.1 )then
c       ---factor and solve transpose system : we must call again to solve the transpose
        path=4
        goto 100
      end if

      call secondf(time4)
      if( mod(job/2,2).eq.1 )then
        if( d(1) )then
          write(*,*) 'CGESL1: time to factor =',time4-time3
          write(*,*) 'CGESL1: work space allocated for LU, nsp =',nsp
          write(*,*) 'CGESL1: Excess work space,           esp =',esp
          write(*,*) 'CGESL1: Number of non-zeros          nze =',nze
          write(*,'('' CGESL1: Work spaces used fratio ='',f6.2)')
     &     fratio
        end if
c        ..return fratio, fill-in ratio
        fratio0=(nsp-esp)/real(nze)
        if( d(1) )then
          write(*,'('' CGESL1: Actual Value for fillinRatio ='',f6.2)') 
     &        fratio0
        end if
      else
        if(d(2)) write(*,*) 'CGESL1: time to solve  =',time4-time3
      end if
      tyale=tyale+time4-time3
      if( d(3) .and. mod(job/2,2).eq.1 )then
        write(1,*) '====CGESL1: matrix and sol after solve'
        do i=1,neq
          write(1,9300) i,sol(i),(ja(j),a(j),j=ia(i),ia(i+1)-1)
        end do
      end if
 9300 format(1x,'Row i=',i4,' sol =',e12.4,/,
     & (4(1x,'j=',i4,' a=',e8.2)) )

c**** ???
* c------next lines needed since the yale solver does not
* c      always compute esp correctly (???)
*       if( esp.gt.0 )then
*         esp=min(esp,nsp-(12*neq+5+2*nqs))
*       end if
      if(flag.ne.0)then
        errmes='CGESL1: '//em
        ierr=200
        return
      endif


      call secondf(time2)
      tcgesl1=tcgesl1+time2-time1

      return
      end

      subroutine cgesl2c( id,rd,job,neq,nze,nqs,ndia,ia,ndja,ja,nda,a,
     & sol,rhs,ikeep,iwh,wh,fratio,fratio2,uh,ias,jas,as,itimp,debug,
     & fratio0,fratio20,ierr )
c=====================================================================
c   CGESL2: Sparse Direct Solve using the Harwell Sparse Matrix Routines
c
c Purpose:
c  Solve (and possibly factor) the equations
c
c=====================================================================
      integer id(*),neq,ia(ndia),ja(ndja),ikeep(*),iwh(*),
     & ias(*),jas(*), debug
      real rd(*),a(nda),sol(neq),rhs(neq),wh(*),fratio,fratio2,uh,
     & as(*),fratio0,fratio20
c.......local
      integer flag
      logical d
c.......pass the error message to cgeser by common
      character*80 errmes
      common/cgescb/ errmes
c     ...MA28 common block:
      logical abort1,abort2
      common /ma28fd/ eps, rmin, resid, irncp, icncp, minirn, minicn,
     * irank, abort1, abort2
c........start statement functions
      d(i)=mod(debug/2**i,2).eq.1
c........end statement functions


      if(d(2)) write(*,*) 'CGESL2: neq,nze =',neq,nze
      if(d(2)) write(*,*) 'CGESL2: ndia,ndja,nda =',ndia,ndja,nda
      tcgesl2=0.
      thar=0.

      call secondf(time1)

c.......Factor and solve or just backsolve
c........call Harwell routine MA28
      if( mod(job/2,2).eq.1 )then
c...........factor
*         write(*,*) 'CGESL2: Pivotting factor for Harwell: uh =',uh
*         write(*,*) 'CGESL2: debug,itimp=',debug,itimp
        if( d(3) )then
          write(1,*) 'CGESL2: (ia,ja,a):'
          write(1,9200) (ia(i),ja(i),a(i),i=1,nze)
        end if
 9200   format((1x,4(i4,i4,e10.2)))

        if( itimp.eq.1 )then
c         ---for iterative refinement - save a compressed version of
c          the matrix - the matrix is assumed to be sorted by row
          write(*,*) 'CGESL2: saving matrix for iterative refinement'
          irow=1
          ias(1)=1
          do i=1,nze
            jas(i)=ja(i)
            as(i)=a(i)
            if( ia(i).gt.irow )then
c             starting a new row:
              ias(irow+1)=i
              irow=irow+1
            end if
          end do
          ias(neq+1)=nze+1
          if( d(3) )then
            write(1,*) 'CGESL2: Compact Harwell Matrix:'
            do i=1,neq
              write(1,9220) i,(jas(j),as(j),j=ias(i),ias(i+1)-1)
            end do
          end if
 9220     format(1x,'Row i=',i4,/,(4(1x,'j=',i4,' a=',e8.2)) )

        end if

c**** subroutine ma28ad(n,nz,a,licn,irn,lirn,icn,u,ikeep,iw,w,iflag)

        call secondf(time3)
        call ma28ad(neq,nze,a,nda,ia,ndia,ja,uh,ikeep,iwh,wh,flag)
        call secondf(time4)
        thar=thar+time4-time3
        if(d(1)) write(*,*) 'Time for Harwell factor =',time4-time3
*         write(*,*) 'CGESL2: MA28ad: flag:',flag
        if( d(2) )then
          write(*,*) 'CGESL2: HARWELL Factor Info'
          write(*,*) ' number of equations            neq =',neq
          write(*,*) ' number of nonzeroes            nze =',nze
          write(*,*) ' space allocated for LU, (a,ja) nda =',nda
          write(*,*) ' actual space used for LU    minicn =',minicn
          write(*,*) ' space allocated for ia, ndia       =',ndia
          write(*,*) ' actual space used for ia    minirn =',minirn
          write(*,*) ' space allocated for nonzeros    nqs=',nqs
          write(*,*) ' fratio=(space for a)/nze=minicn/nze=',
     &             minicn/real(nze)
          write(*,*) ' fratio2=(space ia)/nze=minirn/nze=',
     &            minirn/real(nze)
        end if
        fratio0=minicn/real(nze)
        fratio20=minirn/real(nze)
        if( d(1) )then
          write(*,*) ' Harwell factor used fratio =',fratio
          write(*,*) ' Actual value for fratio    =',fratio0
          write(*,*) ' Harwell factor used fratio2=',fratio2
          write(*,*) ' Actual value for fratio2   =',fratio20
        end if

        if(flag.ne.0) stop

      end if
c...........solve  mtype=1 : solve Ax=b  (as opposed to A^t x =b)
      mtype= 1
      if( mod(job/32,2).eq.1  )then
c        solve transposed system
        mtype=0
      end if
      call secondf(time3)
      call ma28cd(neq,a,nda,ja,ikeep,sol,wh,mtype)
      call secondf(time4)
      if(d(2)) write(*,*) 'Time for Harwell solve =',time4-time3
      thar=thar+time4-time3
      if( d(3) )then
        write(1,9300) (i,sol(i),i=1,neq)
      end if
 9300 format(5(' ( i=',i4,' sol =',e12.4,')') )

      call secondf(time2)
      tcgesl2=tcgesl2+time2-time1

      return
      end

      subroutine cgesl3c( id,rd,job,neq,nze,
     & ndia,ia,ndja,ja,nda,a,sol,rhs,ndiwk,iwk,ndwk,wk,nit,tol,
     & icg,ipc,debug,iter,resmx,nsave,ierr )
c=====================================================================
c  CGESL3: Sparse Direct Solve using the Bi-Conjugate Gradient
c          or Bi-Conjugate Gradient Squared
c
c Purpose:
c Input
c  nit : max number of iterations
c  tol: convergence tolerance
c  icg : = 0 : SLAP: bi-conjugate gradient
c        = 1 : SLAP: bi-conjugate gradient squared
c        = 2 : SLAP: GMRES
c        =11 : ESSL: conjugate gradient
c        =12 : ESSL: bi-conjugate gradient squared
c        =13 : ESSL: GMRES
c        =14 : ESSL: CGSTAB, smoothly converging CGS
c  ipc : =1 : diagonal scaled preconditioner
c        =2 : incomplete LU preconditioner
c
c Output
c  iter : actual number of iterations
c  resmx : maximum residual
c
c
c     parameter( lenw=ndja*2,leniw=ndja*2 )
c
c....................................................................
C     INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, ITOL, ITMAX
C     INTEGER ITER, IERR, IUNIT, LENW, IWORK(NEL+NU+4*N+2), LENIW
C     REAL B(N), X(N), A(NELT), TOL, ERR, RWORK(NEL+NU+8*N)
C
C     CALL SSLUBC(N, B, X, NELT, IA, JA, A, ISYM, ITOL, TOL,
C    $     ITMAX, ITER, ERR, IERR, IUNIT, RWORK, LENW, IWORK, LENIW)
c....................................................................
C *Usage:
C     INTEGER N, NELT, IA(NELT), JA(NELT), ISYM, ITOL, ITMAX
C     INTEGER ITER, IERR, IUNIT, LENW, IWORK(NEL+NU+4*N+2), LENIW
C     REAL B(N), X(N), A(NELT), TOL, ERR, RWORK(NEL+NU+8*N)
C
C     CALL SSLUCS(N, B, X, NELT, IA, JA, A, ISYM, ITOL, TOL,
C    $     ITMAX, ITER, ERR, IERR, IUNIT, RWORK, LENW, IWORK, LENIW)
c
c
c=====================================================================
      integer id(*),neq,ia(ndia),ja(ndja),iwk(ndiwk),debug
      real rd(*),a(nda),rhs(neq),sol(neq),wk(ndwk),tol,resmx
c.......local
      character initc*1,name*20
      integer iparm(10)
      real rparm(10)
      logical d
c.......pass the error message to cgeser by common
      character*80 errmes
      common/cgescb/ errmes
c.......start statement functions
      d(i)=mod(debug/2**i,2).eq.1
c.......end statement functions


      if(d(2)) write(*,*) 'CGESL3: neq,nze =',neq,nze
      if(d(2)) write(*,*) 'CGESL3: ndia,ndja,nda =',ndia,ndja,nda
      tcgesl3=0.

      call secondf(time1)


c      parameters for SLAP bi-conjugate gradient routine:
      isym=0
      itol=1
      itmax=nit
      if( .true. )then
        ibcgs=0  ! use bi-congjugate gradient squared
c*      else
c*        ibcgs=1  ! use bi-congjugate gradient
      end if
      if(d(2)) write(*,*) 'CGESL3: itmax =',itmax
      iout=6
      if( d(2) )then
        iunit=6
      else
        iunit=0
      end if


c.......Solve
      err=0.
      if( icg.lt.10 )then
        if( d(3) )then
          write(1,*) 'CGESL3: (ia,ja,a):'
          write(1,'(1x,4(i4,i4,e10.2))')  (ia(i),ja(i),a(i),i=1,nze)
        end if
c       ====SLAP====
        if( ibcgs.eq.1 )then
          call secondf(time3)

          call sslubc(neq, rhs, sol, nze , ia, ja, a, isym,
     &               itol, tol, itmax, iter, err, ierr, iunit,
     &               wk, ndwk, iwk, ndiwk )
          call secondf(time4)
          if(d(0)) write(iout,9300) iter,err,time4-time3
 9300     format(' SSLUBC iter=',i3,' err=',e10.1,' cpu =',e12.2)
        else

          if( mod(job/2,2).eq.1 )then
c           ...On first call:
            itmax=-itmax
          end if
          call secondf(time3)
          if( icg.eq.0 )then
c           ...Bi-Conjugate gradient
            itol=2
            if( ipc.le.1 )then
c              ...Diagonal scaling PreConditioner
              name='SSDBCG'
              call ssdbcg1( neq, rhs, sol, nze , ia, ja, a, isym,
     &               itol, tol, itmax, iter, err, ierr, iunit,
     &               wk, ndwk, iwk, ndiwk, debug )
            else
c             ...ILU PC
              name='SSLUBC'
              call sslubc1( neq, rhs, sol, nze , ia, ja, a, isym,
     &               itol, tol, itmax, iter, err, ierr, iunit,
     &               wk, ndwk, iwk, ndiwk, debug )
            end if
          elseif( icg.eq.1 )then
c           ...Bi-Conjugate gradient squared
            itol=2
            if( ipc.le.1 )then
c              ...Diagonal scaling PC
              name='SSDCGS'
              call ssdcgs1(neq,rhs,sol,nze,ia,ja,a,isym,itol,tol,
     &         itmax,iter,err,ierr,iunit,wk,ndwk,iwk,ndiwk,debug )
            else
c             ...ILU PC
              name='SSLUCS'
              call sslucs1(neq,rhs,sol,nze ,ia,ja,a,isym,
     &                   itol,tol,itmax,iter,err,ierr,iunit,
     &                   wk,ndwk,iwk,ndiwk,debug )
            end if

          elseif( icg.eq.2 )then
c           ...GMRES
            itol=0 ! use natural stopping criteria
*       write(*,'('' GMRES: nsave ='',i6)') nsave
            if( ipc.le.1 )then
c              ...Diagonal scaling PC
              name='SSDGMR'
              call ssdgmr1(neq,rhs,sol,nze,ia,ja,a,isym,nsave,itol,tol,
     &         itmax,iter,err,ierr,iunit,wk,ndwk,iwk,ndiwk,debug )
*       SUBROUTINE SSDGMR(N, B, X, NELT, IA, JA, A, ISYM, NSAVE,
*      $     ITOL, TOL, ITMAX, ITER, ERR, IERR, IUNIT, RWORK, LENW,
*      $     IWORK, LENIW )
            else
c             ...ILU PC
              name='SSLUGM'
              call sslugm1(neq, rhs, sol, nze , ia, ja, a, isym, nsave,
     &                   itol, tol, itmax, iter, err, ierr, iunit,
     &                   wk, ndwk, iwk, ndiwk, debug )
*       SUBROUTINE SSLUGM(N, B, X, NELT, IA, JA, A, ISYM, NSAVE,
*      $     ITOL, TOL, ITMAX, ITER, ERR, IERR, IUNIT, RWORK, LENW,
*      $     IWORK, LENIW )
            end if

          else
            write(*,*) 'CGESL3: error invalid icg =',icg
            stop 'CGESL3'
          end if
          call secondf(time4)


         if(d(0)) write(iout,9350) name(1:10),iter,err,time4-time3
 9350     format(1x,a10,':iter=',i3,' err=',e10.1,' cpu =',e12.2)
        end if
c       --- return max. residual ---
        resmx=err

        if( ierr.eq.2 )then
          if( err.ge.tol )then
            write(*,'(1x,a10,'': no convergence in ='',i8,'//
     &      ' '' iterations'')') name(1:10),iter
            write(*,'(1x,a10,'': err='',e10.1)') name(1:10),err
          end if
          if( err.lt.1. )then  ! make this a warning
            ierr=-2
          end if
        elseif(ierr.ne.0)then
          write(*,*) 'Error return from '//name(1:10)//', ierr =',ierr
          stop
        endif
      else
c       === ESSL ===

        if( mod(job/2,2).eq.1 )then
c         === initialization ===
          initc='I'
        else
          initc='S'
        end if
c           iparm(2) = 1 : CG
c                    = 2 : CGS
c                    = 3 : GMRES, restart after k=iparm(k) steps
c                    = 4 : CGSTAB, smoothly converging CGS
          iparm(2)=icg-10
c         ...iparm(3)= restart parameter for GMRES
          iparm(3)=5
c
c         iparm(4) = 1 : no preconditioning
c                  = 2 : Diagonal
c                  = 3 : SSOR
c                  = 4 : ILU
          iparm(4)=ipc-10
c         *****  nze(ESSL)=ia(neq+1)-1+neq ***
          nzessl=ia(neq+1)-1+neq
          if( iparm(4).eq.1 )then
             naux1=1.5*nzessl+2.5*neq+30
          elseif( iparm(4).eq.2 )then
             naux1=1.5*nzessl+3.5*neq+30
          elseif( iparm(4).eq.3 .or. iparm(4).eq.4 )then
            naux1=3*nzessl+7*neq+60
          else
            write(*,'('' CGESL3: invalid iparm(4)='',i6)') iparm(4)
          end if
          if( iparm(2).eq.1 )then
            naux2=4*neq
          elseif( iparm(2).eq.2 )then
            naux2=7*neq
          elseif( iparm(2).eq.3 )then
            naux2=(iparm(3)+2)*neq+iparm(3)*(iparm(3)+4)+1
          elseif( iparm(2).eq.4 )then
            naux2=7*neq
          else
            write(*,'('' CGESL3: invalid iparm(4)='',i6)') iparm(4)
          end if
          if( naux1+naux2.gt.ndwk )then
            write(*,'('' CGESL3: not enough work space, need: '',i8)')
     &       naux1+naux2
            write(*,'(''       : ndwk = '',i8)') ndwk
*       write(*,'('' CGESL: naux1,naux2,nzessl,neq,nze1 ='',5i8)') naux1,
*      & naux2,nzessl,neq,nze+neq
            stop
          end if

        if( d(1).and.initc.eq.'I' )then
          write(*,'('' CGESL3: neq,nze,ia(neq+1)-1+neq='',3i8)')
     &      neq,nze, ia(neq+1)-1+neq
          write(*,'('' CGESL3: naux1,naux2='',2i8)') naux1,naux2
        end if
c       ...number of iterations
        iparm(1)=nit
c       ...convergence tolerance
        rparm(1)=tol
c       ...rparm(3)= ssor acceleration parameter
        rparm(3)=1.3

        call dsris( 'G',initc,neq,a,ja,ia,rhs,sol,iparm,rparm,
     &   wk(1),naux1,wk(1+naux1),naux2 )

        iter=iparm(6)
        resmx=rparm(2)
        if( d(1) )then
          write(*,'('' DSRIS: number of iterations ='',i6)') iter
          write(*,'('' DSRIS: Error estimate ='',e12.3)') rparm(2)
        end if
      end if

c.........compute the residual
*         resmx=0.
*         do 750 i=1,neq
*           tmp=0.
*           do 700 j=ia0(i),ia0(i+1)-1
*             jj=ja0(j)
*             tmp=tmp+a0(j)*xp(jj)
*  700      continue
*           tmp=tmp-b(i)
* *           write(*,9400) i,tmp
*           resmx=max(resmx,abs(tmp))
*  750    continue
*       write(1,*) 'SPRCG Max residual =',resmx
*  9400 format(1x,' i=',i4,' residual =',e12.2)

      call secondf(time2)
      tcgesl3=tcgesl3+time2-time1

      return
      end

      subroutine cgesl4c( id,rd,job,neq,nze,ndia,ia,ndja,ja,nda,a,
     & sol,rhs,nit,tol,omega,debug,iter,resmx,ierr )
c=====================================================================
c  CGESL4: Sparse SOR
c
c Purpose:
c Input
c  tol: convergence tolerance
c
c Output -
c  iter : number of iterations taken
c  resmx : maximum residual
c
c=====================================================================
      integer id(*),neq,ia(ndia),ja(ndja),debug
      real rd(*),a(nda),rhs(neq),sol(neq),tol,resmx
c.......local
      logical d
c.......pass the error message to cgeser by common
      character*80 errmes
      common/cgescb/ errmes
c.......start statement functions
      d(i)=mod(debug/2**i,2).eq.1
c.......end statement functions


      if(d(2)) write(*,*) 'CGESL4: neq,nze =',neq,nze
      if(d(2)) write(*,*) 'CGESL4: ndia,ndja,nda =',ndia,ndja,nda

      if( mod(job/2,2).eq.1 )then
        init=1
      end if

      if( d(3) )then
        write(1,*) '====CGESL4: matrix and rhs before solve'
        do i=1,neq
          write(1,9200) i,rhs(i),(ja(j),a(j),j=ia(i),ia(i+1)-1)
        end do
      end if
 9200 format(1x,'Row i=',i4,' rhs =',e12.4,/,
     & (4(1x,'j=',i4,' a=',e8.2)) )
      tcgesl4=0.
      call secondf(time1)

      call spsor( neq,nze,ia,ja,a,rhs,sol,omega,init,nit,tol,
     & iter,resmx,debug,ierr )

      call secondf(time2)
      tcgesl4=tcgesl4+time2-time1

      return
      end

      subroutine cgeser(errmsg,ierr)
c=================================================================
c       Return the Error message from CGES
c CGES SUBROUTINE
c Input -
c  ierr : error number of CGES
c  errmsg : character string (character*80 or longer)
c Output -
c  errmsg : the error message
c Bill Henshaw 1991.
c=================================================================
      character*(*) errmsg
c.......pass the error message to cgeser by common (aaarrrggg)
      character*80 errmes
      common/cgescb/ errmes
c.......
      if( ierr.ne.0 )then
        errmsg=errmes
      else
        errmsg='CGES: successful return'
      end if
      return
      end


      subroutine ogesmatvect( n, x, y, nelt, ia, ja, a )
c ======================================================
c  Matrix vector multiply : compressed row format
c ======================================================


      integer n, nelt, ia(n+1), ja(nelt)
      real    a(nelt), x(n), y(n)

      do i=1,n
        y(i)=0.
        do j=ia(i),ia(i+1)-1
          y(i)=y(i)+a(j)*x(ja(j))      
        end do
      end do

      return
      end

      subroutine ogesresidual( n, x, b, r, nelt, ia, ja, a )
c ======================================================
c  Residual computation: r = b-Ax
c The matrix assumed to be in compressed row format
c ======================================================


      integer n, nelt, ia(n+1), ja(nelt)
      real    a(nelt), x(n), b(n), r(n)

      do i=1,n
        r(i)=0.
        do j=ia(i),ia(i+1)-1
          r(i)=r(i)+a(j)*x(ja(j))      
        end do
        r(i)=b(i)-r(i)
      end do

      return
      end

