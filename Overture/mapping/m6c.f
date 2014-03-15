      program m6
c
c   Convert the m6 x-section data file



      real*8 xc(100), xy(100,20,2:3)


      open(unit=10,file='m6xSection.dat',status='old',form='formatted')
      read(10,*) nsb,nrb

      nra=1
      nsa=1
      read(10,*) (xc(j),(xy(i,j,2),i=nra,nrb),(xy(i,j,3),i=nra,nrb),
     &     j=nsa,nsb)
      close(10)


      open (unit=10,file='m6xSection.new',status='unknown',
     &   form='formatted')


      do j=nsa,nsb
         write(10,*) '* cross section number ',j
         do i=nra,nrb
            write(10,*) xy(i,j,2),xy(i,j,3),xc(j)
         end do
      end do

      stop
      end
