c------------------------------------------------------------------
c  Fortran I/O used by readPlot3d
c------------------------------------------------------------------

      function nblank(line)
c
c       Return the index of the last non-blank character,if any;
c       otherwise return 1.
c
        character*(*) line
        do 10 i=len(line),1,-1
          nblank=i
          if(line(i:i).ne.' ')return
10       continue
        return
       end

      subroutine cgopen(unit,file,status,form,ierr)
c
c       Open a file.
c
        character*(*) file,status,form
        integer unit
        ierr=0

        if(status.eq.'scratch')then
          open(unit=unit,status=status,form=form,err=10)
         else
          if(file.eq.' ')goto10
          open(unit=unit,file=file,status=status,form=form,err=10)
         endif
        rewind(unit)
        return
10      continue
        ierr=-1
       end


c*c ----------------- file format for plot3d "qfile" from overflow/ns/control/saveq.f
c*
c*      DIMENSION Q(JDIG,KDIG,LDIG,NQ)
c*      DIMENSION JD(*),KD(*),LD(*)
c*C
c*C
c*C   Q file is in PLOT3D single or multiple grid format.
c*C
c*C   First time through open the file.  Write the header.
c*C
c*      IF (IGRID.EQ.1) THEN
c*         OPEN(UNIT=4,FILE=QFILE,STATUS='UNKNOWN',FORM='UNFORMATTED')
c*         IF (NGRID.GT.1) THEN
c*            WRITE(4) NGRID
c*            WRITE(4) (JD(IG),KD(IG),LD(IG),IG=1,NGRID),NQ,NQC
c*         ELSE
c*            WRITE(4) JD(1),KD(1),LD(1),NQ,NQC
c*         ENDIF
c*      ENDIF
c*C
c*C   Write the flow information and Q.
c*C
c*      TIME   = ISTEP
c*      RE     = REY*FSMACH
c*      IF      (NQC.GE.2) THEN
c*         WRITE(4) FSMACH,ALPHA,RE,TIME,GAMINF,
c*     &            (RGAS(I),I=1,NQC)
c*      ELSE IF (IGAM.EQ.2) THEN
c*         WRITE(4) FSMACH,ALPHA,RE,TIME,GAMINF,
c*     &            HTINF,HT1,HT2,RGAS(1),RGAS(2)
c*      ELSE
c*         WRITE(4) FSMACH,ALPHA,RE,TIME,GAMINF
c*      ENDIF
c*      WRITE(4) Q
c*C





      subroutine opplt3d( filename,iunit,fileFormat,ngd, ng,nx,ny,nz,
     &                   nq,nqc )
c===================================================================
c /Description:
c   Open a plot3d file, read dimensions
c /filename (input) : name of the file to read
c /iunit (input) : format unit number to use
c /fileFormat (output) : file format, 
c            0= single grid, formatted, 
c            1= single grid, unformatted
c            2= multiple grids, formatted, 
c            3= multiple grids, unformatted
c            4= single grid, 2D, formatted
c            5= single grid, 2D, unformatted
c
c            6= q file, single grid, formatted
c            7= q file, single grid, unformatted
c            8= q file, multiple grids, formatted
c            9= q file, multiple grids, unformatted
c /ngd (input) : maximum number of grids allowed
c /nx(ngd), ny(ngd), nz(ngd) : number of points on each grid
c /ng (output) : number of grids in the file
c /nq (output) : for a q file this is the number of components
c /nqc (output) : for a q file this is the number of species?
c===================================================================
      integer iunit,fileFormat
      integer nx(ngd),ny(ngd),nz(ngd),nq,nqc
      character*(*) filename
c.......local
      character filen*180, line*800, achar
      logical filex

      ierr=0
      ng=1
      nq=0
      nqc=0
c*      fileFormat=-1
      filen=filename
      do itry=1,5
        INQUIRE(FILE=filen, EXIST=filex)
        if ( filex ) goto 5
        write(*,*) 'Error: Unable to open file='//filen
        write(*,*) 'Enter another file name'
        read(*,'(a)') filen
      end do
 5    continue

      if( .false. )then
        ! we should be able to determine the type of file by looking at the characters in the first few lines
        ! this is not finished yet
        open (iunit,file=filen,status='unknown',form='formatted')
        read(iunit,'(a)') line
        write(*,*) 'first line of file =['//line//']'
        length=len(line)
        do i=1,length
          achar=line(i:i)
          write(*,'(''char '',i3,'' =['',a1,''] ichar='',i8,'//
     &      ''' hex='',z4)') 
     &               i,achar,ichar(achar),achar
        end do
        achar=line(1:1)
        if( achar.ne.' ' .and. 
     &      achar.ne.'1' .and. achar.ne.'2' .and. achar.ne.'3' .and.
     &      achar.ne.'4' .and. achar.ne.'5' .and. achar.ne.'6' .and.
     &      achar.ne.'7' .and. achar.ne.'8' .and. achar.ne.'9' .and.
     &      achar.ne.'0' )then
          write(*,*) '**** file must be unformatted ****'
        else
          write(*,*) '**** file must be formatted ****'
        end if
      end if

 10   close(iunit)
c     *** try q file, single grid formatted 
      open (iunit,FILE=filen,STATUS='unknown',form='formatted')

      read(iunit,'(a)') line
      read(line,*,end=20,err=20) nx(1),ny(1),nz(1),nq,nqc
      fileFormat=6
      goto 500

 20   close(iunit)
c     *** try q file, single grid unformatted 
      write(*,*) 'look for q file, single grid unformatted...'
      open (iunit,FILE=filen,STATUS='unknown',form='unformatted')
      read(iunit,end=30,err=30) nx(1),ny(1),nz(1),nq,nqc
      if( nx(1).lt.0 .or. ny(1).lt.0 .or. nz(1).lt.0. .or.
     &    nq.lt.0 .or. nq.gt.10000 .or. 
     &    nqc.lt.0 .or. nqc.gt.10000 ) goto 30
      fileFormat=7
      goto 500
c     *** try q file, multiple grids formatted 
 30   close(iunit)
      write(*,*) 'look for q file, multiple grids formatted...'
      open (iunit,file=filen,status='unknown',form='formatted')
      read(iunit,'(a)') line
      read(line,*,end=40,err=40) ng
      ! write(*,'(" ng=",i6," ngd=",i6)') ng,ngd
      ! *wdh* EDF if( ng.gt.ngd ) goto 500
      if( ng.gt.ngd ) goto 40
      nq=-1
c*wdh no:      read(iunit,*,err=40,end=40) (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
      ! *wdh* 110514 -- this next does not work if nx,ny,... do not fit on one line!
      ! read(iunit,'(a)') line
      ! write(*,*) "line=",line
      ! read(line,*,err=40,end=40)
      ! &         (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
      read(iunit,*,err=40,end=40) (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
      ! write(*,'(" nx,ny,nz=",3i6," nq,nqc=",2i4))') nx(1),ny(1),nz(1),
      !  & nq,nqc
      if( nx(1).le.0. .or. ny(1).le.0 .or. nz(1).le.0 .or. 
     &    nq.le.0 .or. nq.lt.0 .or. nq.gt.10 .or. 
     &    nqc.lt.0 .or. nqc.gt.10 ) goto 40
      fileFormat=8
      goto 500
c     *** try q file, multiple grids unformatted 
 40   close(iunit)
      write(*,*) 'look for  q file, multiple grids unformatted...'
      open (iunit,file=filen,status='unknown',form='unformatted')
      read(iunit,err=50,end=50) ng

      if( ng.lt.0 .or. ng.gt.100000 ) goto 50

      if( ng.gt.ngd ) goto 500
      read(iunit,err=50,end=50) 
     &   (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
      if( nx(1).lt.0 .or. ny(1).lt.0 .or. nz(1).lt.0. .or.
     &    nq.lt.0 .or. nq.gt.10000 .or. 
     &    nqc.lt.0 .or. nqc.gt.10000 ) goto 50
      fileFormat=9
      goto 500


 50   close(iunit)
c     *** try single grid formatted 
      open (iunit,file=filen,STATUS='unknown',form='formatted')
      write(*,*) 'look for single grid formatted...'
      ng=1
      read(iunit,'(a)') line
c      write(*,*) 'line ='//line
      read(line,*,end=60,err=60) nx(1),ny(1),nz(1)
c      write(*,*) 'nx,ny,nz = ',nx(1),ny(1),nz(1)
      fileFormat=0
      goto 500

c     *** try single grid unformatted 
 60   close(iunit)
      write(*,*) 'look for single grid unformatted...'
      open (iunit,file=filen,status='unknown',form='unformatted')
      read(iunit,end=70,err=70) nx(1),ny(1),nz(1)
      if( nx(1).gt.1e6 .and. ny(1).gt.1e6 .and. nz(1).gt.1e6 ) goto 70
      fileFormat=1
      goto 500

 70   close(iunit)
c     *** try single grid formatted 
      open (iunit,file=filen,STATUS='unknown',form='formatted')
      write(*,*) 'look for single grid 2D formatted...'
      read(iunit,'(a)') line
      read(line,*,end=80,err=80) nx(1),ny(1)
      if( nx(1).le.0 .or. nx(2).le.0 ) goto 80
      nz(1)=1
      fileFormat=4
      goto 500
c
c     *** try single grid 2D unformatted 
 80   close(iunit)
      write(*,*) 'look for single grid 2D unformatted...'
      open (iunit,file=filen,status='unknown',form='unformatted')
      read(iunit,end=90,err=90) nx(1),ny(1)
      if( nx(1).gt.1e6 .and. ny(1).gt.1e6  ) goto 90
      nz(1)=1
      fileFormat=5
      goto 500
c
c     *** multiple grids formatted 
 90   close(iunit)
      write(*,*) 'look for multiple grids formatted...'
      open (iunit,file=filen,status='unknown',form='formatted')
      read(iunit,*,end=100,err=100) ng
      read(iunit,*,err=100,end=100)
     &         (nx(n),ny(n),nz(n),n=1,min(ngd,ng))
      fileFormat=2
      goto 500
c     *** multiple grids unformatted 
 100   close(iunit)
      write(*,*) 'look for multiple grids unformatted...'
      open (iunit,file=filen,status='unknown',form='unformatted')
      read(iunit,err=110,end=110) ng
      write(*,*) '...ng=',ng
      read(iunit,err=110,end=110) 
     &   (nx(n),ny(n),nz(n),n=1,min(ngd,ng))
      do n=1,min(ngd,ng)
        write(*,'(''grid '',i4,'' nx,ny,nz='',3i8)') 
     &   n,nx(n),ny(n),nz(n)
      end do
      fileFormat=3
      goto 500

 110   continue
      fileFormat=-1

 500  continue
      if( ng.gt.ngd )then
        write(*,*) 'opplt3d: ERROR: there were more than ',ngd,
     &    ' grids in this file'
        write(*,*) 'increase dimension ngd'
      end if

      return
      end

      subroutine rdplt3d(fileFormat,iunit, grid,nx,ny,nz,
     &  nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,xy,readIblank,iblank,ierr )
c====================================================================
c Read in grid points from a plot3d file
c
c /fileFormat (input):
c /iunit (input):
c /grid (input): grid to read (=1 if only one grid)
c /nd (input) : number of dimensions to read
c /xy : output
c /readIblank (input) : 1=read the iblank array
c /ierr: 0 on success, 1 on error
c====================================================================
      integer nx(*),ny(*),nz(*)
      integer fileFormat,iunit,grid,idum
      integer readIblank,iblank(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),dummy

      ierr=0
      nd0=nd

      if( fileFormat.eq.0 .or. fileFormat.eq.2 .or. 
     &    fileFormat.eq.4 )then
c        formatted file:
         if( readIblank.eq.0 )then
           do n=1,grid-1
              read(iunit,*,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0)
           end do
           read(iunit,*,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
         else
           do n=1,grid-1
              read(iunit,*,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0),
     &           (((idum,i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n))
           end do
           read(iunit,*,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
         end if
      else
c       unformatted file
         write(*,*) 'rdplt3d:read unformatted nx=',nx(grid),
     &       ' ny=',ny(grid),' nz=',nz(grid),' nd=',nd
         if( readIblank.eq.0 )then
           do n=1,grid-1
              read(iunit,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0)
           end do
           read(iunit,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
         else
           do n=1,grid-1
              read(iunit,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0),
     &           (((idum,i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n))
           end do
           read(iunit,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
         end if
      end if

      return
 900  continue
      ierr=1
      write(*,*) 'rdplt3d:ERROR reading file '

      return
      end

      subroutine rdplt3ds(fileFormat,iunit, grid,nx,ny,nz,
     &  nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,xy,readIblank,iblank,ierr )
c====================================================================
c Read in grid points from a plot3d file **SINGLE PRECISION VERSION**
c
c /fileFormat (input):
c /iunit (input):
c /grid (input): grid to read (=1 if only one grid)
c /nd (input) : number of dimensions to read
c /xy : output
c /ierr: 0 on success, 1 on error
c====================================================================
      integer nx(*),ny(*),nz(*)
      integer fileFormat,iunit,grid
      integer readIblank,iblank(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real*4 xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),dummy

      ierr=0
      nd0=nd
      if( fileFormat.eq.0 .or. fileFormat.eq.2 .or. 
     &    fileFormat.eq.4 )then
c        formatted file:
         if( readIblank.eq.0 )then
           do n=1,grid-1
              read(iunit,*,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0)
           end do
           read(iunit,*,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
         else
           do n=1,grid-1
              read(iunit,*,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0),
     &           (((idum,i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n))
           end do
           read(iunit,*,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
         end if
      else
c       unformatted file
         if( readIblank.eq.0 )then
           do n=1,grid-1
              read(iunit,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0)
           end do
           read(iunit,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
         else
           do n=1,grid-1
              read(iunit,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0),
     &           (((idum,i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n))
           end do
           read(iunit,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
         end if
      end if

      return
 900  continue
      ierr=1
      write(*,*) 'rdplt3ds:ERROR reading file '

      return
      end

      subroutine rdplt3dd(fileFormat,iunit, grid,nx,ny,nz,
     &  nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,xy,readIblank,iblank,ierr )
c====================================================================
c Read in grid points from a plot3d file **DOUBLE PRECISION VERSION**
c
c /fileFormat (input):
c /iunit (input):
c /grid (input): grid to read (=1 if only one grid)
c /nd (input) : number of dimensions to read
c /xy : output
c /ierr: 0 on success, 1 on error
c====================================================================
      integer nx(*),ny(*),nz(*)
      integer fileFormat,iunit,grid
      integer readIblank,iblank(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real*8 xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),dummy

      ierr=0
      nd0=nd
      if( fileFormat.eq.0 .or. fileFormat.eq.2 .or. 
     &    fileFormat.eq.4 )then
c        formatted file:
         if( readIblank.eq.0 )then
           do n=1,grid-1
              read(iunit,*,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0)
           end do
           read(iunit,*,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
         else
           do n=1,grid-1
              read(iunit,*,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0),
     &           (((idum,i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n))
           end do
           read(iunit,*,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
         end if
      else
c       unformatted file
         if( readIblank.eq.0 )then
           do n=1,grid-1
              read(iunit,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0)
           end do
           read(iunit,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
         else
           do n=1,grid-1
              read(iunit,end=900,err=900) ((((dummy,
     &           i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n)),kd=1,nd0),
     &           (((idum,i1=1,nx(n)),i2=1,ny(n)),i3=1,nz(n))
           end do
           read(iunit,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
         end if
      end if

      return
 900  continue
      ierr=1
      write(*,*) 'rdplt3ds:ERROR reading file '

      return
      end


      subroutine closeplt3d(iunit)
      close(iunit)
      end 


      subroutine rdplt3dqs(fileFormat,iunit, grid,nx,ny,nz,
     &  nq,ndra,ndrb,ndsa,ndsb,ndta,ndtb,q, 
     & nqc,fsmach,alpha,re,time,gaminf,rgas, ierr )
c====================================================================
c Read in q values from a plot3d file
c  **single precision**
c
c /fileFormat (input):
c /iunit (input):
c /grid (input): grid to read (=1 if only one grid)
c /nd (input) : number of dimensions to read
c /q : output
c /fsmach,alpha,re,time,gaminf,rgas (output):
c /ierr: 0 on success, 1 on error
c====================================================================
      integer nx(*),ny(*),nz(*)
      integer fileFormat,iunit,grid
      real*4 q(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nq)
      real*4 fsmach,alpha,re,time,gaminf,rgas(*)

      ierr=0
      if( fileFormat.eq.6 .or. fileFormat.eq.8 ) then
c        formatted file:
         if( nqc.ge.2 ) then
            read(iunit,*,end=900,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            read(iunit,*,end=900,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            read(iunit,*,end=900,err=900) fsmach,alpha,re,time,gaminf
         endif
         read(iunit,*,end=900,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)
      else
c       unformatted file
         if( nqc.ge.2 ) then
            read(iunit,end=900,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            read(iunit,end=900,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            read(iunit,end=900,err=900) fsmach,alpha,re,time,gaminf
         endif
         write(*,*) "fsmach=",fsmach,"alpha=",alpha,"re=",re
         write(*,*) 'rdplt3dq:read unformatted nx=',nx(grid),
     &       ' ny=',ny(grid),' nz=',nz(grid),' nq=',nq
         
         read(iunit,end=900,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)
      end if

      return
 900  continue
      ierr=1
      write(*,*) 'rdplt3dq:ERROR reading file '

      return
      end

      subroutine rdplt3dqd(fileFormat,iunit, grid,nx,ny,nz,
     &  nq,ndra,ndrb,ndsa,ndsb,ndta,ndtb,q, 
     & nqc,fsmach,alpha,re,time,gaminf,rgas, ierr )
c====================================================================
c Read in q values from a plot3d file
c  **double precision**
c
c /fileFormat (input):
c /iunit (input):
c /grid (input): grid to read (=1 if only one grid)
c /nd (input) : number of dimensions to read
c /q : output
c /fsmach,alpha,re,time,gaminf,rgas (output):
c /ierr: 0 on success, 1 on error
c====================================================================
      integer nx(*),ny(*),nz(*)
      integer fileFormat,iunit,grid
      real*8 q(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nq)
      real*8 fsmach,alpha,re,time,gaminf,rgas(*)

      ierr=0
      if( fileFormat.eq.6 .or. fileFormat.eq.8 ) then
c        formatted file:
         if( nqc.ge.2 ) then
            read(iunit,*,end=900,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            read(iunit,*,end=900,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            read(iunit,*,end=900,err=900) fsmach,alpha,re,time,gaminf
         endif
         write(*,*) "fsmach=",fsmach," alpha=",alpha," re=",re,
     & " time=",time," gaminf=",gaminf
         read(iunit,*,end=900,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)
      else
c       unformatted file
         if( nqc.ge.2 ) then
            read(iunit,end=900,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            read(iunit,end=900,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            read(iunit,end=900,err=900) fsmach,alpha,re,time,gaminf
         endif
         write(*,*) "fsmach=",fsmach,"alpha=",alpha,"re=",re
         write(*,*) 'rdplt3dq:read unformatted nx=',nx(grid),
     &       ' ny=',ny(grid),' nz=',nz(grid),' nq=',nq
         
         read(iunit,end=900,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)
      end if

      return
 900  continue
      ierr=1
      write(*,*) 'rdplt3dq:ERROR reading file '

      return
      end






      subroutine wrplt3ds(filename,fileFormat,iunit, ng,grid,nx,ny,nz,
     &  nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,xy,writeIblank,iblank,ierr )
c====================================================================
c   Write grid points to a plot3d file **SINGLE PRECISION VERSION**
c
c /fileFormat (input):
c /iunit (input):
c /grid (input): grid to read (=1 if only one grid)
c /nd (input) : number of dimensions to read
c /xy : output
c /ierr: 0 on success, 1 on error
c====================================================================
      implicit none
      integer iunit,ng,grid,nx(*),ny(*),nz(*),nd,ndra,ndrb,ndsa,ndsb,
     &  ndta,ndtb,ierr
      integer fileFormat
      integer writeIblank,iblank(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real*4 xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd)
      character*(*) filename
c.......local
      character filen*180
      integer n,i1,i2,i3,kd

      ierr=0
      filen=fileName
      if( fileFormat.eq.0 )then
c        formatted file:
        if( grid.eq.1 )then
          open (iunit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(iunit,*,err=900) ng
          end if
          write(iunit,*,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if
        if( writeIblank.eq.0 )then
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if
      else
c       unformatted file
        if( grid.eq.1 )then
          open (iunit,FILE=filen,STATUS='unknown',form='unformatted')
          if( ng.gt.1 )then
            write(iunit,err=900) ng
          end if
          write(iunit,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if
        if( writeIblank.eq.0 )then
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if
      end if

c     kkc 041014 - if this is a multiple grid file and this is the last grid, close the
c                  file for symmetry with the first call
      if ( (ng.gt.1) .and. (grid.eq.ng) ) close(iunit)

      return
 900  continue
      ierr=1
      write(*,*) 'wrdplt3ds:ERROR writing file '

      return
      end



      subroutine wrplt3dd(filename,fileFormat,iunit, ng,grid,nx,ny,nz,
     &  nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,xy,writeIblank,iblank,ierr )
c====================================================================
c   Write grid points to a plot3d file **DOUBLE PRECISION VERSION**
c
c /fileFormat (input):
c /iunit (input):
c /grid (input): grid to read (=1 if only one grid)
c /nd (input) : number of dimensions to read
c /xy : output
c /ierr: 0 on success, 1 on error
c====================================================================
      implicit none
      integer iunit,ng,grid,nx(*),ny(*),nz(*),nd,ndra,ndrb,ndsa,ndsb,
     &  ndta,ndtb,ierr
      integer fileFormat
      integer writeIblank,iblank(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real*8 xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd)
      integer n,i1,i2,i3,kd

      character*(*) filename
c.......local
      character filen*180

      ierr=0
      filen=filename
      if( fileFormat.eq.0 )then
c        formatted file:
        if( grid.eq.1 )then
          open (iunit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(iunit,*,err=900) ng
          end if
          write(iunit,*,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if
        if( writeIblank.eq.0 )then
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if
      else
c       unformatted file
        if( grid.eq.1 )then
          open (iunit,FILE=filen,STATUS='unknown',form='unformatted')
          if( ng.gt.1 )then
            write(iunit,err=900) ng
          end if
          write(iunit,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if
        if( writeIblank.eq.0 )then
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if
      end if

c     kkc 041014 - if this is a multiple grid file and this is the last grid, close the
c                  file for symmetry with the first call
      if ( (ng.gt.1) .and. (grid.eq.ng) ) close(iunit)

      return
 900  continue
      ierr=1
      write(*,*) 'wrdplt3ds:ERROR writing file '

      return
      end


**************************

      subroutine wrplt3dqs( gridFileName,qFileName, fileFormat,
     &  iunit,junit, ng,grid,nx,ny,nz,
     &  nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,xy, nq, nqc, q, 
     &  writeIblank,iblank, par, ierr )
c====================================================================
c   Write a solution to plot3d "q" file **SINGLE PRECISION VERSION**
c   (see Overture/mapping/plot3d.format)
c
c This routine should be called successively for grid=1,2,..,ng 
c
c /gridFileName (input) : name of the output grid file
c /qFileName (input) : name of the output q file
c /fileFormat (input): 0=formatted, 1=unformatted
c /iunit, junit (input): unit numbers to use for grid and q files.
c /ng (input): total number of grids to save
c /nx(1..ng),ny(1..ng),nz(1..ng) : grid points
c /grid (input): grid to write (call successively with grid=1,2,...,ng 
c /nd (input) : number of dimensions to read
c /xy (input) : grid points
c /nq (input) : number of components in u 
c /nqc (input) : number of species (default =1 )
c /q (input) : solution
c /writeIblank (input) : 0=do not save iblank, 1=save iblank
c /iblank (input) : iblank array
c /par (input) : flow parameters:
c      par(0)=machNumber;
c      par(1)=alpha;
c      par(2)=reynoldsNumber;
c      par(3)=t;
c      par(4)=gamma;
c      par(5)=Rg;
c /ierr: 0 on success, 1 on error
c====================================================================
      implicit none
      integer ng,grid,nx(*),ny(*),nz(*),nd,ndra,ndrb,ndsa,ndsb,
     &  ndta,ndtb,ierr,iunit,junit,nq,nqc
      integer fileFormat
      integer writeIblank,iblank(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd)
      real q(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nq)
      real par(0:*)
      integer n,i1,i2,i3,kd

      character*(*) gridFileName,qFileName
c.......local variables
      character filen*180
      integer i
      real fsmach,alpha,re,time,gaminf,rgas(100)


      fsmach=par(0)
      alpha=par(1)
      re=par(2)
      time=par(3)
      gaminf=par(4)
      if( nqc.gt.100 )then
        write(*,'("wrplt3dqs: error: nqc>100 -- fix me")')
        stop 1177
      end if
      do i=1,nqc 
        rgas(i)=par(4+i)
      end do

      ierr=0
      if( fileFormat.eq.0 )then

        ! -- formatted file: --

        ! -- write the grid file 
        if( grid.eq.1 )then
          filen=gridFileName
          open (iunit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(iunit,*,err=900) ng
          end if
          write(iunit,*,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if

        if( writeIblank.eq.0 )then
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if

        ! write the q file 
        if( grid.eq.1 )then
          filen=qFileName
          open (junit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(junit,*,err=900) ng
          end if
          write(junit,*,err=900) (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
        end if
         if( nqc.ge.2 ) then
            write(junit,*,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            write(junit,*,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            write(junit,*,err=900) fsmach,alpha,re,time,gaminf
         endif
         write(junit,*,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)


      else

        ! --  unformatted file --

        ! -- write the grid file 
        if( grid.eq.1 )then
          filen=gridFileName
          open (iunit,FILE=filen,STATUS='unknown',form='unformatted')
          if( ng.gt.1 )then
            write(iunit,err=900) ng
          end if
          write(iunit,err=900) (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
        end if

        if( writeIblank.eq.0 )then
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if

        ! write the q file 
        if( grid.eq.1 )then
          filen=qFileName
          open (junit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(junit,err=900) ng
          end if
          write(junit,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if
         if( nqc.ge.2 ) then
            write(junit,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            write(junit,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            write(junit,err=900) fsmach,alpha,re,time,gaminf
         endif
         write(junit,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)


      end if

      if( grid.eq.ng )then
        close(iunit)
        close(junit)
      end if

      return
 900  continue
      ierr=1
      write(*,*) 'wrplt3dqs:ERROR writing file '

      return
      end


      subroutine wrplt3dqd( gridFileName,qFileName, fileFormat,
     &  iunit,junit, ng,grid,nx,ny,nz,
     &  nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,xy, nq, nqc, q, 
     &  writeIblank,iblank, par, ierr )
c====================================================================
c   Write a solution to plot3d "q" file **DOUBLE PRECISION VERSION**
c   (see Overture/mapping/plot3d.format)
c
c This routine should be called successively for grid=1,2,..,ng 
c
c /gridFileName (input) : name of the output grid file
c /qFileName (input) : name of the output q file
c /fileFormat (input): 0=formatted, 1=unformatted
c /iunit, junit (input): unit numbers to use for grid and q files.
c /ng (input): total number of grids to save
c /nx(1..ng),ny(1..ng),nz(1..ng) : grid points
c /grid (input): grid to write (call successively with grid=1,2,...,ng 
c /nd (input) : number of dimensions to read
c /xy (input) : grid points
c /nq (input) : number of components in u 
c /nqc (input) : number of species (default =1 )
c /q (input) : solution
c /writeIblank (input) : 0=do not save iblank, 1=save iblank
c /iblank (input) : iblank array
c /par (input) : flow parameters:
c      par(0)=machNumber;
c      par(1)=alpha;
c      par(2)=reynoldsNumber;
c      par(3)=t;
c      par(4)=gamma;
c      par(5)=Rg;
c /ierr: 0 on success, 1 on error
c====================================================================
      implicit none
      integer ng,grid,nx(*),ny(*),nz(*),nd,ndra,ndrb,ndsa,ndsb,
     &  ndta,ndtb,ierr,iunit,junit,nq,nqc
      integer fileFormat
      integer writeIblank,iblank(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real*8 xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd)
      real*8 q(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nq)
      real*8 par(0:*)
      integer n,i1,i2,i3,kd

      character*(*) gridFileName,qFileName
c.......local variables
      character filen*180
      integer i
      real fsmach,alpha,re,time,gaminf,rgas(100)


      fsmach=par(0)
      alpha=par(1)
      re=par(2)
      time=par(3)
      gaminf=par(4)
      if( nqc.gt.100 )then
        write(*,'("wrplt3dqd: error: nqc>100 -- fix me")')
        stop 1177
      end if
      do i=1,nqc 
        rgas(i)=par(4+i)
      end do

      ierr=0
      if( fileFormat.eq.0 )then

        ! -- formatted file: --

        ! -- write the grid file 
        if( grid.eq.1 )then
          filen=gridFileName
          open (iunit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(iunit,*,err=900) ng
          end if
          write(iunit,*,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if

        if( writeIblank.eq.0 )then
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if

        ! write the q file 
        if( grid.eq.1 )then
          filen=qFileName
          open (junit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(junit,*,err=900) ng
          end if
          write(junit,*,err=900) (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
        end if
         if( nqc.ge.2 ) then
            write(junit,*,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            write(junit,*,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            write(junit,*,err=900) fsmach,alpha,re,time,gaminf
         endif
         write(junit,*,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)


      else

        ! --  unformatted file --

        ! -- write the grid file 
        if( grid.eq.1 )then
          filen=gridFileName
          open (iunit,FILE=filen,STATUS='unknown',form='unformatted')
          if( ng.gt.1 )then
            write(iunit,err=900) ng
          end if
          write(iunit,err=900) (nx(n),ny(n),nz(n),n=1,ng),nq,nqc
        end if

        if( writeIblank.eq.0 )then
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(iunit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd),
     &        (((iblank(i1,i2,i3),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid))
        end if

        ! write the q file 
        if( grid.eq.1 )then
          filen=qFileName
          open (junit,FILE=filen,STATUS='unknown',form='formatted')
          if( ng.gt.1 )then
            write(junit,err=900) ng
          end if
          write(junit,err=900) (nx(n),ny(n),nz(n),n=1,ng)
        end if
         if( nqc.ge.2 ) then
            write(junit,err=900) fsmach,alpha,re,time,gaminf,
     &           (rgas(i),i=1,nqc)
c         else if (igam.eq.2) then
c            write(junit,err=900) fsmach,alpha,re,time,gaminf,
c     &           htinf,ht1,ht2,rgas(1),rgas(2)
         else
            write(junit,err=900) fsmach,alpha,re,time,gaminf
         endif
         write(junit,err=900)  ((((q(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nq)


      end if

      if( grid.eq.ng )then
        close(iunit)
        close(junit)
      end if

      return
 900  continue
      ierr=1
      write(*,*) 'wrplt3dqd:ERROR writing file '

      return
      end







      subroutine convertp3d(
     &  inFile,fileFormat,iunit,
     &  outFile,outFileFormat,ounit,
     &  ngrid,nx,ny,nz, 
     &  ndr,nds,ndt,xy )
c====================================================================
c Read in grid points from a plot3d file
c
c /fileFormat (input):
c /iunit (input):
c /ngrid (input): 
c /xy : space to hold the largest grid
c====================================================================
      character inFile*(*), outFile*(*)
      integer fileFormat,outFileFormat
      integer nx(*),ny(*),nz(*)
      integer iunit,ounit,grid
      real xy(ndr,nds,ndt,3)

      if( outFileFormat.eq.0 .or. outfileFormat.eq.2)then
        open (ounit,file=outFile,status='unknown',form='formatted')
        if( outfileFormat.eq.2 )then
          write(ounit,*) ngrid
        end if
        write(ounit,*) (nx(grid),ny(grid),nz(grid),grid=1,ngrid)
      else 
        open (ounit,file=outFile,status='unknown',form='unformatted')
        if( outfileFormat.eq.3 )then
          write(ounit) ngrid
        end if
        write(ounit) (nx(grid),ny(grid),nz(grid),grid=1,ngrid)
      end if


      nd=3
      do grid=1,ngrid

        if( fileFormat.eq.0 .or. fileFormat.eq.2 )then
c         formatted file:
          read(iunit,*,end=800,err=800) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
c         unformatted file
          read(iunit,end=800,err=800) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        end if
        if( outFileFormat.eq.0 .or. outfileFormat.eq.2 )then
          write(ounit,*,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        else
          write(ounit,err=900) ((((xy(i1,i2,i3,kd),
     &        i1=1,nx(grid)),
     &        i2=1,ny(grid)),
     &        i3=1,nz(grid)),kd=1,nd)
        end if

      end do
      close(iunit)
      close(ounit)
      return
 800  continue
      write(*,*) 'convertp3d:ERROR reading file ',inFile
      close(iunit)
      close(ounit)
      return
 900  continue
      write(*,*) 'convertp3d:ERROR writing file ',outFile
      close(iunit)
      close(ounit)

      return
      end


      subroutine dpm1( filename,idata,nd,ndrsab,nrsab,bc,share,per,ndr,
     &   fileform,dataform,errmes,ierr )
c===================================================================
c  Read in the component grid descriptors:
c      nd     : number of space dimensions
c      ndrsab : bounds on the grid points, including fictitious points
c      nrsab  : grid bounds, not including fictitious points
c      bc     : boundary conditions
c      share  : shared sides info
c      per    : periodicity info
c
c===================================================================
      integer ndrsab(3,2),nrsab(3,2),bc(3,2),share(3,2),per(3),ndr(3,2),
     & fileform,dataform
      character*(*) filename,errmes
c.......local
      parameter( ngd=50 )
      integer nii(ngd),njj(ngd),nkk(ngd)
      character filen*180

      ierr=0
      filen=filename
      do itry=1,5
        if( fileform.eq.0 )then
          call cgopen(idata,filen,'old',
     &        'unformatted',ierr)
        else
          call cgopen(idata,filen,'old',
     &        'formatted',ierr)
        end if
        if( ierr.eq.0 ) goto 100
        write(*,*) 'Error: Unable to open file='//filen
        write(*,*) 'Enter another file name'
        read(*,'(a)') filen
      end do
 100  continue

      if( ierr.ne.0 )then
        errmes='DPM1::Unable to open file: '//
     &         filen(1:nblank(filen))
        ierr=1
      end if

c....................................................................
c.......Change these next lines to suit your data file...............
      if( dataform.lt.10 )then
c       ---cmpgrd file format
        if( fileform.eq.0 )then
c         ---unformatted
          read(idata,err=900,end=900) nd
          read(idata,err=900,end=900) ((ndrsab(kd,ks),kd=1,nd),ks=1,2)
          read(idata,err=900,end=900) (( nrsab(kd,ks),kd=1,nd),ks=1,2)
          read(idata,err=900,end=900) ((    bc(kd,ks),kd=1,nd),ks=1,2)
          read(idata,err=900,end=900) (( share(kd,ks),kd=1,nd),ks=1,2)
          read(idata,err=900,end=900) (    per(kd   ),kd=1,nd)
        else
c         ---formatted
          read(idata,*,err=900,end=900) nd
          read(idata,*,err=900,end=900) ((ndrsab(kd,ks),kd=1,nd),ks=1,2)
          read(idata,*,err=900,end=900) (( nrsab(kd,ks),kd=1,nd),ks=1,2)
          read(idata,*,err=900,end=900) ((    bc(kd,ks),kd=1,nd),ks=1,2)
          read(idata,*,err=900,end=900) (( share(kd,ks),kd=1,nd),ks=1,2)
          read(idata,*,err=900,end=900) (    per(kd   ),kd=1,nd)
        end if
      else
c       ---plot3d file format
        if( fileform.eq.0 )then
c         ---unformatted
          ng0=1
          if( dataform.eq.11 .or. dataform.eq.12 )then
c           ...multiple grids
            read(idata,err=900,end=900) ng0
            if( ng0.gt.ngd )then
              write(*,'('' CGGGRDD: error ng0>ngd, dimension error'')')
              write(*,'(''  ng0='',i6,'' >>Increase ngd in CGGRD1'')')
     &          ng0
              stop 'CGGGRDD:CGGRD1'
            end if
          end if
          read(idata,err=900,end=900) (nii(n),njj(n),nkk(n),n=1,ng0)
        else
          ng0=1
          if( dataform.eq.11 .or. dataform.eq.12 )then
c           ...multiple grids
            read(idata,*,err=900,end=900) ng0
            if( ng0.gt.ngd )then
              write(*,'('' CGGRDD: error ng0>ngd, dimension error'')')
              write(*,'(''  ng0='',i6,'' >>Increase ngd in CGGRD1'')')
     &          ng0
              stop 'CGGRDD:CGGRD1'
            end if
          end if
          read(idata,*,err=900,end=900) (nii(n),njj(n),nkk(n),n=1,ng0)
        end if
        k0=1
        if( ng0.gt.1 )then
          write(errmes,'('' Enter the grid to use (1..,'',i4,'')'')')
     &      ng0
          read(*,*) k0
        end if
        k0=max(1,min(k0,ng0))
        if( dataform.eq.10 .or. dataform.eq.11 )then
          if( nd.eq.2 )then
c           The user thinks this is a 2D grid, check to see if this is true
            if( nii(k0).eq.1 )then
              nrsab(1,1)=1
              nrsab(1,2)=njj(k0)
              nrsab(2,1)=1
              nrsab(2,2)=nkk(k0)
              nrsab(3,1)=0
              nrsab(3,2)=0
            else if( njj(k0).eq.1 )then
              nrsab(1,1)=1
              nrsab(1,2)=nii(k0)
              nrsab(2,1)=1
              nrsab(2,2)=nkk(k0)
              nrsab(3,1)=0
              nrsab(3,2)=0
            else if( nkk(k0).eq.1 )then
              nrsab(1,1)=1
              nrsab(1,2)=nii(k0)
              nrsab(2,1)=1
              nrsab(2,2)=njj(k0)
              nrsab(3,1)=0
              nrsab(3,2)=0
            else
c             **** this is a 3D grid
              nd=3
            end if
          end if
          if( nd.eq.3 )then
            nrsab(1,1)=1
            nrsab(1,2)=nii(k0)
            nrsab(2,1)=1
            nrsab(2,2)=njj(k0)
            nrsab(3,1)=1
            nrsab(3,2)=nkk(k0)
          end if
        else
c         ...2D grid disguised as a 3D grid
          nd=2
          nrsab(1,1)=1
          nrsab(1,2)=njj(k0)
          nrsab(2,1)=1
          nrsab(2,2)=nkk(k0)
          nrsab(3,1)=0
          nrsab(3,2)=0
        end if
        do ks=1,2
          do kd=1,3
            ndrsab(kd,ks)=nrsab(kd,ks)
            bc(kd,ks)=ks+2*(kd-1)
            share(kd,ks)=0
            per(kd)=0
          end do
        end do
c       ---skip over first k0-1 grids
        nd0=3
        do n=1,k0-1
          if( fileform.eq.0 )then
            read(idata,end=900,err=900) ((((dummy,
     &       i1=1,nii(n)),i2=1,njj(n)),i3=1,nkk(n)),kd=1,nd0)
          else
            read(idata,*,end=900,err=900) ((((dummy,
     &       i1=1,nii(n)),i2=1,njj(n)),i3=1,nkk(n)),kd=1,nd0)
          end if
        end do
      end if
c....................................................................
      write(*,9600) (kd,(ndrsab(kd,ks),ks=1,2),kd=1,nd)
      write(*,9500) (kd,(nrsab(kd,ks),ks=1,2),kd=1,nd)
      write(*,9200) (kd,(bc(kd,ks),ks=1,2),kd=1,nd)
      write(*,9300) (kd,(share(kd,ks),ks=1,2),kd=1,nd)
      write(*,9400) (per(kd),kd=1,nd)
 9500 format(1x,'CGGRDD: ',/,
     & (1x,' nrsab(',i1,',.) =',2i5))
 9600 format(1x,'CGGRDD: ',/,
     & (1x,' ndrsab(',i1,',.) =',2i5))
 9200 format(1x,'CGGRDD: ',/,
     & (1x,' bc(',i1,',.) =',2i5))
 9300 format((1x,' share(',i1,',.) =',2i5))
 9400 format((1x,' per =',3i5))
      if( nd.eq.2 )then
        do ks=1,2
          nrsab(3,ks)=0
          ndrsab(3,ks)=0
          ndr(3,ks)=0
          bc(3,ks)=0
          share(3,ks)=0
        end do
        per(3)=0
      elseif( nd.ne.3 )then
        ierr=2
        write(errmes,9000) nd
        return
      end if
 9000 format('Error in CGGRD1: invalid value of nd=',i6)
c
c     Add an extra line of ghost points UNLESS there is only one point in a direction
c
      do kd=1,nd
        if( ndrsab(kd,2).gt.ndrsab(kd,1)) then
          ndr(kd,1)=min(ndrsab(kd,1),nrsab(kd,1)-1)
          ndr(kd,2)=max(ndrsab(kd,2),nrsab(kd,2)+1)
        else
          ndr(kd,1)=ndrsab(kd,1)
          ndr(kd,2)=ndrsab(kd,2)
        end if
      end do

      return
 900  continue
      ierr=1
      write(errmes,9100)
 9100 format('Error in CGGRD1: error reading file')
      return
      end

      subroutine dpm2( ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndr,ndrsab,
     &  nrsab,nd,xy,per,idata,errmes,fileform,dataform,ierr )
c====================================================================
c   Read in a grid, extrapolate to get fictitious points
c   which are used for derivatives
c
c Input:
c  ndrsab : bounds for grids points to be read in
c  ndr    : bounds for cggrdd routine, including extra points for
c           extrapolation
c Output:
c  ndrsab : set equal to ndr
c  xy     : grid points and extrapolated values
c
c====================================================================
      integer ndr(3,2),ndrsab(3,2),nrsab(3,2),nd,per(3),
     & fileform,dataform
      real xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),dummy
      character errmes*(*)
c........begin statement functions
      nper(kd)=nrsab(kd,2)-nrsab(kd,1)
c........end statement functions

      ierr=0
c....................................................................
c.......Change these next errmess to suit your data file...............
c.........read in the grid points
      if( fileform.eq.0 )then
c       ---unformatted
        if( dataform.eq.0 )then
c         ---data in the order x...y...z...
          do kd=1,nd
            do i3=ndrsab(3,1),ndrsab(3,2)
              do i2=ndrsab(2,1),ndrsab(2,2)
                read(idata,end=900,err=900)
     &            (xy(i1,i2,i3,kd),i1=ndrsab(1,1),ndrsab(1,2))
              end do
            end do
          end do
        elseif( dataform.eq.1 )then
          do i3=ndrsab(3,1),ndrsab(3,2)
            do i2=ndrsab(2,1),ndrsab(2,2)
              do i1=ndrsab(1,1),ndrsab(1,2)
                read(idata,end=900,err=900) (xy(i1,i2,i3,kd),kd=1,nd)
              end do
            end do
          end do
        else
c         ...plot3d form
          if( dataform.eq.10 .or. dataform.eq.11 )then
            read(idata,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &       i1=ndrsab(1,1),ndrsab(1,2)),
     &       i2=ndrsab(2,1),ndrsab(2,2)),
     &       i3=ndrsab(3,1),ndrsab(3,2)),kd=1,nd)
          else
c           ---2D grid stored as 3D with 2 points in i1 direction
c              save x and z, discard y
            i3=ndrsab(3,1)
            read(idata,*,end=900,err=900)
     &        (((xy(i1,i2,i3,min(2,kd)),dummy,
     &       i1=ndrsab(1,1),ndrsab(1,2)),
     &       i2=ndrsab(2,1),ndrsab(2,2)),kd=1,3)
          end if
        end if
      else
c       ---formatted
        if( dataform.eq.0 )then
c         ---data in the order x...y...z...
          read(idata,*,end=900,err=900)
     &     ((((xy(i1,i2,i3,kd),i1=ndrsab(1,1),ndrsab(1,2)),
     &                         i2=ndrsab(2,1),ndrsab(2,2)),
     &                         i3=ndrsab(3,1),ndrsab(3,2)),kd=1,nd)
        elseif( dataform.eq.1 )then
          read(idata,*,end=900,err=900)
     &     ((((xy(i1,i2,i3,kd),kd=1,nd),
     &                         i1=ndrsab(1,1),ndrsab(1,2)),
     &                         i2=ndrsab(2,1),ndrsab(2,2)),
     &                         i3=ndrsab(3,1),ndrsab(3,2))
        else
c         ...plot3d form
          if( dataform.eq.10 .or. dataform.eq.11 )then
            read(idata,*,end=900,err=900) ((((xy(i1,i2,i3,kd),
     &       i1=ndrsab(1,1),ndrsab(1,2)),
     &       i2=ndrsab(2,1),ndrsab(2,2)),
     &       i3=ndrsab(3,1),ndrsab(3,2)),kd=1,nd)
          else
c           ---2D grid stored as 3D with 2 points in i1 direction
c              save x and z, discard y
            i3=ndrsab(3,1)
            read(idata,*,end=900,err=900)
     &        (((xy(i1,i2,i3,min(2,kd)),dummy,
     &       i1=ndrsab(1,1),ndrsab(1,2)),
     &       i2=ndrsab(2,1),ndrsab(2,2)),kd=1,3)
          end if
        end if
      end if
c....................................................................

c     ...extrapolate extra points
      do kd=1,nd
        do i3=nrsab(3,1),nrsab(3,2)
          do i2=nrsab(2,1),nrsab(2,2)
            do i1=ndrsab(1,1)-1,ndr(1,1),-1
              if( per(1).ne.2 )then
                xy(i1,i2,i3,kd)=2.*xy(i1+1,i2,i3,kd)
     &                            -xy(i1+2,i2,i3,kd)
              else
                xy(i1,i2,i3,kd)=xy(i1+nper(1),i2,i3,kd)
              end if
            end do
            do i1=ndrsab(1,2)+1,ndr(1,2)
              if( per(1).ne.2 )then
                xy(i1,i2,i3,kd)=2.*xy(i1-1,i2,i3,kd)
     &                            -xy(i1-2,i2,i3,kd)
              else
                xy(i1,i2,i3,kd)=xy(i1-nper(1),i2,i3,kd)
              end if
            end do
          end do
        end do
        do i3=nrsab(3,1),nrsab(3,2)
          do i1=ndr(1,1),ndr(1,2)
            do i2=ndrsab(2,1)-1,ndr(2,1),-1
              if( per(2).ne.2 )then
                xy(i1,i2,i3,kd)=2.*xy(i1,i2+1,i3,kd)
     &                            -xy(i1,i2+2,i3,kd)
              else
                xy(i1,i2,i3,kd)=xy(i1,i2+nper(2),i3,kd)
              end if
            end do
            do i2=ndrsab(2,2)+1,ndr(2,2)
              if( per(2).ne.2 )then
                xy(i1,i2,i3,kd)=2.*xy(i1,i2-1,i3,kd)
     &                            -xy(i1,i2-2,i3,kd)
              else
                xy(i1,i2,i3,kd)=xy(i1,i2-nper(2),i3,kd)
              end if
            end do
          end do
        end do
        if( nd.eq.3 )then
          do i2=ndr(2,1),ndr(2,2)
            do i1=ndr(1,1),ndr(1,2)
              do i3=ndrsab(3,1)-1,ndr(3,1),-1
                if( per(3).ne.2 )then
                  xy(i1,i2,i3,kd)=2.*xy(i1,i2,i3+1,kd)
     &                              -xy(i1,i2,i3+2,kd)
                else
                  xy(i1,i2,i3,kd)=xy(i1,i2,i3+nper(3),kd)
                end if
              end do
              do i3=ndrsab(3,2)+1,ndr(3,2)
                if( per(3).ne.2 )then
                  xy(i1,i2,i3,kd)=2.*xy(i1,i2,i3-1,kd)
     &                              -xy(i1,i2,i3-2,kd)
                else
                  xy(i1,i2,i3,kd)=xy(i1,i2,i3-nper(3),kd)
                end if
              end do
            end do
          end do
        end if
      end do
      do kd=1,3
        do ks=1,2
          ndrsab(kd,ks)=ndr(kd,ks)
        end do
      end do

      close(idata)
      return
 900  continue
      write(errmes,9000)
 9000 format('Error in CGGRD2: error reading file')
      ierr=1
      return
      end

      subroutine dpm3( ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndr,ndrsab,
     &  nrsab,nd,xy,per,bc,share,isave,errmes,ierr )
c====================================================================
c  Output the file into an unformatted cmpgrd-type file
c
c Purpose-
c   Save the grid data in a file format that is more compressed
c   This new file will be easier to read-in the next time this
c   grid has to be used
c Input
c   ndrsab,nrsab,xy,per,bc,share : info for this grid
c Output -
c  ierr <> 0 if there was a write error
c  errmes : error message returned if an error occured
c
c====================================================================
      integer ndr(3,2),ndrsab(3,2),nrsab(3,2),nd,per(3),
     & bc(3,2),share(3,2)
      real xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd)
      character errmes*(*)

      ierr=0
      write(isave) nd
      write(isave) ((ndrsab(kd,ks),kd=1,nd),ks=1,2)
      write(isave) (( nrsab(kd,ks),kd=1,nd),ks=1,2)
      write(isave) ((    bc(kd,ks),kd=1,nd),ks=1,2)
      write(isave) (( share(kd,ks),kd=1,nd),ks=1,2)
      write(isave) (    per(kd   ),kd=1,nd)
      do kd=1,nd
        do i3=ndrsab(3,1),ndrsab(3,2)
          do i2=ndrsab(2,1),ndrsab(2,2)
            write(isave,err=900)
     &        (xy(i1,i2,i3,kd),i1=ndrsab(1,1),ndrsab(1,2))
          end do
        end do
      end do

      return
 900  continue
      write(errmes,'('' Error in CGGRD2: error reading file'')')
      ierr=1

      end

