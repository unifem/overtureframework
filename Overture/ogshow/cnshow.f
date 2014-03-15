

      subroutine cnshow( id,rd,nd,ng,pu,t,dt )
c===================================================================
c         CNSHOW: Save results in the SHOW file
c         ------  -----------------------------
c
c PURPOSE-
c   This routine is called by CGCNS at specified intervals to
c   save solutions in the show file
c
c   The number of components saved in the show file is nvshow
c   The components to be saved are defined by ivshow(i), i=1...,nvshow
c
c   The application dependent information that is saved in the show file
c   is defined in files:
c       ush1.f and udef1.h : for idmeth=1
c       ush2.f and udef2.h : for idmeth=2
c       ush3.f and udef3.h : for idmeth=3
c          ... etc ...
c
c  ush1.f, ush2.f etc. define different combinations of the
c  computational variables that may be saved in the show file. For
c  example, for the Navier-Stokes equations we can save the pressure,
c  vorticity, Mach number etc.
c
c  udef1.f, udef2.f etc. define the names of the different combinations
c  and define the comments that go into the show file that are used
c  by cgshow to create the titles for the plots.
c
c
c INPUT-
c   pu(k) k=1,...,ng : pointers to solution at time t
c   t,dt  : current time and time step
c
c===================================================================
      integer pu(ng),id(*)
      real rd(*),t,dt

      include 'cgcns2.h'
      include 'cgcns.h'

c...local
      parameter( nvpd=25,nlined=10 )
      character line(nlined)*80,uvns(nvpd)*10,str*80,uvnp(nvpd)*10
      integer quvns,dim(5),dskfnd,dskloc
      logical first,d
      external uzero
      d(i)=mod(idebug/2**i,2).eq.1
      data first/.true./

      if( d(5) )then
c       ---debug option idebug=2**5
c       ---to determine artificial viscosity parameters plot the
c          solution to the shock tube problem
        k=1
        call cnsplt( ndrs(1,1,k),ndrs(1,2,k),ndrs(2,1,k),ndrs(2,2,k),
     &  rd(pu(k)),rd(id(qxy+k-1)),t )
      end if

      if( cgshow.eq.0 )then
        return
      end if
      iout=10

      if( first )then
c       ===First time through save some info on the show file===
c       ..save nv (nvshow)
        call dskdef( id,cgshow,'nv','I',0,loc,ierr )
        id(loc)=nvshow
        call dskrel( id,cgshow,'nv',' W',ierr )
c       ---Save Header comments in show file
        dim(1)=2
        dim(2)=80
        dim(3)=nfstr
        call dskdfs( id,cgshow,'header','S',dim,loc,ierr )
        do i=1,nfstr
          call dskps ( id,cgshow,'header',i,fstr(i),ierr)
        end do
        call dskrel( id,cgshow,'header',' W',ierr )

c       ---name solution component names for show file
c          First get all possible names as defined by the user,
c          including all possible combinations such as pressure,
c          vorticity, mach number etc
        call cnsudef( 5,nd,t,dt, nvpd,nc,uvnp,ierr )

        uvns(1)='u.cgcns'  ! name of the vector grid function
        do n=1,nvshow
          if( ivshow(n).ge.1 .and. ivshow(n).le.nc )then
            uvns(n+1)=uvnp(ivshow(n))
          else
            write(*,'('' CNSHOW: error, invalid ivshow='',i8)')
     &          ivshow(n)
          end if
        end do

c       ---save solution component names in show file
        dim(1)=2
c           choose string length from uvns(1)
        dim(2)=len(uvns(1))
        dim(3)=nvshow+1
        call dskdfs( id,cgshow,'uvn.show','S',dim,loc,ierr )
        do i=1,nvshow+1
          call dskps ( id,cgshow,'uvn.show',i,uvns(i),ierr)
        end do
        call dskrel( id,cgshow,'uvn.show',' W',ierr )
      end if

c     ---nshow : counter to number solutions
      nshow=nshow+1
* c     ---name the show solution: u000001, u000002, etc
*       write(uvns(1),'(''u'',i6.6)' ) nshow
* c     ---name the components: u000001.1, u000001.2, ...
*       do n=1,nvshow
*         write(uvns(n+1),'(''u'',i6.6,''.'',i1)' ) nshow,n
*       end do
      call cgshgn( 1,nshow,nvshow,uvns,ierr )

c     check to see if a show file solution with this name already exists
      quvns=dskloc(id,cgshow,uvns)
      if( quvns.eq.0 )then
c       ---create a composite grid vector function on the show file
        call cgvf( id,id,cgshow,uvns,uzero,nvshow,quvns )
      else
c       --set pointers to uvns--
        call cgaptr(id,cgshow,uvns,nvshow+1,' ',ierr )
        if( ierr.ne.0 )then
          write(*,*) 'CNSPRT: ERROR return from CGAPTR, ierr=',ierr
          stop
        end if
      end if

c     ...assign show file solution from current solution
      call cnshow2( id,rd,nd,ng,pu,id(quvns),quvns )

c     ...now release the grid function to the file
      call cgvrl( id,rd,cgshow,uvns,nvshow )

c     ===save comments for this solution in a string variable
c        named header000001, or header000002, ...

c     ---Get User defined title strings
      call cnsudef( 4,nd,t,dt, nlined,nc,line,ierr )
      nl=nc

      dim(1)=2
      dim(2)=max(nblank(line(1)),nblank(line(2)))
      dim(3)=nl
      write(str,'(''header'',i6.6)' ) nshow
      loc=dskloc(id,cgshow,str)
      if( loc.eq.0 )then
        call dskdfs( id,cgshow,str,'S',dim,loc,ierr )
      end if
      do i=1,nl
        call dskps( id,cgshow,str,i,line(i),ierr) ! put string
      end do
      call dskrel( id,cgshow,str,' W',ierr )

c     ...save some parameters associated with this solution
c        (this info is read by CGGTGF when restarting...)
c        save the current time, and also save
c        the first 10 user parameters found in rpu(i)
c
      nrp0=11
      write(str,'(''nrp'',i6.6)' ) nshow
      loc=dskloc(id,cgshow,str)
      if( loc.eq.0 )then
        call dskdef(id,cgshow,str,'I',0,loc,ierr)
      end if
      id(loc)=nrp0
      call dskrel( id,cgshow,str,' W',ierr )
      write(str,'(''rp'',i6.6)' ) nshow
      loc=dskloc(id,cgshow,str)
      if( loc.eq.0 )then
        dim(1)=1
        dim(2)=nrp0
        call dskdef(id,cgshow,str,'R',dim,loc,ierr)
      end if

      rd(loc+0)=t
      do i=1,nrp0-1
        rd(loc+i)=rpu(i)  ! user parameters are saved
      end do
c     ...release rp
      call dskrel( id,cgshow,str,' W',ierr )

      if( first )then
c       ---save some other info
        call dskdef(id,cgshow,'idmeth','I',0,loc,ierr)
        id(loc)=idmeth
        call dskrel( id,cgshow,'idmeth',' W',ierr )
c       ---save left and right states---
        dim(1)=1
        dim(2)=nv*2
        call dskdef(id,cgshow,'ulr','R',dim,loc,ierr)
        i=loc-1
        do n=1,nv
          i=i+1
          rd(i)=ulr(n,1)
        end do
        do n=1,nv
          i=i+1
          rd(i)=ulr(n,2)
        end do
        call dskrel( id,cgshow,'ulr',' W',ierr )
      end if

c     ---flush the file buffers
      call dskffl( id )

      first=.false.

      end

