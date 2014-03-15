      program tdf

      real x(5,5,5)
      integer io
      io=10
      open(unit=io,file='td.out',form='formatted',status='old',err=100)

      read(io,*) nx,ny,nz
      write(*,*) 'nx=',nx,', ny=',ny,', nz=',nz
      read(io,*) (((x(i,j,k),i=1,nx),j=1,ny),k=1,nz)
      write(*,*) (((x(i,j,k),i=1,nx),j=1,ny),k=1,nz)
      stop
 100  continue
      write(*,*) 'Error reading the file'
      stop
      end
