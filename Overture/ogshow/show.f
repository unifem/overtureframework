      subroutine show1( id,rd,cgshow,nd,ng,nvshow,nshow,pu )
c===========================================================================
c  Save a solution in the cgshow directory
c     ---nshow : counter to number solutions
c==========================================================================
      integer id(*),cgshow,nd,ng,nvshow,nshow,pu(*)
      real rd(*)
      character uvns(20)*10
c......local
      integer quvns,dskloc
      external uzero

      call dsklog(id,6,63)

*      write(*,*) 'show1: grdir',dskloc(id,cgshow,'grid')

c     ---name the show solution: u000001, u000002, etc
      write(uvns(1),'(''u'',i6.6)' ) nshow
c     ---name the components: u000001.1, u000001.2, ...
      if( nvshow.gt.20 )then
        stop 'show1:error more than 20 components, get Bill to fix this'
      end if
      do n=1,nvshow
        write(uvns(n+1),'(''u'',i6.6,''.'',i1)' ) nshow,n
      end do

c     check to see if a show file solution with this name already exists
      quvns=dskloc(id,cgshow,uvns)
      if( quvns.eq.0 )then
c       ---create a composite grid vector function on the show file
        call cgvfz( id,id,cgshow,uvns,uzero,nvshow,quvns )
      else
c       --set pointers to uvns--
        call cgaptr(id,cgshow,uvns,nvshow+1,' ',ierr )
        if( ierr.ne.0 )then
          write(*,*) 'SHOW1:ERROR return from CGAPTR, ierr=',ierr
          stop
        end if
      end if
      do k=1,ng
        pu(k)=id(quvns+k-1)
      end do

      end

      subroutine show2( id,rd,cgdir,nvshow,k,uC,pu )
c===================================================================
c  Copy the solution into the show file
c==================================================================
      integer id(*),cgdir,k,dskfnd,pu
      real rd(*),uC(*)
      ndrsab(kd,ks,k)=id(ndrs+kd-1+nd*(ks-1+2*(k-1)))
   
      ndrs=dskfnd(id,cgdir,"ndrsab")
      nd=id(dskfnd(id,cgdir,"nd"))
      if( nd.eq.2 )then
        ndta=0
        ndtb=0
      else
        ndta=ndrsab(3,1,k)
        ndtb=ndrsab(3,2,k)
      end if

      call show4( rd(pu),uC,nvshow,ndrsab(1,1,k),ndrsab(1,2,k),
     & ndrsab(2,1,k),ndrsab(2,2,k),ndta,ndtb )

      end

      subroutine show3( id,rd,cgshow,nvshow,nshow )
c===========================================================================
c     ...now release the grid function to the file
c==========================================================================
      integer id(*),cgshow,nvshow,nshow
      real rd(*)
      character uvns(20)*10

c     ---name the show solution: u000001, u000002, etc
      write(uvns(1),'(''u'',i6.6)' ) nshow
c     ---name the components: u000001.1, u000001.2, ...
      if( nvshow.gt.20 )then
        stop 'show2:error more than 20 components, get Bill to fix this'
      end if
      do n=1,nvshow
        write(uvns(n+1),'(''u'',i6.6,''.'',i1)' ) nshow,n
      end do
c     ...now release the grid function to the file
      call cgvrl( id,rd,cgshow,uvns,nvshow )

      end

      subroutine show4( u,uC,nvshow,ndra,ndrb,ndsa,ndsb,ndta,ndtb )
c==========================================================================
c==========================================================================
      real  u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*)
      real uC(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*)
      do n=1,nvshow
        do i3=ndta,ndtb
          do i2=ndsa,ndsb
            do i1=ndra,ndrb
              u(i1,i2,i3,n)=uC(i1,i2,i3,n)
            end do
          end do
        end do
      end do
      end

  
      subroutine uzero( k,rv,xv,uv,nv,nd,cgdir,id,rd )
c==================================================================
c    Define the Composite Grid Function for CGVF
c    Set the grid function to zero.
c==================================================================
      integer k,nv,nd,cgdir,id(*)
      real rv(nd),xv(nd),uv(nd),rd(*)

      do n=1,nv
        uv(n)=0.
      end do

      end


      subroutine cgvfz( id,rd,cgdir,uname,cguv,nv,puv )
c===================================================================
c===================================================================
cTeX end
      implicit integer (a-z)
      integer id(*),cgdir,nv
      real rd(*)
      character*(*) uname(*)
      external cguv
c.......local
      parameter( nvd=20 )
      integer dim(5), cgtype
      logical cellcn, cellvt
      parameter (nrel=8 )
      character rel(nrel)*10,cgvfn*40
      data rel/'grid','ng','ndrsab','nrsab','mrsab','period','nd',
     & 'cgtype'/
c***statement functions cgvf
      dirg(k)=grdir+k-1
      ndrsab(m,n,k)= id(ndptr+m-1+nd*(n-1+2*(k-1)))
      nrsab (m,n,k)= id(nrsptr+m-1+nd*(n-1+2*(k-1)))
      ndra(k)=ndrsab(1,1,k)
      ndsa(k)=ndrsab(2,1,k)
      ndr(k)=ndrsab(1,2,k)-ndrsab(1,1,k)+1
      nds(k)=ndrsab(2,2,k)-ndrsab(2,1,k)+1
      pu(i1,i2,i3,n)=uptr+i1-ndra(k)+ndrk*
     &           (i2-ndsa(k)+ndsk*(i3-ndta+ndtk*(n-1)))
      pmr(k) =mrsptr+nd*2*(k-1)
      pnr(k) =nrsptr+nd*2*(k-1)
      pper(k)=ppper+nd*(k-1)
c.........end statement functions

      grdir  =dskfnd(id,cgdir,'grid')
      ngp    =dskfnd(id,cgdir,'ng')
      if( ngp.eq.0 )then
        stop 'CGVF:ERROR ng not found on the dsk'
      else
        ng   =id(ngp)
      end if
      ndptr  =dskfnd(id,cgdir,'ndrsab')
      nrsptr =dskfnd(id,cgdir,'nrsab')
      mrsptr =dskfnd(id,cgdir,'mrsab')
      ppper  =dskfnd(id,cgdir,'period')
      ndp    =dskloc(id,cgdir,'nd')
      if( ndp.eq.0 )then
        nd=2
      else
        nd=id(ndp)
      end if
*      cgtype =id(dskfnd(id,cgdir,'cgtype'))
      cgtypep=dskfnd(id,cgdir,'cgtype')
      if( cgtypep.eq.0 )then
        stop 'CGVF:ERROR cgtype not found on the dsk'
      else
        cgtype=id(cgtypep)
      end if
      if( cgtype.ne.1 .and. cgtype.ne.2 )then
        write(*,*) 'CGVF:ERROR Invalid value for cgtype=',cgtype
        stop 'CGVF:ERROR Invalid value for cgtype'
      end if
      if( nv.gt.nvd )then
        stop 'CGVF:ERROR nv > nvd dimension error'
      end if
      cellcn = cgtype.eq.2
      cellvt = cgtype.eq.1
c..........Create Pointers for vector grid function uname(1).....
      dim(1)=1
      dim(2)=ng
      call dskdef(id,cgdir,uname(1),'P',dim,uppt,ierr )
      puv = uppt

c*wdh 920609
c       === allocate space in one block ===
c           ndrs0 = total number of grid points on all component grids
      ndrs0=0
      do k=1,ng
        if( nd.eq.2 )then
          ndta=0
          ndtb=0
        elseif( nd.eq.3 )then
          ndta=ndrsab(3,1,k)
          ndtb=ndrsab(3,2,k)
        end if
        ndrs0=ndrs0+(ndrsab(1,2,k)-ndrsab(1,1,k)+1)
     &             *(ndrsab(2,2,k)-ndrsab(2,1,k)+1)
     &             *(ndtb-ndta+1)
      end do
      dim(1)=1
      dim(2)=ndrs0*nv
      cgvfn='cgvf.'//uname(1)
      call dskdef(id,cgdir,cgvfn,'R',dim,uptr0,ierr )
      uptr=uptr0
c*wdh 920609

      do 700 k=1,ng
        if( nd.eq.2 )then
          ndta=1
          ndtb=1
        elseif( nd.eq.3 )then
          ndta=ndrsab(3,1,k)
          ndtb=ndrsab(3,2,k)
        else
         stop 'CGVF:ERROR: Invalid value for nd'
        end if

c...........Define 'u' and assign it's pointer...............
        ndrk=ndrsab(1,2,k)-ndrsab(1,1,k)+1
        ndsk=ndrsab(2,2,k)-ndrsab(2,1,k)+1
        ndtk=ndtb-ndta+1
        dim(1)=nd+1
        dim(2)=ndrk
        dim(3)=ndsk
        dim(4)=ndtk
        dim(nd+2)=nv
c*920609call dskdef(id,dirg(k),uname(1),'R',dim,uptr,ierr )
c*930429call dsklnk(id,dirg(k),uname(1),'R',dim,uptr,ierr )
        call dsklnk(id,dirg(k),uname(1),'R',0,uptr,ierr )
*
*       loc=dskloc(id,dirg(k),uname(1))
*       write(*,'('' CGVF: n,uptr,loc='',i2,2i8)') 1,uptr,loc
*
        id(uppt+k-1)=uptr
c*wdh        if( nv.gt.1 )then
        if( nv.ge.1 )then
c           ....link to each component
          dim(1)=nd
          dim(2)=ndrk
          dim(3)=ndsk
          dim(4)=ndtk
          do 100 n=1,nv
            uvptr=uptr+ndrk*ndsk*ndtk*(n-1)
c*930429    call dsklnk(id,dirg(k),uname(n+1),'R',dim,uvptr,ierr )
            call dsklnk(id,dirg(k),uname(n+1),'R',0,uvptr,ierr )
*
*       loc=dskloc(id,dirg(k),uname(n+1))
*       write(*,'('' CGVF: n,uvptr,loc='',i2,2i8)') n,uvptr,loc
*
 100      continue
        end if
        uptr=uptr+ndrk*ndsk*ndtk*nv
 700  continue

c............Pointers to the vector components...................
c*wdh       if( nv.gt.1 )then
      if( nv.ge.1 )then
        dim(1)=1
        dim(2)=ng
        do 800 n=1,nv
          call dskdef(id,cgdir,uname(n+1),'P',dim,
     &                uvppt,ierr )
          do 800 k=1,ng
            if( nd.eq.2 )then
              ndtk=1
            else
              ndtk=ndrsab(3,2,k)-ndrsab(3,1,k)+1
            end if
            id(uvppt+k-1)=id(uppt+k-1)+(n-1)*ndr(k)*nds(k)*ndtk
          continue
 800    continue
      end if
c.........release
      call dskmrl( id,cgdir,rel,nrel,' ',ierr )
      return
      end




      subroutine showgp( id,rd,mdir,cgshow,nd,ng,nvshow,nshow,pu,name,
     &                  ndhead,header )
c===========================================================================
c  Get pointers to a  solution in the cgshow directory
c     ---nshow : counter to number solutions
c
c Input
c  ndhead : max number of header commments allowed
c Output -
c  ndhead : actual number of header comments returned
c==========================================================================
      integer id(*),mdir,cgshow,nd,ng,nvshow,nshow,pu(*)
      real rd(*)
      character name(*)*(*)
      character uvns(20)*40, uvn(20)*40,str*120,string*120
      character header(*)*(*)
c......local
      integer quvns,dskloc,cgdir
      external uzero

      nvshow=dskloc(id,cgshow,'nv')
      if( nvshow.eq.0)then
        write(str,'(''frame'',i1)') nshow
        cgdir=dskloc(id,mdir,str)
        write(*,*) ' showgp: new format? cgdir=',cgdir
        cgshow=cgdir
        nvshow=dskloc(id,cgdir,'nv')
      else
        cgdir=cgshow
      end if
      if( nvshow.eq.0 )then
        write(*,*) 'ERROR: showgp: nv not found'
        nvshow=1
      else
        nvshow=id(nvshow)
      end if


c     ---name the show solution: u000001, u000002, etc
      write(uvns(1),'(''u'',i6.6)' ) nshow
c     ---name the components: u000001.1, u000001.2, ...
      if( nvshow.gt.20 )then
        stop 'showgp:error more than 20 components, get Bill to fix'
      end if
      do n=1,nvshow
        write(uvns(n+1),'(''u'',i6.6,''.'',i1)' ) nshow,n
      end do

c     check to see if a show file solution with this name already exists
      quvns=dskloc(id,cgdir,uvns)
      if( quvns.eq.0 )then
        write(*,*) 'showgp: fatal ERROR: solution not found'
        return
      end if

c       --set pointers to uvns--
      call cgaptr(id,cgdir,uvns,nvshow+1,' ',ierr )
      if( ierr.ne.0 )then
        write(*,*) 'SHOW1:ERROR return from CGAPTR, ierr=',ierr
        stop
      end if
      do k=1,ng
        pu(k)=id(quvns+k-1)
      end do

c.........read in solution component names
      loc=dskloc(id,cgdir,'uvn.show')
      if( loc.ne.0 )then
        nvc=id(loc+1)
        do i=1,nvc
          call dskgs ( id,cgdir,'uvn.show',i,uvn(i),ierr)
*          write(*,'(1x,a20)') uvn(i)
        end do
        if( nvc.eq.1 .and. nv.eq.1 )then
          uvn(2)=uvn(1)
        elseif( nvc.ne.nvshow+1 )then
          write(*,*) 'CGSHHD: Warning - nvc .ne. nvshow+1'
        end if
        call dskrel( id,cgdir,'uvn.show',' ' ,ierr )
      else
        write(*,*) 'CGSHHD: Warning - No solution names found'
        uvn(1)='u'
        do i=2,nv+1
          write(uvn(i),'(''u'',i1)') i-1
        end do
      end if
      do i=1,nvc
        num=nblank(uvn(i))
        name(i)(1:num)=uvn(i)(1:num)
      end do
     
c..........read header comments
      write(str,'(''header'',i6.6)') nshow
      loc=dskloc( id,cgdir,str)
      if( loc.eq.0 )then
        write(*,*) 'SHOWGP:Warning No header lines found'
        nhead=0
      else
        nhead=id(loc+1)
      end if

      if( nhead.gt.ndhead ) then
        write(*,*) 'SHOWGP: Dimension error : nhead'
        nhead=ndhead
      end if
      do i=1,nhead
        call dskgs ( id,cgdir,str,i,string,ierr)
        num=nblank(string)
        header(i)(1:num) = string(1:num)
*        write(*,'(1x,a80)') header(i)
      end do
      if( nhead.gt.0 )then
        call dskrel( id,cgdir,str,' ' ,ierr )
      end if
      ndhead=nhead

      end


      subroutine showCF( id,rd,cgdir,nvshow,k,uC,pu )
c===================================================================
c  Copy the solution FROM the show file
c==================================================================
      integer id(*),cgdir,k,dskfnd,pu
      real rd(*),uC(*)
      ndrsab(kd,ks,k)=id(ndrs+kd-1+nd*(ks-1+2*(k-1)))
   
      ndrs=dskfnd(id,cgdir,"ndrsab")
      nd=id(dskfnd(id,cgdir,"nd"))
      if( nd.eq.2 )then
        ndta=0
        ndtb=0
      else
        ndta=ndrsab(3,1,k)
        ndtb=ndrsab(3,2,k)
      end if

      call show4( uC,rd(pu),nvshow,ndrsab(1,1,k),ndrsab(1,2,k),
     & ndrsab(2,1,k),ndrsab(2,2,k),ndta,ndtb )

      end



      subroutine cgshgs( mdir,wdir,nsol,nv,id,rd,ierr )
c==================================================================
c CGSHOW: Get_Solution
c
c Purpose -
c   Return a list of solutions to be plotted by CGSHOW
c
c Input -
c  mdir : the CG file for show file is mounted at this directory
c  wdir : directory on the disk in which to save the list of solutions
c Output -
c  nsol : number of solutions in the list
c  wdir : Holds a list of plot objects:
c    object_1 = { dir000001, u000001, name000001, nv000001, header00001 }
c    object_2 = { dir000002, u000002, name000002, nv000002, header00002 }
c      ... etc ...
c    where (letting <n> denote 000001, 000002, etc)
c      dir<n>   : directory in which to find the grid function
c                 This is an integer (equal to cgdir normally)
c      u<n>     : name of the grid function as found in dir<n>
c                 This is a string array, first entry is the name of
c                 the vector grid function, followed by the names of
c                 the components (CGVF format). Pointers for the vector
c                 function and components must exist.
c      name<n>  : generic name of the grid function (and components)
c                 This is a string array (CGVF format). These names
c                 appear on the plots.
c      nv<n>    : number of components in the grid function
c                 This is an integer
c      header<n>: header labels to put on plot
c                 This is a string array.
c
c==================================================================
      integer wdir,id(*),ierr
      real rd(*)
c .....local
      integer dskloc

      ierr=0

      loc=dskloc(id,mdir,'frame1')
      if( loc.eq.0 )then
c       ---Solutions are stored in a sequence: (Bill's format)
        call cgshgs1( mdir,wdir,nsol,nv,id,rd,ierr )  ! old format
      else
c       --- new format based on frame1, frame2, ...
        call cgshgs2( mdir,wdir,nsol,nv,id,rd,ierr )  ! new format for moving grids
      end if

      end

      subroutine cgshgs1( mdir,wdir,nsol,nv,id,rd,ierr )
c==================================================================
c Get_Solutions_1
c
c   Retrieve the plot objects when the solutions are stored
c  as a sequence (i.e. Bill's cgshow format that is used by
c  CGINS, CGCNS, CGEST )
c
c==================================================================
      implicit integer (a-z)
      integer wdir,id(*),ierr
      real rd(*)
c.......local
c     Output message types:
      integer vrbose,info,wrning,error,fatal
      parameter(vrbose=1,info=2,wrning=3,error=4,fatal=5)
      parameter( nvd=20,ndhead=30 )
      character*80 header(ndhead+5),str,line
      character uvn(nvd+1)*40

      ierr=0
*       write(*,'('' CGSHGS1:mdir ='',i8)') mdir

c     ---get the composite grid directory
c     ...request the composite grid to use in directory mdir
      call cgrqcg( id,mdir,mgdir,cgdir,ierr )

*       write(*,'('' mdir,mgdir,cgdir ='',3i8)') mdir,mgdir,cgdir

c     ...get nv - number of vector components
      nv=dskloc(id,cgdir,'nv')
      if( nv.eq.0 )then
        write(*,*) 'CGSHGS1:WARNING nv not found'
        nv=1
      else
        nv=id(nv)
      end if
      if( nv.lt.1 .or. nv.gt.nvd )then
        if( nv.lt.1 )then
          write (line,'(''CGSHGS1:ERROR nv ='',i6)') nv
        else
          write (line,'(''CGSHGS1:ERROR nvd too small '')')
        end if
        write(*,*) line
        stop
      end if

c     ...Read Header comments and solution component names
      call cgshhd( ndhead,nhead,header,nv,uvn,cgdir,id,id,ierr )
      if( nhead.gt.0 )then
        write(*,*) 'CGSHGS1:Header lines in show file:'
        do i=1,nhead
          write(*,*) header(i)
        end do
      end if
      if( nv.gt.1 )then
c        write(*,*) 'Component names:'
        do i=2,nv+1
          write(*,*) uvn(i)
        end do
      end if

      nsol=0
      do isol=1,100000
c       ----look for solutions that have been saved
*         write(str,'(''u'',i6.6)') isol
        call cgshgn( 0,isol,nv,str,ierr )
        loc=dskloc( id,cgdir,str )
        if( loc.eq.0 )then
          goto 900
        end if
        nsol=isol
      end do
      write(*,*) 'CGSHGS1: Warning, there may be more solutions'
 900  continue

      end




      subroutine cgshgs2( mdir,wdir,nsol,nv,id,rd,ierr )
c==================================================================
c Get_Solutions_2
c
c   Retrieve the plot objects when the solutions are stored
c   in the new format
c
c==================================================================
      implicit integer (a-z)
      integer wdir,id(*),ierr
      real rd(*)
c.......local
c     Output message types:
      integer vrbose,info,wrning,error,fatal
      parameter(vrbose=1,info=2,wrning=3,error=4,fatal=5)
      parameter( nvd=20,ndhead=30 )
      character*80 header(ndhead+5),str,line
      character uvn(nvd+1)*40

      ierr=0
      write(*,'('' CGSHGS2:*** new format*** mdir ='',i8)') mdir

      nsol=0
      do isol=1,100000

        write(str,'(''frame'',i1)') isol
        mdir2=dskloc(id,mdir,str)
        if( mdir2.eq.0 )then
          goto 900
        end if

        cgdir=mdir2

c     ...get nv - number of vector components
        nv=dskloc(id,cgdir,'nv')
        if( nv.eq.0 )then
          write(*,*) 'CGSHGS1:WARNING nv not found'
          nv=1
        else
          nv=id(nv)
        end if
        if( nv.lt.1 .or. nv.gt.nvd )then
          if( nv.lt.1 )then
            write (line,'(''CGSHGS1:ERROR nv ='',i6)') nv
          else
            write (line,'(''CGSHGS1:ERROR nvd too small '')')
          end if
          write(*,*) line
          stop
        end if

c       ...Read Header comments and solution component names
        call cgshhd( ndhead,nhead,header,nv,uvn,cgdir,id,id,ierr )
        if( nhead.gt.0 )then
          write(*,*) 'CGSHGS1:Header lines in show file:'
          do i=1,nhead
            write(*,*) header(i)
          end do
        end if
        if( nv.gt.1 )then
c          write(*,*) 'Component names:'
          do i=2,nv+1
            write(*,*) uvn(i)
          end do
        end if

c         ----look for solutions that have been saved
*         write(str,'(''u'',i6.6)') isol
        isol0=isol
        call cgshgn( 0,isol0,nv,str,ierr )
        loc=dskloc( id,cgdir,str )
        if( loc.eq.0 )then
          write(*,*) 'cgshgs2: error solution not found!'
          goto 900
        end if

        nsol=isol
      end do
      write(*,*) 'CGSHGS1: Warning, there may be more solutions'

 900  continue

      end


      subroutine cgshhd( ndhead,nhead,header,nv,uvn,cgdir,id,rd,ierr )
c=====================================================================
c Read in header lines and solution component names
c=====================================================================
      integer ndhead,nhead,nv,cgdir,id(*)
      real rd(*)
      character header(ndhead)*(*),uvn(*)*(*)
c......local
      integer dskloc

      loc=dskloc(id,cgdir,'header')
      if( loc.ne.0 )then
        nhead=id(loc+1)
*         write(*,*) 'CGSHHD: Header lines from show file...nhead=',nhead
      else
        nhead=0
*       write(*,*) 'CGSHHD: Warning - No header comments found'
      end if
      if( nhead.gt.ndhead ) then
        write(*,*) 'CGSHHD: Dimension error : nhead > ndhead'
        nhead=ndhead
      end if
      do 100 i=1,nhead
        call dskgs ( id,cgdir,'header',i,header(i),ierr)
*         write(*,'(1x,a80)') header(i)
 100   continue
      if( nhead.gt.0 )then
        call dskrel( id,cgdir,'header',' ' ,ierr )
      end if

c.........read in solution component names
      loc=dskloc(id,cgdir,'uvn.show')
      if( loc.ne.0 )then
        nvc=id(loc+1)
        do 200 i=1,nvc
          call dskgs ( id,cgdir,'uvn.show',i,uvn(i),ierr)
*           write(*,'(1x,a10)') uvn(i)
 200    continue
        if( nvc.eq.1 .and. nv.eq.1 )then
          uvn(2)=uvn(1)
        elseif( nvc.ne.nv+1 )then
          write(*,*) 'CGSHHD: Warning - nvc .ne. nv+1'
        end if
        call dskrel( id,cgdir,'uvn.show',' ' ,ierr )
      else
        write(*,*) 'CGSHHD: Warning - No solution names found'
        uvn(1)='u'
        do 300 i=2,nv+1
          write(uvn(i),'(''u'',i1)') i-1
 300    continue
      end if
      return
      end


      subroutine cgshgn( iopt,nshow,nv,uvns,ierr )
c=================================================================
c  Assign the generic name of the show file solution
c
c     ---name the show solution: u000001, u000002, etc
c     ---name the components: u000001.1, u000001.2, ...
c
c Input
c  iopt  : iopt=0 : Assign solution name only
c              =1 : Assign solution and component names
c  nshow : the number identifying the solution to return
c  nv    : number of components
c
c Output
c  uvns : generic name for solution number nshow
c=================================================================
      character*(*) uvns(*)

      ierr=0
c     ---name the show solution: u000001, u000002, etc
      write(uvns(1),'(''u'',i6.6)' ) nshow
      if( iopt.eq.1 )then
c       ---name the components: u000001.1, u000001.2, ...
        do n=1,nv
          if( n.lt.10 )then
            write(uvns(n+1),'(''u'',i6.6,''.'',i1)' ) nshow,n
          elseif( n.lt.100 )then
            write(uvns(n+1),'(''u'',i6.6,''.'',i2.2)' ) nshow,n
          elseif( n.lt.1000 )then
            write(uvns(n+1),'(''u'',i6.6,''.'',i3.3)' ) nshow,n
          else
            write(*,'('' CGSHNS:Error too many components! nv='''//
     &       ',i6)') nv
            ierr=1
          end if
        end do
      end if

      end



      subroutine cgrqcg( id,dir,mgdir,cgdir,ierr )
c======================================================================
c CG utility routine
c   Request the name of a composite grid to be found in directory dir
c
c Input -
c  dir : directory containing one or more composite grids
c Output
c  mgdir : the "mulitgrid level" directory requested
c  cgdir : the composite grid directory requested
c======================================================================
      integer dir,mgdir,cgdir,id(*)
c.......local
c     Output message types:
      integer vrbose,info,wrning,error,fatal
      parameter(vrbose=1,info=2,wrning=3,error=4,fatal=5)
      integer dskloc
      character str*80

      ierr=0
      mgdir=dir
      cgdir=dskloc(id,mgdir,'composite grid')

      if( cgdir.eq.0 )then
        write(*,*) ' Composite grid not in main directory'
	write(*,*) '        Choose a directory from:'
	call dskout (id, mgdir, '.', ' Oc', 6, ierr)
        do itry=1,5
          write(*,*) ' Composite grid directory to use:'
          read(*,'(a)') str
          mgdir=dskloc(id,dir,str)
          if( mgdir.ne.0 )then
            cgdir=dskloc(id,mgdir,'composite grid')
            if( cgdir.eq.0 )then
              write(*,*) ' Error: no composite grid found'
            else
              goto 200
            end if
          else
            write(*,*) 'ERROR <'//str(1:nblank(str))//'> not found'
          end if
        end do
        ierr=1
 200    continue
      end if
      end


