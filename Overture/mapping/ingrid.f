c=======================================================================
c
c Routines to read and write ingrid files
c
c Kyle Chand
c September 1999
c
c=======================================================================

c=======================================================================
c
      subroutine opingrid(filenm, MTYPE, IUNIT, RDIM, DDIM, 
     .                            NNODE, NELEM, NEMAX)
c
c     open an ingrid file
c
c=======================================================================
      implicit none

      integer MAXCHARLEN
      parameter(MAXCHARLEN=1024)

c     input : filenm -- filename

      character*(*) filenm

c     output : iunit  -- unit number of the file
c              iunit  -- character string suggesting the type of ingrid file
c              rdim   -- range dimension
c              ddim   -- domain dimension
c              nnode  -- number of nodes
c              nelem  -- number of elements
c              nemax  -- max number of nodes in an element

      integer iunit
      character*(*) mtype
      integer rdim
      integer ddim
      integer nnode
      integer nelem
      integer nemax
      
c     local variables

      character*(MAXCHARLEN) ftok, title, comment, name
      integer dummy, n_spc_faces

c     open the file
      open(iunit, FILE=filenm, form='FORMATTED',STATUS= 'UNKNOWN')
      
c     read title card
      read(iunit, "(a)") ftok
c      if (ftok(1:6).eq."INGRID" .or. ftok(1:7).eq."editmsh" .or. 
c     &     ftok(1:1).ne." " .or. ftok(1:8).eq."INGRID2D"
c     &     .or.ftok(1:16).eq."OVERTUREUMapping") then
c         title = ftok
      if (ftok.ne." ") then
         title = ftok
      else
         backspace iunit
      endif

c     read comment card
      read(iunit, "(a)") ftok
      if (ftok(1:1) .ne." ") then
         comment = ftok
      else
         backspace iunit
      endif

      name = title


      if (title(1:8) .eq. "INGRID2D".or.title(1:7).eq."editmsh") then
         ddim = 2
         rdim = 2
         mtype(1:7) = "editmsh"
         nemax = 4
         read(iunit, *) dummy, nnode, nelem, nemax
      elseif(title(1:16) .eq. "OVERTUREUMapping") then
         mtype(1:8) = "OVERTURE"
         read(iunit, *) dummy, nnode, nelem, nemax, ddim, rdim
      else
         if (title(1:4) .eq. "ICEM") then
            mtype(1:4) = "ICEM"
         else
            mtype(1:6) = "INGRID"
         endif
         rdim = 3 
         ddim = 3
         nemax = 8
         read(iunit, *) dummy, nnode, nelem, n_spc_faces

      endif
      
      end

c=======================================================================
c
      subroutine clingrid(iunit)
c
c     close an ingrid file (provided for call symetry
c
c=======================================================================
      implicit none

c     input : iunit -- the unit number of the file to close
      integer iunit

      close(iunit)

      end

c=======================================================================
c
      subroutine rdingrid(iunit, mtype, rdim, ddim, nnode, nelem, nemax, 
     &     xyz, elems, tags)
c *wdh* 010129     &     xyz, tags, elems)
c
c     read a mesh from an igrid file
c
c=======================================================================
      implicit none

      integer MAXCHARLEN
      parameter(MAXCHARLEN=1024)

c     input : iunit - file unit number
c             mtype - character array indicating the type of ingrid mesh file
c             rdim  - range dimension
c             ddim  - domain dimension
c             nnode - number of nodes
c             nelem - number of elements
c             nemax - max number of nodes in an element

      integer iunit
      integer rdim
      integer ddim
      integer nnode
      integer nelem
      integer nemax
      character*(*) mtype

c     output : xyz   - array of positions
c              elemes- array of elements
c              tags  - array of element tags

      real xyz(1:nnode, *)
      integer elems(1:nelem, *)
      integer tags(*)

c     local variables

      integer dummy, p, i, z
      integer name
        
      write(*,*)'Entering rdingrid, nnode, nelem, rdim, nemax:', 
     .nnode, nelem, rdim, nemax

c     note that this format allows the point "name" to be different than it's 
c     location in the xyz array.  So, point p in elem(n) might actually refer
c     to the point/node "name", not the actual array referece.  For most 
c     (read all) of the meshes we will deal with this will not be the case
c     so we will not worry about it yet ;)
      do p=1,nnode
         read(iunit,*) name, (xyz(p,i),i=1,rdim)
         ! write(*,*) 'p=',p,' xyz=',xyz(p,1)
      enddo

c     note that this format allows the element "name" to be different than
c     it's location in the array, meaning that elems(n) might be "named" m
      if (mtype(1:4) .eq. "ICEM") then
         do z=1,nelem
            read(iunit,'(i8,i8,8i8)') dummy, tags(z),
     &           (elems(z,p),p=1,nemax)
         enddo
      elseif (mtype(1:7).eq."editmsh") then
         do z=1,nelem
            read(iunit,'(i6,i5,4i8)') dummy, tags(z), 
     &           (elems(z,p),p=1,nemax)
         enddo
      else
         do z=1,nelem
c AP            read(iunit,'(i8,i5,8i8)') dummy, tags(z), 
c AP     &           (elems(z,p),p=1,nemax)
            read(iunit,*) dummy, tags(z), 
     &           (elems(z,p),p=1,nemax)

         ! write(*,*) 'z=',z,' elems=',elems(z,1)

         enddo
      endif

      end

c=======================================================================
c
      subroutine wringrid(filenm, iunit, rdim, ddim, nnode, nelem,nemax, 
     &     xyz, elems, tags)
c
c     write a mesh to an ascii igrid file
c
c     currently ignore the option of writting comments in the file
c
c=======================================================================

      implicit none

      integer MAXCHARLEN
      parameter(MAXCHARLEN=1024)

c     input : filenm- filename
c             iunit - file unit number
c             rdim  - range dimension
c             ddim  - domain dimension
c             nnode - number of nodes
c             nelem - number of elements
c             nemax - max number of nodes in an element
c             xyz   - array of points
c             elems - array of elements
c             tags  - array of element tags

      character*(*) filenm
      integer iunit
      integer rdim
      integer ddim
      integer nnode
      integer nelem
      integer nemax
      real xyz(1:nnode, *)
      integer elems(1:nelem, *)
      integer tags(*)

c     local variables

      integer dummy, p, i, z
      integer region

      open(iunit, FILE=filenm, form='FORMATTED',STATUS= 'UNKNOWN')

      write(*,*) filenm
c     regions are not implemented yet in the framework
      region = 0
      dummy = 1
      write(iunit, '(a16)') "OVERTUREUMapping"
c     never have figured out what dummy is used for, only ever seen it 1 or 0
      write(iunit, *) dummy, nnode, nelem, nemax, ddim, rdim

c     note that this format allows the point "name" to be different than it's 
c     location in the xyz array.  So, point p in elem(n) might actually refer
c     to the point/node "name", not the actual array referece.  For most 
c     (read all) of the meshes we will deal with this will not be the case
c     so we will not worry about it yet ;)
      do p=1,nnode
         write(iunit,*) p, (xyz(p,i),i=1,rdim)
      enddo

c     note that this format allows the element "name" to be different than
c     it's location in the array, meaning that elems(n) might be "named" m
c     but we are not using that yet...
      do z=1,nelem
         write(iunit,'(i8,i5,8i8)') z, tags(z), 
     &        (elems(z,p)+1,p=1,nemax)
      enddo

      close(iunit)

      end
