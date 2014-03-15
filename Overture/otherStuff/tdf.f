      program tdf

      implicit real*8 (a-h,o-z)

      real*8 u(-2:257,-2:257,0:0,0:0)
      character name*80
      integer n,io
c-------- read statements for formatted file
c      io=10
c      open(unit=io,file='spin32.dat',form='formatted',status='old',
c     & err=900)
c
c      do n=1,10
c        read(io,'(a)',end=100) name
c        read(io,*) time
c        read(io,*,end=100) nx,ny,nz
c        write(*,*) 'name=',name(1:10),'time=',time,', nx=',nx,'
c     &    , ny=',ny,', nz=',nz
c        read(io,*,end=100) ((u(i,j,n),i=1,nx),j=1,ny)
c      end do
c 100  continue
c      close(io)

c --------- statements for unformateed files
      io=10
      open(unit=io,file='spin32.dat',form='unformatted',status='old',
     & err=900)

      do n=1,100
c       -- component name (char*80)
        read(io,err=200) name
c       --- time
        read(io,err=200) time
c       -- read array dimensions (possibly 4d arrays)
        read(io,err=200) n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b
        write(*,*) 'name=',name(1:10),'time=',time,', n1a=',n1a,
     &    ' n1b=',n1b,' n2a=',n2a,', n2b=',n2b
     &   ,' n3a=',n3a,', n3b=',n3b,' n4a=',n4a,', n4b=',n4b
        read(io,err=200) ((((u(i1,i2,0,0),i1=n1a,n1b),i2=n2a,n2b),
     &            i3=n3a,n3b),i4=n4a,n4b)
c       *** write out a line of values to see if they are correct **** 
        i2=(n2a+n2b)/2
        write(*,*) (u(i1,i2,0,0),i1=n1a,n1b)
      end do
 200  continue



      stop
 900  continue
      write(*,*) 'Error reading the file'
      stop
      end
