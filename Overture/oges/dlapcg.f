c-------------------------------------------------------------------
c  this file contains slighly altered versions of various slap
c  routines. changes are made so the set-up parts of the routines
c  are only done once.
c
c  sslucs1  bi-conjugate gradient squared, ilu preconditioner
c  ssdcgs1  bi-conjugate gradient squared, diagonal preconditioner
c
c  ssdbcg1  bi-conjugate gradient, ilu preconditioner
c  sslubc1  bi-conjugate gradient, diagonal preconditioner
c
c-------------------------------------------------------------------
      subroutine dslucs1(n, b, x, nelt, ia, ja, a, isym, itol, tol,
     $     itmax,iter,err,ierr,iunit,rwork,lenw,iwork,leniw,debug )
c=====================================================================
c   bi-conjugate gradient squared with ilu preconditionning
c
c  see comments in sslucs
c  only change:
c   on first call set itmax to -itmax
c=====================================================================
      implicit double precision(a-h,o-z)
      integer n, nelt, ia(nelt), ja(nelt), isym, itol, itmax, iter
      integer ierr, iunit, lenw, leniw, iwork(leniw),debug
      double precision b(n), x(n), a(nelt), tol, err, rwork(lenw)
      external dsmv, dslui
      parameter (locrb=1, locib=11)
c
c         change the slap input matrix ia, ja, a to slap-column format.
c***first executable statement  dslucs
      ierr = 0
c*wdh
      if( itmax.gt.0 )then
*         write(*,*) 'sslucs1: skipping initialization...'
        goto 100
      else
        itmax=-itmax
      end if
c*wdh

      if( n.lt.1 .or. nelt.lt.1 ) then
         ierr = 3
         return
      endif
      call ds2y( n, nelt, ia, ja, a, isym )
c
c         count number of non-zero elements preconditioner ilu matrix.
c         then set up the work arrays.
      nl = 0
      nu = 0
      do 20 icol = 1, n
c         don't count diagonal.
         jbgn = ja(icol)+1
         jend = ja(icol+1)-1
         if( jbgn.le.jend ) then
cvd$ novector
            do 10 j = jbgn, jend
               if( ia(j).gt.icol ) then
                  nl = nl + 1
                  if( isym.ne.0 ) nu = nu + 1
               else
                  nu = nu + 1
               endif
 10         continue
         endif
 20   continue
c         
      locil = locib
      locjl = locil + n+1
      lociu = locjl + nl
      locju = lociu + nu
      locnr = locju + n+1
      locnc = locnr + n
      lociw = locnc + n
c
      locl   = locrb
      locdin = locl + nl
      locuu  = locdin + n
      locr   = locuu + nu
      locr0  = locr + n
      locp   = locr0 + n
      locq   = locp + n
      locu   = locq + n
      locv1  = locu + n
      locv2  = locv1 + n
      locw   = locv2 + n
c
c         check the workspace allocations.
      call dchkw( 'dslucs', lociw, leniw, locw, lenw, ierr, iter, err )
      if( ierr.ne.0 ) return
c
      iwork(1) = locil
      iwork(2) = locjl
      iwork(3) = lociu
      iwork(4) = locju
      iwork(5) = locl
      iwork(6) = locdin
      iwork(7) = locuu
      iwork(9) = lociw
      iwork(10) = locw
c
c         compute the incomplete lu decomposition.
      call dsilus( n, nelt, ia, ja, a, isym, nl, iwork(locil),
     $     iwork(locjl), rwork(locl), rwork(locdin), nu, iwork(lociu),
     $     iwork(locju), rwork(locuu), iwork(locnr), iwork(locnc) )
c*wdh
 100  continue
c*wdh
c         
c         perform the incomplete lu preconditioned 
c         biconjugate gradient squared algorithm.
      call dcgs(n, b, x, nelt, ia, ja, a, isym, dsmv,
     $     dslui, itol, tol, itmax, iter, err, ierr, iunit,
     $     rwork(locr), rwork(locr0), rwork(locp),
     $     rwork(locq), rwork(locu), rwork(locv1),
     $     rwork(locv2), rwork, iwork )
      return
c------------- last line of dslucs follows ----------------------------
      end



      subroutine dsdcgs1(n, b, x, nelt, ia, ja, a, isym, itol, tol,
     $     itmax,iter,err,ierr,iunit,rwork,lenw,iwork,leniw,debug )
c=====================================================================
c   bi-conjugate gradient squared with diagonal scaling
c   preconditionning
c
c  see comments in ssdcgs
c  only change:
c   on first call set itmax to -itmax
c=====================================================================
      implicit double precision(a-h,o-z)
      integer n, nelt, ia(nelt), ja(nelt), isym, itol, itmax, iter
      integer ierr, lenw, leniw, iwork(leniw),debug
      double precision b(n), x(n), a(n), tol, err, rwork(lenw)
      external dsmv, dsdi
      parameter (locrb=1, locib=11)
c
c         change the slap input matrix ia, ja, a to slap-column format.
c***first executable statement  dsdcgs
      ierr = 0
c*wdh
      if( itmax.gt.0 )then
*         write(*,*) 'sslucs1: skipping initialization...'
        goto 100
      else
        itmax=-itmax
      end if
c*wdh
      if( n.lt.1 .or. nelt.lt.1 ) then
         ierr = 3
         return
      endif
      call ds2y( n, nelt, ia, ja, a, isym )
c         
c         set up the workspace.  compute the inverse of the 
c         diagonal of the matrix.
      lociw = locib
c
      locdin = locrb
      locr  = locdin + n
      locr0 = locr + n
      locp  = locr0 + n
      locq  = locp + n
      locu  = locq + n
      locv1 = locu + n
      locv2 = locv1 + n
      locw  = locv2 + n
c
c         check the workspace allocations.
      call dchkw( 'dsdcgs', lociw, leniw, locw, lenw, ierr, iter, err )
      if( ierr.ne.0 ) return
c
      iwork(4) = locdin
      iwork(9) = lociw
      iwork(10) = locw
c
      call dsds(n, nelt, ia, ja, a, isym, rwork(locdin))
c*wdh
 100  continue
c*wdh
c         
c         perform the diagonally scaled 
c         biconjugate gradient squared algorithm.
      call dcgs(n, b, x, nelt, ia, ja, a, isym, dsmv,
     $     dsdi, itol, tol, itmax, iter, err, ierr, iunit,
     $     rwork(locr), rwork(locr0), rwork(locp),
     $     rwork(locq), rwork(locu), rwork(locv1),
     $     rwork(locv2), rwork(1), iwork(1))
      return
c------------- last line of dsdcgs follows ----------------------------
      end



      subroutine dsdbcg1(n, b, x, nelt, ia, ja, a, isym, itol, tol,
     $ itmax,iter,err,ierr,iunit,rwork,lenw,iwork,leniw,debug)
c=====================================================================
c   bi-conjugate gradient with diagonal scaling
c   preconditionning
c
c  see comments in ssdbcg
c  only change:
c   on first call set itmax to -itmax
c=====================================================================
      implicit double precision(a-h,o-z)
      integer n, nelt, ia(nelt), ja(nelt), isym, itol, itmax, iter
      integer ierr, lenw, leniw, iwork(leniw),debug
      double precision b(n), x(n), a(n), tol, err, rwork(lenw)
      external dsmv, dsmtv, dsdi
      parameter (locrb=1, locib=11)
c
c         change the slap input matrix ia, ja, a to slap-column format.
c***first executable statement  dsdbcg
      ierr = 0
c*wdh
      if( itmax.gt.0 )then
*        write(*,*) 'sslucs1: skipping initialization...'
        goto 100
      else
        itmax=-itmax
      end if
c*wdh
      if( n.lt.1 .or. nelt.lt.1 ) then
         ierr = 3
         return
      endif
      call ds2y( n, nelt, ia, ja, a, isym )
c         
c         set up the workspace.  compute the inverse of the 
c         diagonal of the matrix.
      lociw = locib
c
      locdin = locrb
      locr = locdin + n
      locz = locr + n
      locp = locz + n
      locrr = locp + n
      loczz = locrr + n
      locpp = loczz + n
      locdz = locpp + n
      locw = locdz + n
c
c         check the workspace allocations.
      call dchkw( 'dsdbcg', lociw, leniw, locw, lenw, ierr, iter, err )
      if( ierr.ne.0 ) return
c
      iwork(4) = locdin
      iwork(9) = lociw
      iwork(10) = locw
c
      call dsds(n, nelt, ia, ja, a, isym, rwork(locdin))
c*wdh
 100  continue
c*wdh
c         
c         perform the diagonally scaled biconjugate gradient algorithm.
      call dbcg(n, b, x, nelt, ia, ja, a, isym, dsmv, dsmtv,
     $     dsdi, dsdi, itol, tol, itmax, iter, err, ierr, iunit,
     $     rwork(locr), rwork(locz), rwork(locp),
     $     rwork(locrr), rwork(loczz), rwork(locpp),
     $     rwork(locdz), rwork(1), iwork(1))
      return
c------------- last line of dsdbcg follows ----------------------------
      end


      subroutine dslubc1(n, b, x, nelt, ia, ja, a, isym, itol, tol,
     $  itmax,iter,err,ierr,iunit,rwork,lenw,iwork,leniw,debug)
c=====================================================================
c   bi-conjugate gradient with ilu preconditionning
c
c  see comments in sslubc
c  only change:
c   on first call set itmax to -itmax
c=====================================================================
      implicit double precision(a-h,o-z)
      integer n, nelt, ia(nelt), ja(nelt), isym, itol, itmax, iter
      integer ierr, iunit, lenw, leniw, iwork(leniw), debug
      double precision b(n), x(n), a(nelt), tol, err, rwork(lenw)
      external dsmv, dsmtv, dslui, dsluti
      parameter (locrb=1, locib=11)
c
c         change the slap input matrix ia, ja, a to slap-column format.
c***first executable statement  dslubc
      ierr = 0
c*wdh
      if( itmax.gt.0 )then
*         write(*,*) 'sslucs1: skipping initialization...'
        goto 100
      else
        itmax=-itmax
      end if
c*wdh
      if( n.lt.1 .or. nelt.lt.1 ) then
         ierr = 3
         return
      endif
      call ds2y( n, nelt, ia, ja, a, isym )
c
c         count number of non-zero elements preconditioner ilu matrix.
c         then set up the work arrays.
      nl = 0
      nu = 0
      do 20 icol = 1, n
c         don't count diagonal.
         jbgn = ja(icol)+1
         jend = ja(icol+1)-1
         if( jbgn.le.jend ) then
cvd$ novector
            do 10 j = jbgn, jend
               if( ia(j).gt.icol ) then
                  nl = nl + 1
                  if( isym.ne.0 ) nu = nu + 1
               else
                  nu = nu + 1
               endif
 10         continue
         endif
 20   continue
c         
      locil = locib
      locjl = locil + n+1
      lociu = locjl + nl
      locju = lociu + nu
      locnr = locju + n+1
      locnc = locnr + n
      lociw = locnc + n
c
      locl = locrb
      locdin = locl + nl
      locu = locdin + n
      locr = locu + nu
      locz = locr + n
      locp = locz + n
      locrr = locp + n
      loczz = locrr + n
      locpp = loczz + n
      locdz = locpp + n
      locw = locdz + n
c
c         check the workspace allocations.
      call dchkw( 'dslubc', lociw, leniw, locw, lenw, ierr, iter, err )
      if( ierr.ne.0 ) return
c
      iwork(1) = locil
      iwork(2) = locjl
      iwork(3) = lociu
      iwork(4) = locju
      iwork(5) = locl
      iwork(6) = locdin
      iwork(7) = locu
      iwork(9) = lociw
      iwork(10) = locw
c
c         compute the incomplete lu decomposition.
      call dsilus( n, nelt, ia, ja, a, isym, nl, iwork(locil),
     $     iwork(locjl), rwork(locl), rwork(locdin), nu, iwork(lociu),
     $     iwork(locju), rwork(locu), iwork(locnr), iwork(locnc) )
c*wdh
 100  continue
c*wdh
c         
c         perform the incomplete lu preconditioned 
c         biconjugate gradient algorithm.

c     ... restore some pointers! *wdh* 980620  
      nu = iwork(4)-iwork(3)  ! locju-lociu
      locr=iwork(7)+nu        ! locu+nu
      locz=locr+n
      locp=locz+n
      locrr=locp+n
      loczz=locrr+n
      locpp=loczz+n
      locdz=locpp+n

      call dbcg(n, b, x, nelt, ia, ja, a, isym, dsmv, dsmtv,
     $     dslui, dsluti, itol, tol, itmax, iter, err, ierr, iunit,
     $     rwork(locr), rwork(locz), rwork(locp),
     $     rwork(locrr), rwork(loczz), rwork(locpp),
     $     rwork(locdz), rwork, iwork )
      return
c------------- last line of dslubc follows ----------------------------
      end


      subroutine dsdgmr1(n, b, x, nelt, ia, ja, a, isym, nsave,
     $     itol, tol, itmax, iter, err, ierr, iunit, rwork, lenw,
     $     iwork, leniw,debug )
c=====================================================================
c   gmres with diagonal preconditionning
c
c  see comments in ssdgmr
c  only change:
c   on first call set itmax to -itmax
c=====================================================================
      implicit double precision(a-h,o-z)
      integer  n, nelt, ia(nelt), ja(nelt), isym, nsave, itol,debug
      integer  itmax, iter, ierr, iunit, lenw, leniw, iwork(leniw)
      double precision b(n), x(n), a(nelt), tol, err, rwork(lenw)
      external dsmv, dsdi
      parameter (locrb=1, locib=11)
c
c         change the slap input matrix ia, ja, a to slap-column format.
c***first executable statement  dsdgmr
      ierr = 0
      err  = 0.0
      if( nsave.le.1 ) then
         ierr = 3
         return
      endif
c*wdh
      if( itmax.gt.0 )then
*        write(*,*) 'ssdgmr1: skipping initialization...'
        goto 100
      else
        itmax=-itmax
      end if
c*wdh
      call ds2y( n, nelt, ia, ja, a, isym )
c         
c         set up the workspace.  we assume maxl=kmp=nsave.
c         compute the inverse of the diagonal of the matrix.
      locigw = locib
      lociw = locigw + 20
c
      locdin = locrb
      locrgw = locdin + n
      locw = locrgw + 1+n*(nsave+6)+nsave*(nsave+3)
c
      iwork(4) = locdin
      iwork(9) = lociw
      iwork(10) = locw
c
c         check the workspace allocations.
      call dchkw( 'dsdgmr', lociw, leniw, locw, lenw, ierr, iter, err )
      if( ierr.ne.0 ) return
c
      call dsds(n, nelt, ia, ja, a, isym, rwork(locdin))
c         
c         perform the diagonaly scaled generalized minimum
c         residual iteration algorithm.  the following dgmres 
c         defaults are used maxl = kmp = nsave, jscal = 0,
c         jpre = -1, nrmax = itmax/nsave
c*wdh
 100  continue
c*wdh
c     ... restore some pointers! *wdh* 980620  
      locigw=iwork(9)-20
      locrgw=iwork(10)-(1+n*(nsave+6)+nsave*(nsave+3))

      iwork(locigw  ) = nsave
      iwork(locigw+1) = nsave
      iwork(locigw+2) = 0
      iwork(locigw+3) = -1
      iwork(locigw+4) = itmax/nsave
      myitol = 0
c      
      call dgmres( n, b, x, nelt, ia, ja, a, isym, dsmv, dsdi,
     $     myitol, tol, itmax, iter, err, ierr, iunit, rwork, rwork,
     $     rwork(locrgw), lenw-locrgw, iwork(locigw), 20,
     $     rwork, iwork )
c
      if( iter.gt.itmax ) ierr = 2
      return
c------------- last line of dsdgmr follows ----------------------------
      end

      subroutine dslugm1(n, b, x, nelt, ia, ja, a, isym, nsave,
     $     itol, tol, itmax, iter, err, ierr, iunit, rwork, lenw,
     $     iwork, leniw, debug )
c=====================================================================
c   gmres with ilu preconditionning
c
c  see comments in sslugm
c  only change:
c   on first call set itmax to -itmax
c
c*wdh on input iter=min number of iterations
c=====================================================================
      implicit double precision(a-h,o-z)
      integer  n, nelt, ia(nelt), ja(nelt), isym, nsave, itol,debug
      integer  itmax, iter, ierr, iunit, lenw, leniw, iwork(leniw)
      double precision b(n), x(n), a(nelt), tol, err, rwork(lenw)
      external dsmv, dslui
      parameter (locrb=1, locib=11)
c
c         change the slap input matrix ia, ja, a to slap-column format.
c***first executable statement  dslugm
      ierr = 0
      err  = 0.0
      if( nsave.le.1 ) then
         ierr = 3
         return
      endif
c*wdh
      if( itmax.gt.0 )then
*        write(*,*) 'sslugm1: skipping initialization...'
        goto 100
      else
        itmax=-itmax
      end if
c*wdh
      if( debug .gt. 1 )then
        call secondf(time1)
      end if
      write(*,*) 'sslugm1: (gmres, ilu) initialization... tol=',tol
      call ds2y( n, nelt, ia, ja, a, isym )
c
c         count number of non-zero elements preconditioner ilu matrix.
c         then set up the work arrays.  we assume maxl=kmp=nsave.
      nl = 0
      nu = 0
      do 20 icol = 1, n
c         don't count diagonal.
         jbgn = ja(icol)+1
         jend = ja(icol+1)-1
         if( jbgn.le.jend ) then
cvd$ novector
            do 10 j = jbgn, jend
               if( ia(j).gt.icol ) then
                  nl = nl + 1
                  if( isym.ne.0 ) nu = nu + 1
               else
                  nu = nu + 1
               endif
 10         continue
         endif
 20   continue
c         
      locigw = locib
      locil = locigw + 20
      locjl = locil + n+1
      lociu = locjl + nl
      locju = lociu + nu
      locnr = locju + n+1
      locnc = locnr + n
      lociw = locnc + n
c
      locl = locrb
      locdin = locl + nl
      locu = locdin + n
      locrgw = locu + nu
      locw = locrgw + 1+n*(nsave+6)+nsave*(nsave+3)
c
c         check the workspace allocations.
      call dchkw( 'dslugm', lociw, leniw, locw, lenw, ierr, iter, err )
      if( ierr.ne.0 ) return
c
      iwork(1) = locil
      iwork(2) = locjl
      iwork(3) = lociu
      iwork(4) = locju
      iwork(5) = locl
      iwork(6) = locdin
      iwork(7) = locu
      iwork(9) = lociw
      iwork(10) = locw
c
c         compute the incomplete lu decomposition.
      call dsilus( n, nelt, ia, ja, a, isym, nl, iwork(locil),
     $     iwork(locjl), rwork(locl), rwork(locdin), nu, iwork(lociu),
     $     iwork(locju), rwork(locu), iwork(locnr), iwork(locnc) )
      if( debug .gt. 1 )then
        call secondf(time2)
        write(*,*) 'gmres: time for init and ilu pre-conditionner= ',
     &  time2-time1
      end if
c         
c         perform the incomplet lu preconditioned generalized minimum
c         residual iteration algorithm.  the following dgmres 
c         defaults are used maxl = kmp = nsave, jscal = 0,
c         jpre = -1, nrmax = itmax/nsave
c*wdh
 100  continue
c*wdh
c     ... restore some pointers! *wdh* 980620      
c        iwork(1) = locil = locigw + 20
      locigw=iwork(1)-20
c        iwork(10) = locw = locrgw + 1+n*(nsave+6)+nsave*(nsave+3)   
      locrgw=iwork(10)-(n*(nsave+6)+nsave*(nsave+3))

      iwork(locigw  ) = nsave
      iwork(locigw+1) = nsave
      iwork(locigw+2) = 0
      iwork(locigw+3) = -1
      iwork(locigw+4) = itmax/nsave  ! max number of restarts
      myitol = 0
c      
      if( debug .gt. 1 )then
        call secondf(time1)
      end if

c      write(*,*)' gmres: nsave=',nsave,' itol=',itol,' tol=',tol,
c     & ' itmax=',itmax,' iter=',iter
c      write(*,*) 'locrgw=',locrgw,' lenw-locrgw=',lenw-locrgw,
c     & ' locigw=',locigw,' iwork(locigw)=',iwork(locigw),
c     & 'rwork(locrgw)=',rwork(locrgw)
c      write(*,*) 'n=',n,' nelt=',nelt,' isym=',isym
      call dgmres( n, b, x, nelt, ia, ja, a, isym, dsmv, dslui,
     $     myitol, tol, itmax, iter, err, ierr, iunit, rwork, rwork,
     $     rwork(locrgw), lenw-locrgw, iwork(locigw), 20,
     $     rwork, iwork )

      if( debug .gt. 1 )then
        call secondf(time2)
        write(*,*) 'gmres: time for solve = ',time2-time1
      end if
c
      if( iter.gt.itmax ) ierr = 2
      return
c------------- last line of dslugm follows ----------------------------
      end
