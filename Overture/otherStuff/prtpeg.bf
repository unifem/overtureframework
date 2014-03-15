      subroutine prtpeg( io,il,ip,ig,dr,n,n0,n1)

c  output the fort.2 file in pegsus format

      integer il(n,*),ip(n,*),ig(n)
      real dr(n,*)

c      write(io)  IBPNTS(IG),IIPNTS(IG),IIEPTR(IG),IISPTR(IG)
c      write(io)  (JI(I),KI(I),LI(I),DJ(I),DK(I),DL(I),I=1,IIPNTS(IG))
c      write(io)  (JB(I),KB(I),LB(I),IBC(I),I=1,IBPNTS(IG))

      write(io)  n,n,n0,n1
      write(io)  (il(i,1),il(i,2),il(i,3),dr(i,1),dr(i,2),dr(i,3),i=1,n)
      write(io)  (ip(i,1),ip(i,2),ip(i,3),ig(i),i=1,n)
      return
      end

#beginMacro WPEG( prec )

      subroutine wpeg5 ## prec 
     .                (iunit,g,ng,mjmax,mkmax,mlmax,
     .                 ji,ki,li,dxint,dyint,dzint,
     .                 jb,kb,lb,ibc,
     .                 ibnpts, iinpts, 
     .                 iieptr,iisptr,iblank,p4)


      implicit none

      integer g,ng
      integer iunit,ibnpts,iinpts,iieptr,iisptr
      integer mjmax,mkmax,mlmax
      integer ji(*),ki(*),li(*),jb(*),kb(*),lb(*),ibc(*)
      integer iblank(*)

      integer p4

#If (prec eq d)
      double precision dxint(*),dyint(*),dzint(*)
#Else
      real*4 dxint(*),dyint(*),dzint(*)
#End

      integer i
      integer one
      parameter(one=1)

c     pegsus 5.1 XINTOUT format interpolation and iblank information
c     http://people.nas.nasa.gov/~rogers/pegasus/sec3.html#OutFileForm

      if ( g.eq.1 ) 
     . open(iunit,FILE='fort.2',FORM='unformatted',STATUS='unknown')

      if ( p4.ne.1 ) then
         write(iunit)
     .        IBNPTS,IINPTS,IIEPTR,IISPTR,
     .        MJMAX,MKMAX, MLMAX

         write(iunit)
     .        (JI(I),I=1,IINPTS),
     .        (KI(I), I=1,IINPTS),
     .        (LI(I), I=1,IINPTS),
     .        (DXINT(I), I=1,IINPTS),
     .        (DYINT(I), I=1,IINPTS),
     .        (DZINT(I),I=1,IINPTS)

C        write(*,*)
C      .(JI(I)+one,I=1,IINPTS),
C      .(KI(I)+one, I=1,IINPTS),
C      .(LI(I)+one, I=1,IINPTS),
C      .(DXINT(I), I=1,IINPTS),
C      .(DYINT(I), I=1,IINPTS),
C      .(DZINT(I),I=1,IINPTS)
 
         write(iunit)
     .        (JB(I), I=1,IBNPTS),
     .        (KB(I), I=1,IBNPTS),
     .        (LB(I), I=1,IBNPTS),
     .        (IBC(I),I=1,IBNPTS)
     
      else
         
         write(iunit)
     .        IBNPTS,IINPTS,IIEPTR,IISPTR

         write(iunit)
     .        (JI(I),KI(I),LI(I),DXINT(I),DYINT(I),DZINT(I),
     .         I=1,IINPTS)

         write(iunit)
     .        (JB(I), KB(I), LB(I), IBC(I),I=1,IBNPTS)

      endif

      write(iunit) (IBLANK(I),I=1,MJMAX*MKMAX*MLMAX)

      if ( g.eq.ng ) close(iunit)

      return 
      end
#endMacro

WPEG( d )

WPEG( f )
