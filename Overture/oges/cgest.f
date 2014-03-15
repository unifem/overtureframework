      subroutine cgest( rd,id,ndisk,ncg,iotmp,ncgshow,ioshow,invert,
     &  root,mgdir, mgshow, ierr )
c====================================================================
c    Test Program for the Composite Grid Equation Solver CGES
c    --------------------------------------------------------
c
c  See the file cgap/doc/cges.tex for documentation on CGES
c
c Who to blame:
c   Bill Henshaw......................created........1991
c======================================================================
c*wdh      parameter(ndisk=10 000 000,nvd=6 ,ngd=10 )
      parameter(nvd=6 ,ngd=10 )
      implicit integer (a-z)
      character line*80,ncg*(*),ncgshow*(*),flags*80
      character*10 fvn(nvd+1),u1vn(nvd+1),u2vn(nvd+1),u3vn(nvd+1),
     & u4vn(nvd+1),u5vn(nvd+1)
      integer id(ndisk)
      integer iv0(3,nvd,ngd),dim(5),ip(20),icn(5,5),
     & pipcf,prpcf,nit,ipr(20),bc0(3,2,ngd),ips(20),ipic(20)
      real rd(ndisk/2),err(nvd,ngd),time1,time2,fx,fy,fz,rp(20),
     & rpr(20),cmax,epsc,rc(10),resmx,tol,omega,zratio,fratio,
     & fratio2,uharwell,epsz,fint(10),rps(20),rpic(20)
      logical d
c     ...parameter arrays to cgescf
      parameter( ndipcf=50,ndrpcf=50 )
      integer ipcf(ndipcf)
      real rpcf(ndrpcf)
c*wdh      equivalence (id,rd)
c*wdh      common/dskblk/ id

      external zerof,cgde,cgvfn,cgvfic
      namelist/inp/ icf,iplot,l,nv,iopt,iord,fx,fy,fz,
     & iwmax,flags,idebug,ioptr,intbc,rc,itrans,nit,tol,omega,icg,
     & ipc,zratio,fratio,uharwell,intcf,epsz,itimp,inl,
     & ipf,icopt,ioption,nx,bc0,nnit,nsit,ipcf,rpcf,fratio2,epsc,nvf,
     & idopt,ipcm,nsave
      data fvn/'f','f.1','f.2','f.3','f.4','f.5','f.6'/
      data u1vn/'u1','u1.1','u1.2','u1.3','u1.4','u1.5','u1.6'/
      data u2vn/'u2','u2.1','u2.2','u2.3','u2.4','u2.5','u2.6'/
      data u3vn/'u3','u3.1','u3.2','u3.3','u3.4','u3.5','u3.6'/
      data u4vn/'u4','u4.1','u4.2','u4.3','u4.4','u4.5','u4.6'/
      data u5vn/'u5','u5.1','u5.2','u5.3','u5.4','u5.5','u5.6'/
c.........start statement functions
      nrsab(kd,ks,k) =id(ppnrs+kd-1+nd*(ks-1+2*(k-1)))
      mrsab(kd,ks,k) =id(ppmrs+kd-1+nd*(ks-1+2*(k-1)))
      ndrsab(kd,ks,k)=id(pndrs+kd-1+nd*(ks-1+2*(k-1)))
      d(i)=mod(idebug/2**i,2).eq.1
      bc(kd,ks,k) =id(pbc+kd-1+nd*(ks-1+2*(k-1)))
c.........end statement functions
      icf=0
      iplot=0
      l=1
      nv=1
      iopt=0
      idopt=-1
      iord=2
      fx=1.
      fy=1.
      fz=1.
      iwmax=5
      flags='Y'
      idebug=1+2
      ioptr=1
      nx=0
      intbc=0
      itrans=0
      nit=0
      nsave=10  ! for GMRES
      tol=0.
      omega=0.
      icg=1
      ipc=1
      zratio=0.
      fratio=0.
      fratio2=0.
      uharwell=.1
      epsz=0.
      intcf=0
      rc(1)=0.
c*wdh      ioshow=0
      itimp=0
      inl=0
      ipf=0
      icopt=0
      ioption=0
      ipcm=0
      nx=0
      nnit=5 ! max number of iterations for Newtons method
      nsit=1 ! number of quasi-Newton steps per Newton steps
      epsc=1.e-8 ! convergence for Newton
      nvf=1  ! number of vector grid functions needed besides f

      do kd=1,3
        do ks=1,2
          do k=1,ngd
            bc0(kd,ks,k)=0
          end do
         end do
      end do

c     ---prompt for any changes to the parameters
      write(*,9500) icf,flags(1:3),iplot,l,nv
      write(*,9510) iord,fx,fy,fz,iwmax,idebug,ioptr,intbc,itrans,
     & nit,nsave,tol,omega,icg,ipc,zratio,fratio
      write(*,9520) uharwell,epsz,ioshow,itimp,inl,ipf,icopt,
     & ioption,ipcm,nx,nnit,nsit
      write(*,9530) nvf,idopt
 9500 format(5x,'Test of Composite Grid Equation Solver',/,
     &       5x,'--------------------------------------',/,
     & 1x,'         Variable                           Default',/,
     & 1x,'icf : Problem number as defined in CGESCF  = ',i6,/,
     & 1x,' =0 : Dirichlet                                  ',/,
     & 1x,' =+2 Biharmonic =+3 NL: Delta u - e**u           ',/,
     & 1x,' =+4 NL: Delta u - lambda u  eigenvalue problem  ',/,
     & 1x,' =+5 NL: Delta u - lambda e**(u/(1+bu) continuation ',/,
     & 1x,' =+6 NL: Delta u - lambda u + constraints        ',/,
     & 1x,' =+11  : Laplace + BC: u= or u.n=              ',/,
     & 1x,' =+12  : Neumann problem - add null vector as xtra eqn',/,
     & 1x,' intcf : =1 Compute integration wts.(Use with icf=12)',/,
     & 1x,'         =2 save the left null vector',/,
     & 1x,'flags : Y=Yale, H=Harwell, C=BCG           =',a3,/,
     & 1x,'Plotting Option                    iplot   = ',i6,/,
     & 1x,'  iplot = 2**1 : Grid Plot, 2**2 Contours, 2**3 Errors',/,
     & 1x,'          2**4 : Solution                     ',/,
     & 1x,'Multigrid level                    l       = ',i2,/,
     & 1x,'Vector size (nv < 4)               nv      = ',i2)
 9510 format(
     & 1x,'Order of discretization  2 or 4,   iord    = ',i2,/,
     & 1x,'fx,fy,fz : frequency of true solution = ',3f7.2,/,
     & 1x,'iwmax = maximum interpolation width        =',i2,/,
     & 1x,'idebug= debug option                       =',i2,/,
     & 1x,' 1=info 2=timing 4=diagnostics 8=print matrix       ',/,
     & 1x,'ioptr = 1 : twilightzone flow as true solution =',i2,/,
     & 1x,'      = 0 : real live run                       ',/,
     & 1x,'intbc = 0 apply BC  at boundary, eqn first line =',i2,/,
     & 1x,'      = 1 apply eqn at boundary, BC  first line =',/,
     & 1x,'rc()  = rhs for constraint equation              ',/,
     & 1x,'itrans: solve transpose if itrans=1             =',i2,/,
     & 1x,'nit,nsave,tol,omega for iterative solvers =',2i2,2f4.1/,
     & 1x,'For flags(1:1)=''C'' specify icg and ipc:      ',/,
     & 1x,'icg: icg=0:Bi-CG, icg=1:Bi-CG-squared, 2:GMRES =',i2,/,
     & 1x,'ipc: ipc=0:Diag Scaled precond., ipc=1:ILU    =',i2,/,
     & 1x,'zratio: ratio of nonzero entries to unknowns =',f7.3,/,
     & 1x,'fratio: Fillin ratio for direct solve work space =',f7.3,/,
     & 1x,'fratio2: (another fillin ratio for Harwell)      =',f7.3)
 9520 format(
     & 1x,'uharwell: pivotting level for Harwell          =',f7.3,/,
     & 1x,'epsz: parameter for throwing out small elements=',f7.3,/,
     & 1x,'ioshow>0 : save the solution in a show file ',i2,/,
     & 1x,'itimp > 0 : perform iterative improvement    ',i2,/,
     & 1x,'inl : 0=linear, 1=nonlinear problem ',i2,/,
     & 1x,'ipf : 1=perform path following (continuation) ',i2,/,
     & 1x,'icopt : Initial condition option           ',i2,/,
     & 1x,'   0: u0=0.  1: u0=1., 2: read in u0      ',/,
     & 1x,'ioption : various options                   ',i2,/,
     & 1x,'   2**0 : switch BC type for Johan                 ',/,
     & 1x,'   2**2 : DO NOT reorder with ODRV for YALE    ',/,
     & 1x,'ipcm : =1 : precondition eqns at boundary    ',i2,/,
     & 1x,'nx : number of extra equations ',i2,/,
     & 1x,'bc0(kd,ks,k) : make changes to the bc array ',/,
     & 1x,'nnit : max no. of iterations for Newtons method=',i3,/,
     & 1x,'nsit : quasi_newton steps per Newton step      =',i2,/,
     & 1x,'ipcf(),rpcf() : arrays passed to CGESCF, CGESR ')
 9530 format(
     & 1x,'nvf : number of grid fns needed u1,u2,...,u{nvf}  ',i2,/,
     & 1x,'specify idopt (overides intbc)                    ',/,
     & 5x,'>>>Enter changes to namelist inp : ',
     & '" &inp iplot=2,...,&end"')
      read(5,inp)

c     ==== Assign parameter for CGES ====

      if( idopt.eq.-1 .and. iord.eq.2 )then
c       ---specify idopt
c       ---2nd order accuracy
        if( intbc.eq.0 )then
c         2nd order, 8=apply eqn first line out
          idopt=8
        else
c         2=eqn at boundary, 4=BC on 1st line
          idopt=6
        end if
      elseif( idopt.eq.-1 .and. iord.eq.4 )then
c       ---4th order accuracy
        if( intbc.eq.0 )then
c         1=4'th order, 8=eqn one line outside, 32=extrap 2nd line
          idopt=1+8+32
        else
c         1=4'th order, 2=eqn on bound. 4=BC 1st line  32=extrap 2nd line
          idopt=39
        end if
      end if

      if( idopt.ne.0 )then
c       ---assign the discretization option
        flags(3:3)='D'
        ip(2)=idopt
      end if
      if( nv.gt.1 )then
c       ---number of components
        flags(4:4)='V'
        ip(3)=nv
      end if
      if( idebug.ne.0 )then
c       ---debug flag for diagnostic messages
        flags(5:5)='D'
        ip(4)=idebug
      end if
      if( itrans.eq.1 )then
c       ...solve transpose system ip(1)=32
        if( flags(2:2).ne.'J' )then
          flags(2:2)='J'
          ip(1)=1+2+4 +32
        else
          ip(1)=ip(1) +32
        end if
      end if
      if( mod(ioption/2**2,2).eq.1 )then
c       ...do not re-order rows with ODRV for Yale
        if( flags(2:2).ne.'J' )then
          flags(2:2)='J'
          ip(1)=1+2
        elseif( mod(ip(1)/4,2).eq.1 )then
          ip(1)=ip(1) -4
        end if
      end if
      if( ipcm.eq.1 )then
c       ...pre-condition rows at the boundary to prevent a null pivot
        if( flags(2:2).ne.'J' )then
          flags(2:2)='J'
          ip(1)=1+2+4+ 2**3
        else
          ip(1)=ip(1) +2**3
        end if
      end if
      if( itimp.gt.0 )then
c       ...perform iterative improvement
        flags(11:11)='I'
      end if

      if( zratio.gt.0. .or. fratio.gt.0. )then
c                       number of nonzero entries
c            zratio >=  -------------------------
c                          number of unknowns
c            fratio = fillin ratio for direct sparse solvers
c                   = (extra work space needed)/(no. of non-zero entries)
        flags(8:8)='W'
        rp(1)=zratio
        rp(2)=fratio
      end if

      if( flags(1:1).eq.'C' )then
c       ---Conjugate gradient options
        ip(6)=icg
        ip(7)=ipc
      end if

      if( epsz.gt.0. )then
c       ---small parameter to limit fill-in
        flags(9:9)='P'
        rp(5)=epsz
      end if

      if( flags(1:1).eq.'H' )then
c         ...specify parameters for Harwell solver
        flags(9:9)='P'
        rp(3)=uharwell
        rp(4)=fratio2
        rp(5)=epsz
      elseif( flags(1:1).eq.'C' .or. flags(1:1).eq.'S' )then
        if( nit.gt.0 .or. tol.ne.0. .or. omega.ne.0. )then
c         ...specify some parameters for iterative solvers
          flags(9:9)='P'
          ip(8)=nit
          ip(9)=nsave
          rp(3)=tol
          rp(4)=omega
          rp(5)=epsz
        end if
      end if

c............Number of multi-grid levels:
      mg=id(dskfnd(id,mgdir,'mg'))
      if( l.lt.1 .or. l.gt.mg)  then
        write(*,*) 'CGETST>>>Error invalid l, l,mg =',l,mg
        l=max(1,min(mg,l))
        write(*,*) '....Continuing with new l =',l
      end if
c............Number of space dimensions
      nd=id(dskfnd(id,mgdir,'nd'))
      if( nd.ne.2 .and. nd.ne.3 ) then
        write(*,*) 'CGETST>>>Error invalid nd =',nd
        nd=2
      end if
      write(*,*) 'Number of space dimensions = ',nd
      if( nv.gt.nvd )then
        stop 'CGETST>>>ERROR  nv > nvd'
      end if

c........Directory pointer to multigrid level l
      cgdir=dskfnd(id,mgdir,'composite grid')+l-1
c........print dimensions of grids
      ng=id(dskfnd(id,cgdir,'ng'))
      ppnrs=dskfnd(id,cgdir,'nrsab')
      ppmrs=dskfnd(id,cgdir,'mrsab')
      pndrs =dskfnd(id,cgdir,'ndrsab')

      write(*,9200)
     & (k,((ndrsab(kd,ks,k),ks=1,2),kd=1,nd),(0,0,kd=nd+1,3),
     &    ((nrsab (kd,ks,k),ks=1,2),kd=1,nd),(0,0,kd=nd+1,3),
     &    ((mrsab (kd,ks,k),ks=1,2),kd=1,nd),(0,0,kd=nd+1,3),k=1,ng)
 9200 format(15x,'Grid Dimensions',/,15x,15('-'),/,
     &  2x,'k       ndrs                         nrs          ',
     &                                '          mrs          ',/,
     &  2x,'k  ra  rb  sa  sb  ta  tb   ra  rb  sa  sb  ta  tb',
     &                              '   ra  rb  sa  sb  ta  tb',/,
     &   (1x,i2,6i4,1x,6i4,1x,6i4) )

      if( ng.gt.ngd )then
        write(*,*) 'CGEST Dimension error, ng > ngd =',ngd
        stop
      end if

c     ...Work space for CGES
      call dskdef(id,cgdir,'cges work space','D',0,wdir,ierr)

c.........Generate grid functions
c         f is used for the right hand side
c         u1 is used for the solution
c         u2 is used as the correction for nonlinear problems
c         u3 is used for the eigenvalue and continuation problems
      if( inl.eq.1 )then
        nvf=max(3,nvf)   ! at least 3 grid fns for nonlinear problems
      end if
      if( nx.gt.0 )then  ! *** needed??
        nvf=max(3,nvf)
      end if
      if( ipf.eq.1 )then
        nvf=max(4,nvf)   ! at least 4 grid fns for path following
      end if

      call cgvf( id,id,cgdir,fvn,zerof,nv,pfvn )
      call cgvf( id,id,cgdir,u1vn,zerof,nv,pu1vn )  ! initialize to zero
      pu2vn=pu1vn  ! give default values
      pu3vn=pu1vn
      pu4vn=pu1vn
      pu5vn=pu1vn
      if( nvf.ge.2 )then
        if( intcf.ge.1 )then
c       ...testing out the integration weights from CGIC
c          initialize u2 to have a function to integrate
          call cgvf( id,id,cgdir,u2vn,cgvfic,nv,pu2vn )
        else
          call cgvf( id,id,cgdir,u2vn,zerof,nv,pu2vn )
        end if
      end if
      if( nvf.ge.3 )then
c**     call cgvf( id,id,cgdir,u3vn,zerof,nv,pu3vn )
        call cgvf( id,id,cgdir,u3vn,cgvfn,nv,pu3vn )
      end if
      if( nvf.ge.4 )then
        call cgvf( id,id,cgdir,u4vn,zerof,nv,pu4vn )
      end if
      if( nvf.ge.5 )then
        call cgvf( id,id,cgdir,u5vn,zerof,nv,pu5vn )
      end if
      if( nvf.gt.5 )then
        write(*,'('' CGEST: ERROR nvf > 5 '')')
        stop 'CGEST'
      end if

c ...Pass parameter info to CGESCF using the arrays ipcf() and rpcf()
c    These arrays can be used by the USER to pass any information
c    that is needed in CGESCF when the PDE coefficients and
c    constraint functions are defined (in the include files CF?.FOR)
c      nipcf : maximum number of integer parameters
c      ipcf(i) i=1,nipcf : integer parameters
c      nrpcf : maximum number of real parameters
c      rpcf(i) i=1,nipcf : real parameters
c
c The first two entries in icpf are reserved as follows:
c   ipcf(1) = icf : specifies which problem is to be solved
c                   in CGESCF.
c   ipcf(2) = ijac : when a nonlinear problem is being solved
c           CGESCF must know whether to evaluate the function (ijac=0)
c           or to evaluate the Jacobian (ijac=1)
c

      ipcf(1)=icf
      ipcf(2)=0
      ipcf(3)=pu1vn
      ipcf(4)=pu3vn
      ipcf(5)=pu4vn
      ipcf(6)=pu5vn

      if( inl.eq.1 )then
c       ...for nonlinear problems, set ijac=1 and
c          pass pointers to u1 and u3
        inl=1
        ipcf(2)=1
        ipcf(3)=pu1vn
        ipcf(4)=pu3vn
      elseif( nx.gt.0 )then
c       ...pass pointer to the constraint equation which is stored
c          in the grid function u3
        ipcf(3)=pu3vn
      end if

      if( icopt.eq.1 )then
c       ---get initial conditions
        ipic(1)=icf
        ipic(2)=wdir
        call cgesic( id,rd,cgdir,nd,ng,nv,u1vn,ipic,rpic,
     &   ipcf,rpcf,ierr )
      end if



      nipcf=ndipcf    ! max number of entries in ipcf() array
      nrpcf=ndrpcf    ! max number of entries in rpcf() array
      call dskdef(id,wdir,'nipcf','I',0,loc,ierr)
      id(loc)=nipcf
      call dskdef(id,wdir,'nrpcf','I',0,loc,ierr)
      id(loc)=nrpcf
      dim(1)=1
      dim(2)=nipcf
      call dskdef(id,wdir,'ipcf','I',dim,pipcf,ierr)
      dim(1)=1
      dim(2)=nrpcf
      call dskdef(id,wdir,'rpcf','R',dim,prpcf,ierr)
c........pointer to ijac****
      pijac=pipcf+2-1
c     ...copy from local arrays
      do i=1,nipcf
        id(pipcf+i-1)=ipcf(i)
      end do
      do i=1,nrpcf
        rd(prpcf+i-1)=rpcf(i)
      end do

* c     ...Create ibd(kd,ks,k) : Boundary Discretization array
*       dim(1)=3
*       dim(2)=nd
*       dim(3)=2
*       dim(4)=ng
*       call dskdef(id,wdir,'ibd','I',dim,ibd,ierr)
*       do k=1,ng
*         do ks=1,2
*           do kd=1,nd
*             id(ibd+kd-1+nd*(ks-1+2*(k-1)))=idopt
*           end do
*         end do
*       end do
      pbc=dskfnd(id,cgdir,'bc')
      do k=1,ng
        do ks=1,2
          do kd=1,nd
            if( bc0(kd,ks,k).gt.0 .and. bc(kd,ks,k).gt.0 )then
              id(pbc+kd-1+nd*(ks-1+2*(k-1)))=bc0(kd,ks,k)
            end if
          end do
        end do
      end do

c******** fix this -> corners
*       if( mod(ioption,2).eq.1 )then
* c       ...In this test we apply Dirichlet BC for bc=2 and
* c          Neumann BC's otherwise
* c       ...change boundary discretization for sides with bc=2
*         do k=1,ng
*           do ks=1,2
*             do kd=1,nd
*               if( bc(kd,ks,k).eq.2 )then
* c                ...switch eqn at 0 and BC at -1 (idopt=6) to
* c                   to BC at 0 and eqn at -1 (idopt=8)
* c                   add 2**10 so this value takes precedence
* c                   at a corner
*                 id(ibd+kd-1+nd*(ks-1+2*(k-1)))=idopt-6+8+2**10
*               end if
*             end do
*           end do
*         end do
*       end if

c     ...determine locations for constraint equations
c     ...and initialize constraint grid function u3
      if( nx.gt.0 )then
        call cgce( 0,id,rd,cgdir,wdir,nd,nv,idopt,nx,id(pu3vn),icn,
     &   ierr )
      end if


c     ---for cgesr:
      ipr(1)=0
      ipr(2)=idopt
      ipr(3)=ioptr
      ipr(4)=wdir
      ipr(5)=nv
      ipr(6)=icf
      rpr(1)=fx
      rpr(2)=fy
      rpr(3)=fz

      if( inl.eq.0 .and. ipf.eq.0 )then
c       ===linear problem

c       ...Assign the right hand side
        call cgesr( id,rd,cgdir,u1vn,fvn,u2vn,ipr,rpr,rc )

c       ===call the Equation solver===
        wdir1=0
        pu1=0
        pf=0
        call second(time1)
        call cges( id,rd,cgdir,wdir1,u1vn,pu1,fvn,pf,flags,ip,rp,cgde,
     &   ierr )
        call second(time2)
        write(*,*) 'Time for CGES =',time2-time1
        if( ierr.eq.0 )then
          write(*,*) 'Calling CGES again...'
          call second(time1)
          call cges( id,rd,cgdir,wdir,u1vn,pu1,fvn,pf,flags,ip,rp,cgde,
     &     ierr )
          call second(time2)
          write(*,*) 'Time for CGES =',time2-time1
        end if
        call cgeser(line,ierr)
        write(*,*) 'CGETST>>'//line
        if(ierr.gt.0)then
          write(*,*) 'CGETST>> Fatal error from solver'
          stop 'CGEST'
        end if
        if( mod(intcf/2,2).eq.1 )then
c         ...save the left null vector
          call cgest1( id,rd,root,l,cgdir,u1vn,ng,nv,ierr )
        end if
        if( mod(intcf,2).eq.1 )then
c         ...Compute Integration coefficients
          call cgic( id,rd,cgdir,wdir,pu1,ierr )
c         ...integrate u2vn:
          call cgif( id,rd,cgdir,wdir,pu1,pu2vn,fint,ierr )
          stop
        end if

      elseif( inl.eq.1 .and. ipf.eq.0 )then
c       === Nonlinear Problem ===
        ipr(1)=1
c             u : current guess at solution
c             v : correction
c             f : right hand side


c          Iterate until convergence
        do it=1,nnit*(nsit+1)
c         ...to determine the rhs and residual evaluate F(u) instead
c            of the Jacobian
          id(pijac)=0
c         ...Assign the right hand side (forcing for twilight-zone flow)
          call cgesr( id,rd,cgdir,u1vn,fvn,u2vn,ipr,rpr,rc )
c         ...Determine the residual   f <- -F(u)
          call cgres( id,rd,cgdir,id(pu1vn),id(pfvn),id(pfvn),ipr,rpr,
     &     resmx,ierr )

          if( d(1) ) write(*,9300) it,resmx
 9300  format(' CGEST: it =',i2,' resmx =',e12.4)
          if( d(2) )then
            write(1,'('' ****Solution**** it='',i4)')  it
            call cgesprt( id,rd,cgdir,nv,u1vn )
            write(1,'('' ****Residual**** it='',i4)')  it
            call cgesprt( id,rd,cgdir,nv,fvn )
          end if

c         ...solve for the correction u2
c         ...set ijac=1 to evaluate the Jacobian
          id(pijac)=1
          wdir1=0
          pu2=0
          pf=0
          call second(time1)
          call cges( id,rd,cgdir,wdir1,u2vn,pu2,fvn,pf,flags,ip,rp,
     &     cgde,ierr )
          call second(time2)
          if( d(2) ) write(*,*) 'Time for CGES =',time2-time1
          if(ierr.gt.0)then
            write(*,*) 'CGEST>> Fatal error from solver'
            call cgeser(line,ierr)
            write(*,*) 'CGETST>>'//line
            stop 'CGEST'
          end if
          if( mod(it,nsit+1).eq.0 )then
c           ...set refactor option
            flags(6:6)='R'
          else
c           ...for quasi-Newton we only factor the first time
            flags(6:6)=' '
          end if
          ip(5)=1

          if( d(3) )then
            write(1,'('' ****Correction**** it='',i4)') it
            call cgesprt( id,rd,cgdir,nv,u2vn )
          end if

c         ...correct the solution u1 <- u1+ u2
          call cgcor( id,rd,cgdir,id(pu1vn),id(pu2vn),ipr,rpr,cmax,
     &     ierr )
          if( d(0) )then
            write(*,'('' CGEST: it ='',i2,'' max corr ='',e10.2,'//
     &       ''' max residual='',e10.2)') it,cmax,resmx
          end if

          if( abs(cmax).lt.epsc ) goto 550

        end do
 550    continue
      else
c       ===Continuation problem
c       ---call path following routine
        call cgpf( id,rd,nd,ng,nv,idopt,pu1vn,pu2vn,pfvn,pu3vn,pu4vn,
     &   id(pu1vn),id(pu2vn),id(pfvn),id(pu3vn),id(pu4vn),
     &   u1vn,u2vn,fvn,u3vn,u4vn,cgdir,wdir,flags,pijac,
     &   ip,rp,id(pipcf),rd(prpcf),idebug,icf,ierr )
      end if

      if( ioptr.eq.1 )then
c       ===Twilight-zone flow
c       ...Calculate the maximum error
        call cgmxer( id,rd,cgdir,u1vn,ipr,rpr,iplot,nx,icn,nvd,iv0,err )
c       ...Print errors
        write(*,9100)
        do n=1,nv
          do k=1,ng
            write(*,9110) n,k,err(n,k),(iv0(m,n,k),m=1,nd)
          end do
        end do
      end if
 9100 format('  n  Grid  Maximum error    i,j')
 9110 format((:i3,i5,e15.5,'  ',i3,',',i3,:',',i3))

* c     --- test cgintsp --
* c         v <- u interpolated
*       write(*,'('' CGEST: calling CGINTSP...'')')
*       ipv0=0
*       ipu0=0
*       iwptr=0
*       call cgintsp( nv,u1vn,u2vn,ipu0,ipv0,cgdir,iwptr,id,rd,ierr )
* c..........Calculate the maximum error
*       call cgmxer( id,rd,cgdir,u1vn,ipr,rpr,iplot,nx,icn,nvd,iv0,err )
* c........Print errors
*       write(*,9100)
*       do n=1,nv
*         do k=1,ng
*           write(*,9110) n,k,err(n,k),(iv0(m,n,k),m=1,nd)
*         end do
*       end do


c     === Save the solution in a show file ===
c         The solution can be plotted with CGSHOW

      if( ioshow.gt.0 )then
        cgshow=dskfnd(id,mgshow,'composite grid')+l-1
        ips(1)=icf
        ips(2)=iord
        call cgeshow( id,rd,cgdir,cgshow,u1vn,nv,ips,rps,ierr )

        write(*,*) 'CGEST: dismount show file...'
        call dskumt( id,mgshow,ierr )
        if( ierr.ne.0 )then
          write(*,*) 'CGEST:Error return from dskumt'
        end if
      end if


c*wdh      stop 'CGEST'
      end

      subroutine zerof( k,rv,xv,uv,nv,nd,cgdir,id,rd )
c==================================================================
c    Define the Composite Grid Function for CGVF
c==================================================================
      integer k,nv,nd,cgdir,id(*)
      real rv(nd),xv(nd),uv(nd),rd(*)
c.......local
      do 100 n=1,nv
        uv(n)=0.
 100  continue
      return
      end

      subroutine cgvfn( k,rv,xv,uv,nv,nd,cgdir,id,rd )
c==================================================================
c    Define the Composite Grid Function for CGVF
c==================================================================
      integer k,nv,nd,cgdir,id(*)
      real rv(nd),xv(nd),uv(nd),rd(*)
c.......local
      do 100 n=1,nv
        uv(n)=1.
 100  continue
      return
      end

      subroutine cgvfic( k,rv,xv,uv,nv,nd,cgdir,id,rd )
c==================================================================
c CGVF initialization routine
c   assign values to test integration routine CGIF
c==================================================================
      integer k,nv,nd,cgdir,id(*)
      real rv(nd),xv(nd),uv(nd),rd(*)
      parameter( c000=1.,c001=1.,c002=.5,c003=.25,c004=.125,
     &                   c010=1.,c020=.5,c030=.25,c040=.125,
     &                   c100=1.,c200=.5,c300=.25,c400=.125 )
*       parameter( c000=1.,c001=1.,c002=.5,c003=.00,c004=.000,
*      &                   c010=1.,c020=.5,c030=.00,c040=.000,
*      &                   c100=1.,c200=.5,c300=.00,c400=.000 )
c1    f(x,y,z)=x+y+z
c2    f(x,y,z)=cos(pi*x)*cos(pi*y)*cos(pi*z)
      f(x,y,z)=c000+x*(c001+x*(c002+x*(c003+c004*x)))
     &             +y*(c010+y*(c020+y*(c030+c040*y)))
     &             +z*(c100+z*(c200+z*(c300+c400*z)))

      pi=atan(1.)*4.
      if( nd.eq.3 )then
        z0=xv(3)
      else
        z0=0.
      end if
      do n=1,nv
        uv(n)=f(xv(1),xv(2),z0)
      end do

      end

      subroutine cgest1( id,rd,root,l,cgdir,uvn,ng,nv,ierr )
c=================================================================
c   Save the solution on a file
c
c Input
c  l : multigrid level
c  cgdir,uvn : grid directory and name of the grid function
c
c=================================================================
      implicit integer (a-z)
      integer id(*)
      real rd(*)
      character*(*) uvn
c.......local
      character tname*80,tuvn*20
      external zerof

      if( nv.ne.1 )then
        write(*,'('' CGEST1:ERROR nv <>1 and trying to save '//
     &  ' the left null vector'')')
        return
      end if

c        mount a file in a temporary directory
      write(*,'('' CGETST1>>Enter name of the Composite Grid File'')')
      write(*,'(''        >>on which to save the left null vector'')')
      read(*,'(a)') tname

      call dskdef( id,root,'temp','D',0,tmgdir,ierr)
      write(*,*) 'CGEST1: Mounting a file ',tname(:nblank(tname))
      iotemp=22

      call dskmnt( id,tmgdir,tname(:nblank(tname)),iotemp,' W L4096',
     & ierr)

      if( ierr.ne.0 )then
        write(*,*) 'CGEST1:ERROR return from DSKMNT, ierr=',ierr
        goto 900
      end if
      tcgdir=dskfnd(id,tmgdir,'composite grid')+l-1

c     ...copy solution onto the file
      write(*,*) 'CGEST1: Copy left null vector in the file...'

      tuvn='left null vector'
      loc=dskloc(id,tcgdir,tuvn)
      if( loc.eq.0 )then
c*        write(*,'('' CGEST1: call cgvf...'')')
        call cgvf( id,rd,tcgdir,tuvn,zerof,nv,loc )       ! create
      else
        write(*,'('' CGEST1: Null vector exist, overwriting..'')')
c       ...set pointers to left null vector
        call cgaptr(id,tcgdir,tuvn,nv,' ',ierr )
      end if
      locnv=dskloc(id,cgdir,uvn)
      if( locnv.eq.0 )then
        write(*,'('' CGEST1:ERROR unable to find solution'',a)') uvn
      end if
c*      write(*,'('' CGEST1: call cgcopy...'')')
      call cgcopy( nv,id,rd,cgdir,uvn, id,rd,tcgdir,tuvn,ierr )
c     ...now release the grid function to the file
      if( loc.eq.0 )then
        call cgvrl( id,rd,tcgdir,tuvn,nv )
      end if

c.......dismount composite grid file
      write(*,*) 'CGEST1: dismount composite grid file...'
      call dskumt( id,tmgdir,ierr )
      if( ierr.ne.0 )then
        write(*,*) 'CGEST1:Error return from dskumt'
        goto 900
      end if
 900  continue

      return
      end

