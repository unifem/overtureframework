      subroutine ffopen(io,fileName,fileForm,fileStatus)

      character*(*) fileName,fileForm,fileStatus

c
c  Routine to open fortran files 
c      
      open (unit=io, file=fileName,form=fileForm,
     &        status=fileStatus,err=100)

      return
 100  continue
      write(*,*) 'FFOPEN: error opening file'
      return
      end

      subroutine ffclose(io)
      close(io)
      end

      subroutine ffprinti( io,x,n )
      integer x(n)
      write(io) x
      return
      end

      subroutine ffprintf( io,x,n )
      real x(n)
      write(io) x
      return
      end

      subroutine ffprintd( io,x,n )
      double precision x(n)
      write(io) x
      return
      end

      subroutine ffprintc( io,x )
      character*(*) x
      character*80 xx
      xx=x
      write(io) xx
      return
      end

      subroutine ffprintia( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b )
c        write a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to write

      integer x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     
      write(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b)

      return
      end

      subroutine ffprintfa( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b )
c        write a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to write

      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     

      write(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b)

      return
      end

      subroutine ffprintda( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b )
c        write a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to write

      double precision x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     
      write(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b)

      return
      end

      subroutine ffprintifa( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     &                      y,m1a,m1b,m2a,m2b,m3a,m3b,m4a,m4b,
     &                      md1a,md1b,md2a,md2b,md3a,md3b,md4a,md4b )
c        write a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to write

      integer x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     
      real y(md1a:md1b,md2a:md2b,md3a:md3b,md4a:md4b)     
      write(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b),
     &          ((((y(i1,i2,i3,i4),i1=m1a,m1b),i2=m2a,m2b),
     &                           i3=m3a,m3b),i4=m4a,m4b)

      return
      end

      subroutine ffprintida( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     &                      y,m1a,m1b,m2a,m2b,m3a,m3b,m4a,m4b,
     &                      md1a,md1b,md2a,md2b,md3a,md3b,md4a,md4b )
c        write a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to write

      integer x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     
      double precision y(md1a:md1b,md2a:md2b,md3a:md3b,md4a:md4b)     
      write(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b),
     &          ((((y(i1,i2,i3,i4),i1=m1a,m1b),i2=m2a,m2b),
     &                           i3=m3a,m3b),i4=m4a,m4b)

      return
      end

      subroutine ffreadi( io,x,n )
      integer x(n)
      read(io) x
      return
      end

      subroutine ffreadf( io,x,n )
      real x(n)
      read(io) x
      return
      end

      subroutine ffreadd( io,x,n )
      double precision x(n)
      read(io) x
      return
      end

      subroutine ffreadc( io,x )
      character*(*) x
      character*80 xx
      read(io) xx
      x=xx
      return
      end

      subroutine ffreadia( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b )
c        read a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to read

      integer x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     
      read(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b)

      return
      end

      subroutine ffreadfa( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b )
c        read a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to read

      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     
      read(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b)

      return
      end

      subroutine ffreadda( io,x,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,
     &                      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b )
c        read a 4-d array
c        nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b: actual dimensions of the array
c        n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b : elements to read

      double precision x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)     
      read(io) ((((x(i1,i2,i3,i4),i1=n1a,n1b),i2=n2a,n2b),
     &                           i3=n3a,n3b),i4=n4a,n4b)

      return
      end

