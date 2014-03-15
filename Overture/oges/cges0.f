      subroutine cgesi( id,rd,cgdir,wdir,ptr,flags,ip,rp,ierr )
c=====================================================================
c   Initialization routine for CGES
c
c=====================================================================
      implicit integer (a-z)
      integer id(*),cgdir,wdir,ptr,ip(*),ierr
      real rp(*),rd(*)
      character*(*) flags
c........local
      integer dim(5)
      real r1mach,d1mach,oneps
c Pointers :
      include 'cgesp.h'
c.......pass the error message to cgeser by common (aaarrrggg)
      character*80 errmes
      common/cgescb/ errmes
c.......start statement functions
      p(i)        =id(ptr+i)
      ndrsab(kd,ks,k)=id(pndr +kd-1+id(p(nd))*(ks-1+2*(k-1)))
      nrsab(kd,ks,k) =id(pnr  +kd-1+id(p(nd))*(ks-1+2*(k-1)))
      pndrs3(kd,ks,k)=p(ndrs3)+kd-1+3*(ks-1+2*(k-1))
      ndr(kd,k)=id(pndrs3(kd,2,k))-id(pndrs3(kd,1,k))+1
      bc(kd,ks,k)=id(pbc+kd-1+id(p(nd))*(ks-1+2*(k-1)))
c.......end statement functions

*       write(*,*) 'Entering CGESI...'
c     Create the array of pointers on the work directory
      dim(1)=1
      dim(2)=numptr
      call dskdef(id,wdir,'pointers','P',dim,ptr,ierr)
      ptr=ptr-1
c.......look up composite grid variables
      id(ptr+nd)=dskfnd(id,cgdir,'nd')
      id(ptr+ng)=dskfnd(id,cgdir,'ng')
      pbc=dskfnd(id,cgdir,'bc')
      id(ptr+ndrs3)=dskloc(id,cgdir,'ndrs3')
      if( p(ndrs3).eq.0 )then
c       ...make the ndrs3 array
        call cgndrs3( id,cgdir,ierr )
        id(ptr+ndrs3)=dskfnd(id,cgdir,'ndrs3')
      end if

c..........decode the flags
      call dskdef(id,wdir,'solver','I',0,id(ptr+solver),ierr)
      call dskdef(id,wdir,'idopt' ,'I',0,id(ptr+idopt ),ierr)
      call dskdef(id,wdir,'job'   ,'I',0,id(ptr+job   ),ierr)
      call dskdef(id,wdir,'nv'    ,'I',0,id(ptr+nv    ),ierr)
      call dskdef(id,wdir,'debug' ,'I',0,id(ptr+debug ),ierr)
      call dskdef(id,wdir,'zratio','R',0,id(ptr+zratio),ierr)
      call dskdef(id,wdir,'fratio','R',0,id(ptr+fratio),ierr)
      call dskdef(id,wdir,'fratio2','R',0,id(ptr+fratio2),ierr)

c     ispfmt = sparse storage format
c         = 0 : ia() stored in compressed mode
c         = 1 : ia() stored in uncompressed mode
      call dskdef(id,wdir,'ispfmt','I',0,id(ptr+ispfmt),ierr)
      id(p(ispfmt))=0  ! default is compressed

c      default solver is Yale:
      id(p(solver))=1
      if( flags(1:1).eq.'H' )then
c           Harwell
        id(p(solver))=2
        id(p(ispfmt))=1  ! uncompressed ia()
c       ... tol holds uh : pivoting parameter for harwell
        call dskdef(id,wdir,'tol','R',0,id(ptr+tol),ierr)
        if( flags(9:9).eq.'P' )then
          rd(p(tol))=rp(3)
        else
          rd(p(tol))=.1
        end if
      elseif( flags(1:1).eq.'C' )then
c         Conjugate Gradient
        id(p(solver))=3
c       ...SLAP:
c         icg : = 0 : SLAP: bi-conjugate gradient
c               = 1 : SLAP: bi-conjugate gradient squared
c               = 2 : SLAP: GMRES
c         ipc : =0 : diagonal scaled preconditioner
c               =1 : incomplete LU preconditioner
c       ...ESSL:
c         icg =11 : conjugate gradient
c             =12 : bi-conjugate gradient squared
c             =13 : GMRES
c             =14 : CGSTAB, smoothly converging CGS
c         ipc =11 : no preconditioning
c             =12 : Diagonal
c             =13 : SSOR
c             =14 : ILU
        call dskdef(id,wdir,'icg','I',0,id(ptr+icg),ierr)
        call dskdef(id,wdir,'ipc','I',0,id(ptr+ipc),ierr)
        id(p(icg))=ip(6)
        id(p(ipc))=ip(7)
        if( id(p(icg)).le.10 )then
          id(p(ispfmt))=1  ! SLAP stores uncompressed ia()
        end if
        call dskdef(id,wdir,'nit','I',0,id(ptr+nit),ierr)
        call dskdef(id,wdir,'nsave','I',0,id(ptr+nsave),ierr)
        call dskdef(id,wdir,'tol','R',0,id(ptr+tol),ierr)
        if( flags(9:9).eq.'P' )then
          id(p(nit))=ip(8)
          if( ip(9).gt.0 )then
            id(p(nsave))=ip(9)  ! nsave for GMRES
          else
            id(p(nsave))=20
          end if
          rd(p(tol))=rp(3)
        else
          id(p(nit))=0
          rd(p(tol))=0.
        end if
      elseif( flags(1:1).eq.'S' )then
c         SOR
        id(p(solver))=4
        call dskdef(id,wdir,'nit','I',0,id(ptr+nit),ierr)
        call dskdef(id,wdir,'tol','R',0,id(ptr+tol),ierr)
        call dskdef(id,wdir,'omega','R',0,id(ptr+omega),ierr)
        if( flags(9:9).eq.'P' )then
          id(p(nit))=ip(8)
          rd(p(tol))=rp(3)
          rd(p(omega))=rp(4)
        else
          id(p(nit))=0
          rd(p(tol))=0.
          rd(p(omega))=0.
        end if
      end if

      if( flags(2:2).eq.'J' )then
        id(p(job))=ip(1)
      else
c      Default value for job
c        1=create work spaces
c        2=generate matrix and factor
c        4=re-order (if available)
        id(p(job))=1+2+4
      end if
c.......Discretization option
c     ...default: 2nd order, apply BC at boundary, extrapolate
c        first line outside
      id(p(idopt))=16
      if( flags(3:3).eq.'D' )then
        id(p(idopt))=ip(2)
      end if
c.......Number of vector components
      id(p(nv))=1
      if( flags(4:4).eq.'V' )then
        id(p(nv))=ip(3)
      end if
c.......debug option
      id(p(debug))=0
      if( flags(5:5).eq.'D' )then
        id(p(debug))=ip(4)
      end if
c     ...Ratio's for allocating storage
c                       number of entries in LU decomposition
c            fratio >=  -------------------------------------
c                            number of unknowns
      rd(p(zratio))=0.
c     ...default for fratio depends on 2nd or 4th order
c        and number of space dimensions
      if( mod(id(p(idopt)),2).eq.0 )then
        rd(p(fratio))=3**id(p(nd))
      else
        rd(p(fratio))=5**id(p(nd))
      end if
c     ...fratio2 is another fill-in ratio for Harwell:
      rd(p(fratio2))=2.

      if( flags(8:8).eq.'W' )then
        rd(p(zratio))=rp(1)
        if( rp(2).gt.0. )then
          rd(p(fratio))=rp(2)
        end if
        if( rp(3).gt.0. )then
          rd(p(fratio2))=rp(4)
        end if
      end if

      call dskdef(id,wdir,'neq'  ,'I',0  ,id(ptr+neq  ),ierr)
      call dskdef(id,wdir,'nqs'  ,'I',0  ,id(ptr+nqs  ),ierr)
      call dskdef(id,wdir,'nze'  ,'I',0  ,id(ptr+nze  ),ierr)
      pndr=dskfnd(id,cgdir,'ndrsab')
      pnr =dskfnd(id,cgdir,'nrsab')

c     --- count the number of equations
      neqn=0
      do k=1,id(p(ng))
        neqn=neqn+ndr(1,k)*ndr(2,k)*ndr(3,k)
      end do

c     ....Check that we have enough fictitous points for the
c         given order of accuarcy
      if( mod(id(p(idopt)),2).eq.0 )then
        i24=1
      else
        i24=2
      end if
      do k=1,id(p(ng))
        do kd=1,id(p(nd))
          if( ndrsab(kd,1,k).gt.nrsab(kd,1,k)-i24 .or.
     &        ndrsab(kd,2,k).lt.nrsab(kd,2,k)+i24 )then
            write(*,*) 'CGES:CGESI:ERROR: not enough fictitious points'
            if( i24.eq.1 )then
              write(*,*) '  CGES needs 1 line of fictitious points'
              write(*,*) '  Create the grid with NXTRA=1'
              write(*,'('' k,ndrsab='',6i6)') ((ndrsab(kdd,kss,k),
     &         kdd=1,nd),kss=1,2)
              write(*,'('' k,nrsab ='',6i6)') ((nrsab(kdd,kss,k),
     &         kdd=1,nd),kss=1,2)
              errmes='CGESI:Not enough fictitious points: needs 1 line'
            else
              write(*,*) '  CGES needs 2 lines of fictitious points'
              write(*,*) '  Create the grid with NXTRA=2'
              write(*,'('' k,ndrsab='',6i6)') ((ndrsab(kdd,kss,k),
     &         kdd=1,nd),kss=1,2)
              write(*,'('' k,nrsab ='',6i6)') ((nrsab(kdd,kss,k),
     &         kdd=1,nd),kss=1,2)
              errmes='CGESI:Not enough fictitious points: need 2 lines'
            end if
            ierr=100
            return
          end if
        end do
      end do
c     ...allocate space for the boundary preconditioner variables
      call dskdef(id,wdir,'neqp','I',0  ,id(ptr+neqp),ierr)
      call dskdef(id,wdir,'nzep','I',0  ,id(ptr+nzep),ierr)
      id(p(neqp))=0  ! assign default values
      id(p(nzep))=0
      id(ptr+iep)=1
      id(ptr+iap)=1
      id(ptr+jap)=1
      id(ptr+ap)=1
      id(ptr+rhsp)=1

c       Number of equations:
      id(p(neq))=neqn*id(p(nv))


      call dskdef(id,wdir,'lratio','I',0,id(ptr+lratio),ierr)
      call dskdef(id,wdir,'epslon','R',0,id(ptr+epslon),ierr)
c     .. get epslon=r1mach(4) = largest relative spacing
c      IBM Single Prec.: epslon=.95367E-06
      rd(p(epslon))=r1mach(4)
      id(p(lratio))=1
c     The following fudge tries to detect whether the code has been
c     compiled with an automatic double precision option in which
c     case we want to call D1MACH
c      IBM Double Prec.: epslon=.222E-15
      oneps=1.+rd(p(epslon))/1000.
      if( oneps.ne.1. )then
        rd(p(epslon))=d1mach(4)
        id(p(lratio))=2
      end if
c     ...parameter for throwing away small matrix elements:
      call dskdef(id,wdir,'epsz','R',0,id(ptr+epsz),ierr)
      if( flags(9:9).eq.'P' .and. rp(5).gt.0. )then
        rd(p(epsz))=rp(5)
      else
        rd(p(epsz))=rd(p(epslon))
      end if

c     isf = storage format
c         = 0 : allocate local space to store solution for sparse solvers
c         = 1 : no need to allocate local storage
      call dskdef(id,wdir,'isf','I',0,id(ptr+isf),ierr)
      id(p(isf))=0
      if( flags(10:10).eq.'C' )then
*       if( flags(10:10).eq.'C' .or. id(p(nv)).eq.1 )then
c        storage for u and f will be in compact form
        id(p(isf))=1
        if( id(p(nv)).gt.1 )then
          write(*,'('' CGES:CGESI:ERROR: Compact storage but nv>1'')')
          write(*,'(''  This option not implemented.'')')
          write(*,'(''  Cannot use flags(10:10)="C" with nv>1'')')
          stop 'CGESI'
        end if
      end if

c     ---iterative improvement?
      call dskdef(id,wdir,'itimp','I',0,id(ptr+itimp),ierr)
      id(p(itimp))=0
      if( flags(11:11).eq.'I' )then
        id(p(itimp))=1
      end if

      end

      subroutine cgesw1( id,rd,wdir,ptr,ierr )
c=====================================================================
c   CGES Routine
c     Allocate Work spaces for solvers
c
c=====================================================================
      implicit integer (a-z)
      integer id(*),wdir,ptr,ierr
      real rd(*)
c........local
      parameter( yale=1,harwell=2,bcg=3,sor=4 )
      integer dim(5)
c Pointers :
      include 'cgesp.h'
c.......start statement functions
      p(i)        =id(ptr+i)
c.......end statement functions

c     ...Allocate local space for solution if isf=0
c        sol : local space for solution
c        (i1v,kv) : pointers from sol back into u
      if( id(p(isf)).eq.0 )then
        nsol=id(p(neq))
      else
        write(*,'('' CGES:CGESW1: Not allocating space for sol...'')')
        nsol=1
      end if
      dim(1)=1
      dim(2)=nsol
      call dskdef(id,wdir,'i1v'  ,'I',dim,id(ptr+i1v  ),ierr)
      call dskdef(id,wdir,'kv'   ,'I',dim,id(ptr+kv   ),ierr)
      call dskdef(id,wdir,'sol'  ,'R',dim,id(ptr+sol  ),ierr)

      if( id(p(solver)).eq.yale )then
c.........Yale arrays:
c         iwk=5*neq+nqs  rwk=neq+nqs
        dim(1)=1
        dim(2)=id(p(neq))
        call dskdef(id,wdir,'perm' ,'I',dim,id(ptr+perm ),ierr)
        call dskdef(id,wdir,'iperm','I',dim,id(ptr+iperm),ierr)
        call dskdef(id,wdir,'ndia' ,'I',0  ,id(ptr+ndia ),ierr)
        call dskdef(id,wdir,'ndja' ,'I',0  ,id(ptr+ndja ),ierr)
        call dskdef(id,wdir,'nda'  ,'I',0  ,id(ptr+nda  ),ierr)
c       ---normally the rhs vector can use the same space as "sol"
c          this is not true if the rhs needs to be preconditionned
        if( id(p(isf)).ne.0 .and. mod(id(p(job))/8,2).eq.1
     &                      .and. mod(id(p(job))/16,2).eq.0 )then
          call dskdef(id,wdir,'rhs','R',dim,id(ptr+rhs),ierr)
        else
          id(ptr+rhs)=id(ptr+sol)
        end if
        id(p(ndia))=id(p(neq))+1
        dim(1)=1
        dim(2)=id(p(ndia))
        call dskdef(id,wdir,'ia','I',dim,id(ptr+ia),ierr)
        id(p(ndja))=id(p(nqs))
        dim(1)=1
        dim(2)=id(p(ndja))
        call dskdef(id,wdir,'ja','I',dim,id(ptr+ja),ierr)
        id(p(nda))=id(p(nqs))
        dim(1)=1
        dim(2)=id(p(nda))
        call dskdef(id,wdir,'a','R',dim,id(ptr+a),ierr)

      elseif( id(p(solver)).eq.harwell )then
c.........  Harwell arrays:
c         storage: iwk=15*neq+nqs+nfill     rwk=2*neq+nfill
        dim(1)=1
        dim(2)=id(p(neq))
        call dskdef(id,wdir,'wh'   ,'R',dim,id(ptr+wh   ),ierr)
        dim(2)=id(p(neq))*5
        call dskdef(id,wdir,'ikeep','I',dim,id(ptr+ikeep),ierr)
        dim(2)=id(p(neq))*8
        call dskdef(id,wdir,'iwh'  ,'I',dim,id(ptr+iwh  ),ierr)
        dim(2)=id(p(neq))
        call dskdef(id,wdir,'ndia' ,'I',0  ,id(ptr+ndia ),ierr)
        call dskdef(id,wdir,'ndja' ,'I',0  ,id(ptr+ndja ),ierr)
        call dskdef(id,wdir,'nda'  ,'I',0  ,id(ptr+nda  ),ierr)
c       ---normally the rhs vector can use the same space as "sol"
c          this is not true if the rhs needs to be preconditionned
        if( id(p(isf)).ne.0 .and. mod(id(p(job))/8,2).eq.1
     &                      .and. mod(id(p(job))/16,2).eq.0 )then
          call dskdef(id,wdir,'rhs','R',dim,id(ptr+rhs),ierr)
        else
          id(ptr+rhs)=id(ptr+sol)
        end if
        call dskdef(id,wdir,'nsp'  ,'I',0  ,id(ptr+nsp  ),ierr)
        id(p(nsp))=id(p(nqs))*rd(p(fratio))+.5
        id(p(ndia))=id(p(nqs))*rd(p(fratio2))+.5
        dim(1)=1
        dim(2)=id(p(ndia))
        call dskdef(id,wdir,'ia','I',dim,id(ptr+ia),ierr)
        id(p(ndja))=id(p(nsp))
        dim(1)=1
        dim(2)=id(p(ndja))
        call dskdef(id,wdir,'ja','I',dim,id(ptr+ja),ierr)
        id(p(nda))=id(p(nsp))
        dim(1)=1
        dim(2)=id(p(nda))
        call dskdef(id,wdir,'a','R',dim,id(ptr+a),ierr)
      elseif( id(p(solver)).eq.bcg )then
c.........  Conjugate gradient routines
c             iwk=2*neq+2*nqs  rwk=2*neq+nqs
        call dskdef(id,wdir,'ndia' ,'I',0  ,id(ptr+ndia ),ierr)
        call dskdef(id,wdir,'ndja' ,'I',0  ,id(ptr+ndja ),ierr)
        call dskdef(id,wdir,'nda'  ,'I',0  ,id(ptr+nda  ),ierr)
c       ---allocate space for the rhs
        if( id(p(isf)).eq.0 .or. mod(id(p(job))/8,2).eq.1
     &                      .and. mod(id(p(job))/16,2).eq.0 )then
          dim(1)=1
          dim(2)=id(p(neq))
          call dskdef(id,wdir,'rhs','R',dim,id(ptr+rhs),ierr)
        else
          id(ptr+rhs)=id(ptr+sol)
        end if
*         dim(1)=1
*         dim(2)=nsol
*         call dskdef(id,wdir,'rhs'  ,'R',dim,id(ptr+rhs  ),ierr)
        if( id(p(icg)).le.10 .or. mod(id(p(job))/32,2).eq.1  )then
c         ...SLAP CG stores full ia()
c         ...BUT when the transpose is needed allocate enough space
c         in ia for the whole matrix...
          id(p(ndia))=id(p(nqs))
        else
c         ...ESSL CG stores compressed ia()
          id(p(ndia))=id(p(neq))+1
        end if
        id(p(ndja))=id(p(nqs))
        id(p(nda)) =id(p(nqs))
        dim(1)=1
        dim(2)=id(p(ndia))
        call dskdef(id,wdir,'ia','I',dim,id(ptr+ia),ierr)
        dim(1)=1
        dim(2)=id(p(ndja))
        call dskdef(id,wdir,'ja','I',dim,id(ptr+ja),ierr)
        dim(1)=1
        dim(2)=id(p(nda))
        call dskdef(id,wdir,'a' ,'R',dim,id(ptr+a ),ierr)
      elseif( id(p(solver)).eq.sor )then
c.........  sor routine
c          rw=2*neq+nqs    iw=3*neq+nqs
        call dskdef(id,wdir,'ndia' ,'I',0  ,id(ptr+ndia ),ierr)
        call dskdef(id,wdir,'ndja' ,'I',0  ,id(ptr+ndja ),ierr)
        call dskdef(id,wdir,'nda'  ,'I',0  ,id(ptr+nda  ),ierr)
c       ---allocate space for the rhs
        if( id(p(isf)).eq.0 .or. mod(id(p(job))/8,2).eq.1 )then
          dim(1)=1
          dim(2)=id(p(neq))
          call dskdef(id,wdir,'rhs','R',dim,id(ptr+rhs),ierr)
        else
          id(ptr+rhs)=id(ptr+sol)
        end if
*         dim(1)=1
*         dim(2)=nsol
*         call dskdef(id,wdir,'rhs'  ,'R',dim,id(ptr+rhs  ),ierr)
        id(p(ndia))=id(p(neq))+1
        id(p(ndja))=id(p(nqs))
        id(p(nda)) =id(p(nqs))
        dim(1)=1
        dim(2)=id(p(ndia))
        call dskdef(id,wdir,'ia','I',dim,id(ptr+ia),ierr)
        dim(2)=id(p(nqs))
        call dskdef(id,wdir,'ja','I',dim,id(ptr+ja),ierr)
        call dskdef(id,wdir,'a' ,'R',dim,id(ptr+a ),ierr)

      else
        write(*,*) 'CGES:CGESWK:ERROR invalid value for solver'
        write(*,*) 'CGES:CGESWK solver =',solver
        stop 'CGES:CGESWK:ERROR invalid value for solver'
      end if

      return
      end

      subroutine cgesw2( id,rd,wdir,ptr,ierr )
c=====================================================================
c   CGES Routine
c     Allocate Work spaces for solvers
c
c  This routine knows nze, the number of non-zero elements in the matrix
c=====================================================================
      implicit integer (a-z)
      integer id(*),wdir,ptr,ierr
      real rd(*)
c........local
      parameter( yale=1,harwell=2,bcg=3,sor=4 )
      integer dim(5)
c Pointers :
      include 'cgesp.h'
c.......start statement functions
      p(i)        =id(ptr+i)
c.......end statement functions

*       write(*,*) 'CGESW2 nda,nze =',id(p(nda)),id(p(nze))
*       write(*,*) 'CGESW2 excess  =',id(p(nda))-id(p(nze))

      if( id(p(itimp)).eq.1 )then
c       --- Iterative improvement
c         ias,jas,as : saved version of ia,ja,a (by default the original
c                      version is assumed to be ok to use - Yale)
c         rhsii(neq),vii(neq),resii(neq) - work spaces, by default
c           vii and resii occupy the same space (Yale, Harwell)
        loc=dskloc(id,wdir,'rhsii')
        if( loc.eq.0 )then
          id(ptr+ias)=p(ia)
          id(ptr+jas)=p(ja)
          id(ptr+as )=p(a)
          dim(1)=1
          dim(2)=id(p(neq))
          call dskdef(id,wdir,'rhsii','R',dim,id(ptr+rhsii),ierr)
          call dskdef(id,wdir,'vii','R',dim,id(ptr+vii),ierr)
c**       call dskdef(id,wdir,'resii','R',dim,id(ptr+resii),ierr)
          id(ptr+resii)=p(vii)
        end if
      else
c         --- give the iterative refinement pointers some default
c           values (should not be used except to pass as parameters)
        id(ptr+ias)=p(ia)
        id(ptr+jas)=p(ja)
        id(ptr+as)=p(a)
        id(ptr+rhsii)=p(rhs)
        id(ptr+vii)=p(sol)
        id(ptr+resii)=p(sol)
      end if

      if( id(p(solver)).eq.yale )then
c       ...allocate extra space needed by Yale
c         iwk=5*neq+nqs  rwk=neq+nqs+nfill
        loc=dskloc(id,wdir,'nsp')
        if( loc.ne.0 )then
c         ---check if there is enough space already:
          if( id(p(nsp)).lt.id(p(nze))*rd(p(fratio))+.5 )then
            write(*,'('' CGESW2: re-allocating rsp work space'')')
            call dskdel(id,wdir,'rsp',' ',ierr )
            id(p(nsp))=id(p(nze))*rd(p(fratio))+.5
            dim(1)=1
            dim(2)=id(p(nsp))
            call dskdef(id,wdir,'rsp','R',dim,id(ptr+rsp),ierr)
          end if
        else
          call dskdef(id,wdir,'nsp','I',0,id(ptr+nsp),ierr)
          id(p(nsp))=id(p(nze))*rd(p(fratio))+.5
          dim(1)=1
          dim(2)=id(p(nsp))
          call dskdef(id,wdir,'rsp','R',dim,id(ptr+rsp),ierr)
        end if

      elseif( id(p(solver)).eq.harwell )then
c       ...allocate extra space needed by Harwell
        if( id(p(itimp)).eq.1 )then
c         --- Iterative improvement
c           ias,jas,as : new versions are needed
          loc=dskloc(id,wdir,'ias')
          if( loc.ne.0 )then
            dim(1)=1
            dim(2)=id(p(neq))+1
            call dskdef(id,wdir,'ias','I',dim,id(ptr+ias),ierr)
            dim(2)=id(p(nze))
            call dskdef(id,wdir,'jas','I',dim,id(ptr+jas),ierr)
            call dskdef(id,wdir,'as','R',dim,id(ptr+as),ierr)
          end if
        end if

      elseif( id(p(solver)).eq.bcg )then
c.........  Conjugate gradient routines
c            nel=lower triangle+diag
c            nu = upper triangle+diag
*         write(*,*) 'CGESW2 allocating work space, icg,ipc=',
*      &  id(p(icg)),id(p(ipc))
        if( id(p(icg)).eq.0 )then
          if( id(p(ipc)).eq.0 )then
c           ssdbcg:
c         ... rwork:  lenw >= 8*n.
c         ... iwork: leniw >= 10
c             storage: iwk=2*neq+2*nqs+10  rwk=2*neq+nqs+8*neq
            lenw =8*id(p(neq)) + 10
            leniw=20
          else
c           sslubc:
c         ... rwork:  lenw >= nel+nu+8*n
c         ... iwork: leniw >= nel+nu+4*n+12
c             storage: iwk=2*neq+2*nqs+9neq+nze  rwk=2*neq+nqs+5neq+nze
            lenw =id(p(nze))+9*id(p(neq))
            leniw=id(p(nze))+5*id(p(neq))+12
          end if
        elseif( id(p(icg)).eq.1 )then
          if( id(p(ipc)).eq.0 )then
c           ssdcgs:
c         ... rwork:  lenw >= 8*n.
c         ... iwork: leniw >= 10
c             storage: iwk=2*neq+2*nqs  rwk=2*neq+nqs+8neq
            lenw =8*id(p(neq)) +10
            leniw=20
          else
c           sslucs:
c         ... rwork:  lenw >= nel+nu+8*n
c         ... iwork: leniw >= nel+nu+4*n+12
c             storage: iwk=2*neq+2*nqs+9neq+nze  rwk=2*neq+nqs+5neq+nze
            lenw =id(p(nze))+9*id(p(neq))
            leniw=id(p(nze))+5*id(p(neq))+12
          end if


        elseif( id(p(icg)).eq.2 )then
c         ...GMRES
          nsave0=id(p(nsave))
          if( id(p(ipc)).eq.0 )then
c           ssdgmr:
c         ... rwork:  lenw >= 1+(nsave+7)*n.
c         ... iwork: leniw >= 30
C         Length of the real workspace, RWORK.  LENW >= 1 + N*(NSAVE+7)
C         + NSAVE*(NSAVE+3).
            lenw =1+(nsave0+7)*id(p(neq))
            leniw=30
          else
c           sslugm:
c         ... rwork:  lenw >= 1 + n*(nsave+7)+nsave*(nsave+3)+nel+nu
c         ... iwork: leniw >= nel+nu+4*n+32
C         Length of the real workspace, RWORK. LENW >= 1 + N*(NSAVE+7)
C         +  NSAVE*(NSAVE+3)+NEL+NU.
            lenw =id(p(nze))+(nsave0+8)*id(p(neq))+nsave0*(nsave0+3)+1
            leniw=id(p(nze))+5*id(p(neq))+32
          end if
        elseif( id(p(icg)).gt.10 .and. id(p(icg)).le.14 )then
c         ESSL:
c         icg =11 : conjugate gradient
c             =12 : bi-conjugate gradient squared
c             =13 : GMRES
c             =14 : CGSTAB, smoothly converging CGS
c         ipc =11 : no preconditioning
c             =12 : Diagonal
c             =13 : SSOR
c             =14 : ILU
          icg1=id(p(icg))-10
          ipc1=id(p(ipc))-10
          neq1=id(p(neq))
          nze1=id(p(nze))+id(p(neq))
          if( ipc1.eq.1 )then
             naux1=1.5*nze1+2.5*neq1+30
          elseif( ipc1.eq.2 )then
             naux1=1.5*nze1+3.5*neq1+30
          elseif( ipc1.eq.3 .or. ipc1.eq.4 )then
            naux1=3*nze1+7*neq1+60
          else
            write(*,'('' CGESL3: invalid ipc='',i6)') id(p(ipc))
            stop 'CGESL3'
          end if
          if( icg1.eq.1 )then
            naux2=4*neq1
          elseif( icg1.eq.2 )then
            naux2=7*neq1
          elseif( icg1.eq.3 )then
            naux2=(ipc1+2)*neq1+ipc1*(ipc1+4)+1
          elseif( icg1.eq.4 )then
            naux2=7*neq1
          else
            write(*,'('' CGESL3: invalid icg='',i6)') id(p(icg))
            stop 'CGESL3'
          end if
          lenw =naux1+naux2
          leniw=1
*       write(*,'('' CGESW2: naux1,naux2,nze1,neq1 ='',4i8)') naux1,
*      & naux2,nze1,neq1
        else
          write(*,'('' CGESW2: error unknown value for icg='',i6)')
     &      id(p(icg))
          stop 'CGESW2'
        end if

        loc=dskloc(id,wdir,'ndiwk')
        if( loc.ne.0 )then
          if( id(p(ndiwk)).lt.leniw )then
            write(*,'('' CGESW2: re-allocating iwk work space'')')
            call dskdel(id,wdir,'iwk',' ',ierr )
            id(p(ndiwk))=leniw
            dim(1)=1
            dim(2)=id(p(ndiwk))
            call dskdef(id,wdir,'iwk','I',dim,id(ptr+iwk),ierr)
          end if
          if( id(p(ndwk)).lt.lenw )then
            write(*,'('' CGESW2: re-allocating wk work space'')')
            call dskdel(id,wdir,'wk',' ',ierr )
            id(p(ndwk ))=lenw
            dim(1)=1
            dim(2)=id(p(ndwk))
            call dskdef(id,wdir,'wk' ,'R',dim,id(ptr+wk ),ierr)
          end if
        else
          call dskdef(id,wdir,'ndiwk' ,'I',0  ,id(ptr+ndiwk),ierr)
          call dskdef(id,wdir,'ndwk'  ,'I',0  ,id(ptr+ndwk ),ierr)
          id(p(ndiwk))=leniw
          id(p(ndwk ))=lenw
          dim(1)=1
          dim(2)=id(p(ndiwk))
          call dskdef(id,wdir,'iwk','I',dim,id(ptr+iwk),ierr)
          dim(2)=id(p(ndwk))
          call dskdef(id,wdir,'wk' ,'R',dim,id(ptr+wk ),ierr)
        end if
      end if

      return
      end


      subroutine cgesg( id,rd,cgdir,job,nd,ng,nv,ndrs3,cgde,epsz,
     & nde,ce,ie,ne,peqn, neq,nze,ndia,ia,ndja,ja,nda,a,isf,i1v,kv,
     & solver,icg,idopt,ise,debug,wdir,ispfmt,ierr )
c=====================================================================
c  Generate and load the matrix
c
c  subroutine cgde returns the equations in discrete form
c  When CGDE is called for the point (i1,i2,i3,k) it returns nv
c  discrete equations as defined by
c
c                    ne(n)
c for n=1,...,nv :    SUM  ce(i,n)* u(n(i),i1(i),i2(i),i3(i),k(i))  =
c                     i=1
c    where (n(i),i1(i),i2(i),i3(i),k(i))=(ie(1,i),ie(2,i),...,ie(5,i))
c Output
c  nze : number of non-zero entries in the matrix
c=====================================================================
      integer cgdir,job,ndrs3(3,2,ng),peqn(ng),ne(*),
     & ie(5,nde,nv),ia(ndia),ja(ndja),i1v(*),kv(*),solver,debug,
     & wdir,id(*)
      real epsz,ce(nde,nv),a(nda),rd(*)

      external cgde
c.......local
      integer yale,harwell,bcg,sor
      parameter( yale=1,harwell=2,bcg=3,sor=4 )
      integer iv(4),eqn,ip(30),eqn2,ppuc,puc
      logical d
c........start statement functions
      ndr(kd,k)=ndrs3(kd,2,k)-ndrs3(kd,1,k)+1
      eqn(n,i1,i2,i3,k)=n+ nv*(i1-ndrs3(1,1,k)+
     &               ndr(1,k)*(i2-ndrs3(2,1,k)+
     &               ndr(2,k)*(i3-ndrs3(3,1,k))))  + peqn(k)
      eqn2(n,i1,i2,i3,k)=     (i1-ndrs3(1,1,k)+
     &               ndr(1,k)*(i2-ndrs3(2,1,k)+
     &               ndr(2,k)*(i3-ndrs3(3,1,k)
     &              +ndr(3,k)*(n-1))))
      puc(k)=id(ppuc+k-1)
      uc(i1,i2,i3,n,k)=rd( puc(k)+i1-ndrs3(1,1,k)+ndr(1,k)*(
     &                            i2-ndrs3(2,1,k)+ndr(2,k)*(
     &                            i3-ndrs3(3,1,k)+ndr(3,k)*(n-1))) )
      d(i)=mod(debug/2**i,2).eq.1
c........end statement functions

      ierr=0

c*wdh**
*       if( .true. )then
*         write(*,'('' **** CGESG : dskdf at start..'')')
*         call dskdf( id,' ',6,ierr )
*       end if

      ip(1)=0
      ip(2)=cgdir
      ip(3)=nv
      ip(4)=idopt
      ising0=0  ! non singular problem
      ip(5)=ising0
      ip(6)=wdir

      if( d(2) )then
        write(1,*) 'CGESG: ising0 =',ising0
        write(1,*) 'CGESG: idopt  =',idopt
        write(1,*) 'CGESG: nd,ng,nde,nv =',nd,ng,nde,nv
        write(1,*) 'CGESG: ndia,ndja,nda =',ndia,ndja,nda
        write(1,*) 'CGESG: epsz =',epsz
      end if

      peqn(1)=0
      do k=2,ng
        peqn(k)=peqn(k-1)+nv*ndr(1,k-1)*ndr(2,k-1)*ndr(3,k-1)
      end do
c     ...some solvers store ia() in a compressed form
c           Yale : compressed
c           SOR  : compressed
c           ESSL : compressed - unless we have to form the transpose,
c                  then initially create the matrix in un-compressed form
      isparse=ispfmt   ! ispfmt=0 : compressed
      if( solver.eq.bcg .and.icg.gt.10 .and. mod(job/32,2).eq.1 )then
        isparse=1
      end if
*       if( solver.eq.yale .or. solver.eq.sor
*      &    .or. (solver.eq.bcg .and.icg.gt.10
*      &    .and. mod(job/32,2).eq.0  ) )then
*         isparse=0
*       else
*         isparse=1
*       end if
      ii=0
      do 800 k=1,ng
        iv(4)=k
        do 700 i3=ndrs3(3,1,k),ndrs3(3,2,k)
          iv(3)=i3
          do 700 i2=ndrs3(2,1,k),ndrs3(2,2,k)

            iv(2)=i2
            do 700 i1=ndrs3(1,1,k),ndrs3(1,2,k)

              iv(1)=i1

c             get equations in discrete form
c               (ignore constant term if any)
              call cgde( id,rd,ip,iv, nde,ce,ie,ne,ierr )
              if( ip(5).ne.ising0 )then
c                 save eqn number for de-singularized equation
                ising0=ip(5)
                ise=eqn(1,i1,i2,i3,k)
                write(1,*) 'CGESG: de-sing, ise =',ise
              end if

c             load the matrix
              do 600 n=1,nv
                ieqn=eqn(n,i1,i2,i3,k)
                if( ieqn.lt.0 .or. ieqn.gt.neq )then
                  write(*,*) 'CGESG:1 ieqn out of range, ieqn=',ieqn
                end if
                if( isf.eq.0 )then
                  kv(ieqn)=k
                  i1v(ieqn)=eqn2(n,i1,i2,i3,k)
                end if
                if( isparse.eq.0 ) ia(ieqn)=ii+1
                scale=0.
                do 200 i=1,ne(n)
                  scale=max(scale,abs(ce(i,n)))
 200            continue
                scale=scale*2.*epsz
                if( scale.eq.0. ) scale=epsz
                do 300 i=1,ne(n)
                  if( abs(ce(i,n)).gt.scale )then
                    jeqn=eqn(ie(1,i,n),ie(2,i,n),ie(3,i,n),ie(4,i,n),
     &                       ie(5,i,n))
                    if( jeqn.lt.0 .or. jeqn.gt.neq )then
                      write(*,*) 'CGESG: jeqn out of range, jeqn=',jeqn
                      write(1,*) 'CGESG: jeqn out of range, jeqn=',jeqn
                      write(1,*) 'CGESG: itype,ne(n),n =',ip(7),ne(n),n
                      write(1,9000) i1,i2,i3,k,i,ie(1,i,n),ie(2,i,n),
     &                 ie(3,i,n),ie(4,i,n),ie(5,i,n),ce(i,n)
 9000 format(' i1,i2,i3,k =',3i4,i3,' i,ie(5,i,n) =',i3,5i4,
     &       ' ce=',e12.5)
                    end if
                    ii=ii+1
                    if( ii.gt.ndja )then
c                     ...not enough space to store matrix
                      goto 990
                    end if
                    if( isparse.eq.1 ) ia(ii)=ieqn
                    ja(ii)=jeqn
                    a(ii)=ce(i,n)

                  end if
 300            continue
                if( ip(24).ne.0 )then
c                  ...add in a constraint equation
                  ppuc=ip(24)

c                 **** compute scale for constraints ??
                  do 500 kc=1,ng
                    do 500 i3c=ndrs3(3,1,kc),ndrs3(3,2,kc)
                      do 500 i2c=ndrs3(2,1,kc),ndrs3(2,2,kc)
                        do 500 i1c=ndrs3(1,1,kc),ndrs3(1,2,kc)
                          cdc=uc(i1c,i2c,i3c,n,kc)
                          if( abs(cdc).gt.scale )then
                            jeqn=eqn(n,i1c,i2c,i3c,kc)
                            if( jeqn.lt.0 .or. jeqn.gt.neq )then
                              write(*,*) 'CGESG:2 jeqn out of range',
     &                         ', jeqn=',jeqn
                            end if
                            ii=ii+1
                            if( ii.gt.ndja )then
                              goto 990
                            end if
                            if( isparse.eq.1 ) ia(ii)=ieqn
                            ja(ii)=jeqn
                            a(ii)=cdc
                          end if
                        continue
                      continue
                    continue
 500              continue
                end if

 600          continue
            continue
          continue
 700    continue
 800  continue
      nze=ii
      neqn=eqn(nv,ndrs3(1,2,ng),ndrs3(2,2,ng),ndrs3(3,2,ng),ng)
      if( neqn.ne.neq )then
        write(*,*) 'CGESG: neq,neqn,nv =',neq,neqn,nv
      end if
      if( isparse.eq.0 ) ia(neqn+1)=ii+1

      if( solver.eq.bcg .and. mod(job/32,2).eq.1  )then
c       When using SLAP or ESSL routines we have to form the transpose
c       of the matrix when explicitly asked job=32
*         if( icg.gt.10 )then
*           write(*,'('' CGESG: transpose for icg>10 not implemented'')')
*           stop 'CGESG'
*         end if
c       ...transpose matrix
        if(d(2)) write(*,*) 'CGESG: transposing matrix for BCG'
        do i=1,nze
          itmp=ia(i)
          ia(i)=ja(i)
          ja(i)=itmp
        end do
        if( icg.gt.10 )then
c         ===ESSL
c         ...Convert to compressed ia() use SLAP routine which
c          converts to column format (note we have switched the
c          roles of ia and ja so actually we are converting to
c          compressed row format)
*           write(*,*) 'CGESG: call ss2y to compress for ESSL...'
          write(*,*) 'CGESG: isparse =',isparse
          isym=0
          call ss2y( neq,nze,ja,ia,a,isym )

        end if
      end if

c*wdh
*       if( d(3) )then
*         if( solver.eq.yale )then
*           write(1,*) 'CGESG: After loading matrix:'
*           do 950 i=1,neqn
*             write(1,9200) i,(ja(j),a(j),j=ia(i),ia(i+1)-1)
*  950      continue
*         end if
*       end if
*  9200 format(1x,'Row i=',i4,/,(4(1x,'j=',i4,' a=',e8.2)) )
c*wdh

      return
 990  continue
      write(*,*) 'CGESG:ERROR: Not enough space to store Matrix'
      write(*,*) ' number of equations: neq        =',neq
      write(*,*) ' processing stopped at eqn ieqn  =',ieqn
      write(*,*) ' allocated space for matrix, nda =',nda
      write(*,*) ' current  zratio : nda/neq       =',nda/real(neq)
      write(*,*) ' apparent zratio : nda/ieqn      =',nda/real(ieqn)
      write(*,*) ' ***Specify a bigger zratio and rerun***'
      ierr=1
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

