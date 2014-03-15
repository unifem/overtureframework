#beginMacro beginLoops()
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro
#beginMacro endLoops()
end do
end do
end do
#endMacro

#beginMacro beginBoundaryLoops(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b)
do i3=nd3a,nd3b
do i2=nd2a,nd2b
do i1=nd1a,nd1b
#endMacro
#beginMacro endBoundaryLoops()
end do
end do
end do
#endMacro


      subroutine findInterpNeighbours( nd, ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & ndi,ndin, eir, dim, pbc, ni, nin, mask,m, ip,id,ia,vew, status, ipar, ierr )
!======================================================================
!  Optimised findInterpolationNeighbours
!   *new* version for parallel
!         
! nd : number of space dimensions
! ni : number of interpolation points
! eir : **this is really the extendedIndexRange***
!              In parallel it does NOT include parallel ghost
! pbc(side,axis)=1 if this face is an internal parallel boundary, 0 otherwise.
! ip : ip(i,0:nd-1) i=0,1,...,ni-1 -- list of interpolation points
! mask(:,:,:) : mask
! m(:,:,:) : work space the size of mask 
!
! ia(i,0:nd-1) : points to extrapolate
! id(i,0:nd-1) : direction to extrapolate
! vew(i) : variable extrapolation width (not yet used)
!
! status (output) : 
!   status(i)=0  : means we can evaluate the pt on this processor: 
!            =2  : this point cannot extrap on this processor
!            =-1 : this point is not needed
!
!======================================================================
      implicit none
      integer nd, 
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & ndi,ndin,nin,ierr

      integer ni,ip(0:ndi-1,0:*),ia(0:ndin-1,0:*),
     & id(0:ndin-1,0:*), vew(0:ndin-1), status(0:ndin-1), ipar(0:*), eir(0:1,0:2),
     & dim(0:1,0:2),pbc(0:1,0:2)

      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)
      integer m(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

!     --- local variables -----
      integer n1a,n1b,n2a,n2b,n3a,n3b,i1,i2,i3,i,n0,n1,n2,mm,j1,j2,j3,myid,ierr1
      integer m1,m2,m3,imask(-3:3,-3:3,-3:3)
      integer axis,maxExtrap,ndm(0:1,0:2)
      integer maxExtrapWidth,useVariableExtrapolationWidth,ghostBoundaryWidth,ghostMaskValue
      integer numberOfErrors,md1a,md1b,md2a,md2b,md3a,md3b
      logical found

      ! this function is normally only used with 2nd-order methods that
      ! use a fourth order stencil for dissipation or upwinding so maxExtrap=3
      ! should be ok -- this is a 4 point stencil and thus there should always
      ! be enough points for it
      !  mask: 0 -1 1 -1 0 
      !        1 -3 3 -1

      myid=ipar(3)
      maxExtrapWidth=ipar(0)
      if( maxExtrapWidth.le.0 .or. maxExtrapWidth.gt.1000 )then
        write(*,'("findInterNeighboursOpt:ERROR: maxExtrapWidth=",i6)') maxExtrapWidth
      end if
      maxExtrap=maxExtrapWidth-1  ! = 3  ! max order of extrap

      useVariableExtrapolationWidth=ipar(1)
      ghostBoundaryWidth=ipar(2)
      if( ghostBoundaryWidth.lt.0 .or. ghostBoundaryWidth.gt.100 )then
        write(*,'("findInterNeighOpt:ERROR: ghostBoundaryWidth=",i6)') ghostBoundaryWidth
        stop 1089
      end if

      ierr=0

      ! [n1a,n1b]x[n2a,n2b]x[n3a,n3b] defines the range of indicies where we
      ! we look for interpolation neighbours.
      ! Note: eir includes ghost lines on interp boundaries => we DO include neighbours on the 2nd ghost line

      n1a=eir(0,0)
      n1b=eir(1,0)
      n2a=eir(0,1)
      n2b=eir(1,1)
      n3a=eir(0,2)
      n3b=eir(1,2)
      
      ! We allow interp neighbours to lie 1 grid point outside the "eir" 
      ! ** do not add an extra pt on parallel ghost boundaries ***
      if( pbc(0,0).eq.0 )then
        n1a=eir(0,0)-1
      end if
      if( pbc(1,0).eq.0 )then
        n1b=eir(1,0)+1
      end if
      if( nd.ge.2 )then
        if( pbc(0,1).eq.0 )then
          n2a=n2a-1
        end if
        if( pbc(1,1).eq.0 )then
          n2b=n2b+1
        end if
      end if
      if( nd.ge.3 )then
        if( pbc(0,2).eq.0 )then
          n3a=n3a-1
        end if
        if( pbc(1,2).eq.0 )then
          n3b=n3b+1
        end if
      end if
     

      ! [md1a,md1b]x[md2a,md2b]x[md3a,md3b] : defines the range of valid indicies on the mask where

!!$      md1a=ndm1a
!!$      md1b=ndm1b
!!$      md2a=ndm2a
!!$      md2b=ndm2b
!!$      md3a=ndm3a
!!$      md3b=ndm3b

      numberOfErrors=0

      ! ***************************************
      ! **** Initialize m(i1,i2,i3) = -1 ******
      ! ***************************************
      beginLoops()
        m(i1,i2,i3)=-1
      endLoops()
  
c      write(*,'(" findIN: maxExtrapWidth,ghostBoundaryWidth=",2i3)') maxExtrapWidth,ghostBoundaryWidth

      ! mark points that are outside the dimensions (these are extra parallel ghost boundaries)
      ! We do not need to extrapolate interp neighbours for these points
     
      ! ghostMaskValue=-1
      ! if( (maxExtrapWidth-1) .gt. ghostBoundaryWidth )then
      !   ghostMaskValue=-2
      ! end if

      ! *** turn this off: I don't think we need it -- check me --

!!$      ghostMaskValue=-2  ! *wdh* 091031 -- always do this 
!!$
!!$      beginBoundaryLoops(ndm1a,dim(0,0)-1,ndm2a,ndm2b,ndm3a,ndm3b)
!!$        m(i1,i2,i3)=ghostMaskValue
!!$      endBoundaryLoops()
!!$      beginBoundaryLoops(dim(1,0)+1,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b)
!!$        m(i1,i2,i3)=ghostMaskValue
!!$      endBoundaryLoops()
!!$
!!$      beginBoundaryLoops(ndm1a,ndm1b,ndm2a,dim(0,1)-1,ndm3a,ndm3b)
!!$        m(i1,i2,i3)=ghostMaskValue
!!$      endBoundaryLoops()
!!$      beginBoundaryLoops(ndm1a,ndm1b,dim(1,1)+1,ndm2b,ndm3a,ndm3b)
!!$        m(i1,i2,i3)=ghostMaskValue
!!$      endBoundaryLoops()
!!$
!!$      beginBoundaryLoops(ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,dim(0,2)-1)
!!$        m(i1,i2,i3)=ghostMaskValue
!!$      endBoundaryLoops()
!!$      beginBoundaryLoops(ndm1a,ndm1b,ndm2a,ndm2b,dim(1,2)+1,ndm3b)
!!$        m(i1,i2,i3)=ghostMaskValue
!!$      endBoundaryLoops()


      i2=eir(0,1)
      i3=eir(0,2)

      ! Mark unused "corner points" that are next to interp pts, they may be over-written below
      !        0 X -1  1  1 ...
      !        0 X -1  1  1 ...
      !        0 X -1 -1 -1 ...
      !        0 X  X  X  X   
      !        0 0  0  0  0 ...
      if( nd.eq.1 )then
        do n0=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0)+n0,i2,i3).eq.0 .and. \
                 m(ip(i,0)-n0,i2,i3).ne.-2 )then
  	    m(ip(i,0)+n0,i2,i3)=-n0+1
          end if
        end do
        end do
      else if( nd.eq.2 )then
c        mark corners first, they may be over-written below
        do n0=-1,1,2
        do n1=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0)+n0,ip(i,1)+n1,i3).eq.0 .and. \
                 m(ip(i,0)-n0,ip(i,1)-n1,i3).ne.-2 )then
	    m(ip(i,0)+n0,ip(i,1)+n1,i3)=-n0+1+10*(-n1+1)   ! encode the extrap direction
          end if
        end do
        end do
        end do

        do n1=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0),ip(i,1)+n1,i3).eq.0 .and. \
                 m(ip(i,0),ip(i,1)-n1,i3).ne.-2 )then
	    m(ip(i,0),ip(i,1)+n1,i3)=1+10*(-n1+1)
          end if
        end do
        end do

        do n0=-1,1,2
        do i=0,ni-1
         if( mask(ip(i,0)+n0,ip(i,1),i3).eq.0 .and. \
                 m(ip(i,0)-n0,ip(i,1),i3).ne.-2 )then
	    m(ip(i,0)+n0,ip(i,1),i3)=-n0+1+10
          end if
        end do
        end do

      else 

        ! -- 3D --

        do n0=-1,1,2
        do n1=-1,1,2
        do n2=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0)+n0,ip(i,1)+n1,ip(i,2)+n2).eq.0 .and. \
                 m(ip(i,0)-n0,ip(i,1)-n1,ip(i,2)-n2).ne.-2 )then
	    m(ip(i,0)+n0,ip(i,1)+n1,ip(i,2)+n2)=-n0+1+10*(-n1+1)+100*(-n2+1)
          end if
        end do
        end do
        end do
        end do
        
        do n1=-1,1,2
        do n2=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0),ip(i,1)+n1,ip(i,2)+n2).eq.0 .and. \
                 m(ip(i,0),ip(i,1)-n1,ip(i,2)-n2).ne.-2  )then
	    m(ip(i,0),ip(i,1)+n1,ip(i,2)+n2)=1+10*(-n1+1)+100*(-n2+1)
          end if
        end do
        end do
        end do

        do n0=-1,1,2
        do n2=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0)+n0,ip(i,1),ip(i,2)+n2).eq.0 .and. \
                 m(ip(i,0)-n0,ip(i,1),ip(i,2)-n2).ne.-2  )then
	    m(ip(i,0)+n0,ip(i,1),ip(i,2)+n2)=-n0+1+10+100*(-n2+1)
          end if
        end do
        end do
        end do


        do n0=-1,1,2
        do n1=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0)+n0,ip(i,1)+n1,ip(i,2)).eq.0 .and. \
                 m(ip(i,0)-n0,ip(i,1)-n1,ip(i,2)).ne.-2  )then
	    m(ip(i,0)+n0,ip(i,1)+n1,ip(i,2))=-n0+1+10*(-n1+1)+100
          end if
        end do
        end do
        end do

c        finally the face centres
        do n2=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0),ip(i,1),ip(i,2)+n2).eq.0 .and. \
                 m(ip(i,0),ip(i,1),ip(i,2)-n2).ne.-2  )then
	    m(ip(i,0),ip(i,1),ip(i,2)+n2)=1+10+100*(-n2+1)
          end if
        end do
        end do

        do n1=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0),ip(i,1)+n1,ip(i,2)).eq.0 .and. \
                 m(ip(i,0),ip(i,1)-n1,ip(i,2)).ne.-2  )then
  	    m(ip(i,0),ip(i,1)+n1,ip(i,2))=1+10*(-n1+1)+100
          end if
        end do
        end do

        do n0=-1,1,2
        do i=0,ni-1
          if( mask(ip(i,0)+n0,ip(i,1),ip(i,2)).eq.0 .and. \
                 m(ip(i,0)-n0,ip(i,1),ip(i,2)).ne.-2  )then
	    m(ip(i,0)+n0,ip(i,1),ip(i,2))=-n0+1+10+100
          end if
        end do
        end do
      end if
      
      !  intArray & ia = extrapolateInterpolationNeighbourPoints

      nin=0
      if( nd.eq.2 )then
        beginLoops()
        if( m(i1,i2,i3).ge.0 )then
          ia(nin,0)=i1
          ia(nin,1)=i2

          mm=m(i1,i2,i3)
          id(nin,1)=mm/10-1
          id(nin,0)=mm-(id(nin,1)+1)*10-1

          nin=nin+1
        end if
        endLoops()
      else if( nd.eq.3 )then
        beginLoops()
        if( m(i1,i2,i3).ge.0 )then
          ia(nin,0)=i1
          ia(nin,1)=i2
          ia(nin,2)=i3

          mm=m(i1,i2,i3)
          id(nin,2)=mm/100-1
          mm=mm-(id(nin,2)+1)*100
          id(nin,1)=mm/10-1
          id(nin,0)=mm-(id(nin,1)+1)*10-1

          nin=nin+1
        end if
        endLoops()
      else
        beginLoops()
        if( m(i1,i2,i3).ge.0 )then
          ia(nin,0)=i1
          id(nin,0)=m(i1,i2,i3)
          nin=nin+1
        end if
        endLoops()
      end if

      ! *************************
      ! ***** check results *****
      ! *************************

      ! Sometimes a "hanging" interpolation point will be created that is needed for discretization
      ! and it's neighbours may not extrapolate in a valid direction

      ! Mark unused "corner points" that are next to interp pts, they may be over-written below
      !        0  X -1  1  1 ...
      !        Y  X -1  1  1 ...
      !        Y -2 -1 -1 -1 ...   <-  pt -2 is a hanging interpolation point, no need to assign Y's 
      !        Y  X  X  X  X   
      !        0  0  0  0  0 ...

      if( .true. .and. nd.eq.2 )then
        ! ******************************************
        ! ************* 2D *************************
        ! ******************************************

        ndm(0,0)=ndm1a
        ndm(1,0)=ndm1b
        ndm(0,1)=ndm2a
        ndm(1,1)=ndm2b

        i3=ndm3a

        do i=0,nin-1  ! ************** check all interpolation neighbours *****
          
          !write(*,'(" findIN: myid=",i2," i=",i3," ia=",2i4," id=",2i3)') myid,i,ia(i,0),ia(i,1),id(i,0),id(i,1)
          !write(*,'(" findIN: ndm1a,ndm1b,...=",6i4)') ndm1a,ndm1b,ndm2a,ndm2b

          ! status(i)=0  : means we can evaluate the pt on this processor: 
          !          =2  : this point cannot extrap on this processor
          !          =-1 : this point is not needed

          ierr=0
          if(ia(i,0).lt.ndm1a .or. ia(i,0).gt.ndm1b .or. \
             ia(i,0)+maxExtrap*id(i,0).lt.ndm1a .or.\
             ia(i,0)+maxExtrap*id(i,0).gt.ndm1b .or.\
             ia(i,1).lt.ndm2a .or. ia(i,1).gt.ndm2b .or.\
             ia(i,1)+maxExtrap*id(i,1).lt.ndm2a .or.\
             ia(i,1)+maxExtrap*id(i,1).gt.ndm2b )then

            ! extrap. formula lies outside the local array bounds : 
            ! This could be a parallel case. 
            ierr=2 

          end if    

          ! now check the points used in the extrapolation formula
          if( ierr.eq.0 )then
            do mm=1,maxExtrap
              i1=ia(i,0)+mm*id(i,0)
              i2=ia(i,1)+mm*id(i,1)
              if( mask(i1,i2,i3).eq.0 )then
                ! This direction is invalid 
                ierr=1
              end if
            end do
          end if

          if( ierr.ne.0 )then ! *wdh* 060319 -- check if this point really needs to be assigned
            ierr1=0
            m3=0
            do m2=-maxExtrap,maxExtrap
            do m1=-maxExtrap,maxExtrap
              i1=ia(i,0)+m1
              i2=ia(i,1)+m2
              if( i1.ge.ndm1a .and. i1.le.ndm1b .and.
     &            i2.ge.ndm2a .and. i2.le.ndm2b )then
                if( mask(i1,i2,i3).gt.0 )then
                  ! There is a discretization points that needs this value assigned
                  ierr1=ierr
                end if
              end if
            end do
            end do   
            ierr=ierr1
            if( ierr.eq.0 )then ! this point does not need to be assigned
               ! just set the direction to zero to make the formula valid
               ! we could remove this point from the list
               ierr=-1 ! not needed
               id(i,0)=0
               id(i,1)=0
               ! write(*,'("findInterNeigh:pt is not needed afterall")') 
            end if
          end if


          if( ierr.gt.0 )then
            ! The extrapolation formula is invalid,
            ! look in all other directions for a valid extrapolation formula
            found=.false.
            m3=0
            ierr1=ierr
            do m2=-1,1
            do m1=-1,1
              if( ia(i,0)+maxExtrap*m1.ge.ndm(0,0) .and. \
                  ia(i,0)+maxExtrap*m1.le.ndm(1,0) .and. \
                  ia(i,1)+maxExtrap*m2.ge.ndm(0,1) .and. \
                  ia(i,1)+maxExtrap*m2.le.ndm(1,1) .and. \
                  mask(ia(i,0)+m1,ia(i,1)+m2,i3).ne.0 )then
                ierr1=0
                do mm=2,maxExtrap
                  if( mask(ia(i,0)+mm*m1,ia(i,1)+mm*m2,i3).eq.0 )then
                    ! This direction is invalid 
                    ierr1=ierr
                  end if
                end do
              end if
              if( ierr1.eq.0 )then
                found=.true.
                id(i,0)=m1
                id(i,1)=m2
                ! Normally the first point in the extrapolation direction should be an interp point
                ! This may not be possible in some very special cases
                if( mask(ia(i,0)+m1,ia(i,1)+m2,i3).lt.0 )then
                  ! break from the loop
                  goto 200
                else
                  ! keep looking for a better case
                  ierr1=ierr
                end if
              end if
            end do
            end do
 200        continue
            if( found )then
              ierr=0
            end if
            if( ierr.eq.0 )then
              do mm=0,maxExtrap
                i1=ia(i,0)+mm*id(i,0)
                i2=ia(i,1)+mm*id(i,1)
                if( mask(i1,i2,i3).gt.0 )then
                  imask(mm,0,0)=1
                else if( mask(i1,i2,i3).lt.0 )then
                  imask(mm,0,0)=-1
                else
                  imask(mm,0,0)=0
                end if
              end do                
c              write(*,'("findInterNeigh:INFO: new extrap dir '//
c     &          'found, i=",i6," ia=",2i4," id=",2i4," mask=",4i3)') i,
c     &         ia(i,0),ia(i,1),id(i,0),id(i,1),
c     &         imask(0,0,0),imask(1,0,0),imask(2,0,0),imask(3,0,0)
            else
              ! write(*,'("findInterNeigh:ERROR: unable to find new extrap direction")') 
               ! '
            end if
          end if

          status(i)=ierr


          if( ierr.gt.0 .and. ierr.ne.2 )then
            numberOfErrors=numberOfErrors+1

            write(*,'("findInterNeigh:ERROR i=",i6," ia=",2i4," id=",'//
     &  '2i4," mask=0 at i1,i2,i3=",3i4)') i,ia(i,0),ia(i,1),
     & id(i,0),id(i,1),i1,i2,i3

            write(*,'("findInterNeigh: ndm1a,ndm1b,...=",4i4)') 
     &          ndm1a,ndm1b,ndm2a,ndm2b
            if( .false. )then ! print the mask in a nice way
             m3=0
             do m2=-3,3
             do m1=-3,3
              i1=ia(i,0)+m1
              i2=ia(i,1)+m2
              if( i1.ge.ndm1a .and. i1.le.ndm1b .and.
     &            i2.ge.ndm2a .and. i2.le.ndm2b )then
                if( mask(i1,i2,i3).gt.0 )then
                  imask(m1,m2,m3)=1
                else if( mask(i1,i2,i3).lt.0 )then
                  imask(m1,m2,m3)=-1
                else
                  imask(m1,m2,m3)=0
                end if
              else
                imask(m1,m2,m3)=2
              end if
             end do
             end do
            
             write(*,'("mask: (2=outside bounds)",/,(7i3))') 
     &             ((imask(m1,m2,m3),m1=-3,3),m2=-3,3)
          
            end if
            ! stop 7777
            ! *wdh* 091121 return
          end if

        end do

      else  if( .true. .and. nd.eq.3 )then
        ! ******************************************
        ! ************* 3D *************************
        ! ******************************************

        ndm(0,0)=ndm1a
        ndm(1,0)=ndm1b
        ndm(0,1)=ndm2a
        ndm(1,1)=ndm2b
        ndm(0,2)=ndm3a
        ndm(1,2)=ndm3b

        do i=0,nin-1  ! ************** check all interpolation neighbours *****
          
          ! write(*,'(" findIN: myid=",i2," i=",i4," ia=",3i4," id=",3i3)') myid,i,ia(i,0),ia(i,1),ia(i,2),id(i,0),id(i,1),id(i,2)

          status(i)=0 

          ierr=0
          if( ia(i,0).lt.ndm1a .or. ia(i,0).gt.ndm1b .or. \
             ia(i,0)+maxExtrap*id(i,0).lt.ndm1a .or.\
             ia(i,0)+maxExtrap*id(i,0).gt.ndm1b .or.\
             ia(i,1).lt.ndm2a .or. ia(i,1).gt.ndm2b .or.\
             ia(i,1)+maxExtrap*id(i,1).lt.ndm2a .or.\
             ia(i,1)+maxExtrap*id(i,1).gt.ndm2b .or.\
             ia(i,2).lt.ndm3a .or. ia(i,2).gt.ndm3b .or.\
             ia(i,2)+maxExtrap*id(i,2).lt.ndm3a .or.\
             ia(i,2)+maxExtrap*id(i,2).gt.ndm3b )then

            ! extrap. formula lies outside the local array bounds : 
            ! This could be a parallel case. 
            ierr=2 

          end if    

          ! now check the points used in the extrapolation formula
          if( ierr.eq.0 )then
            do mm=1,maxExtrap
              i1=ia(i,0)+mm*id(i,0)
              i2=ia(i,1)+mm*id(i,1)
              i3=ia(i,2)+mm*id(i,2)
              if( mask(i1,i2,i3).eq.0 )then
                ! This direction is invalid 
                ierr=1
              end if
            end do
          end if

          if( ierr.ne.0 )then ! *wdh* 060319 -- check if this point really needs to be assigned
            ierr1=0
            do m3=-maxExtrap,maxExtrap
            do m2=-maxExtrap,maxExtrap
            do m1=-maxExtrap,maxExtrap
              i1=ia(i,0)+m1
              i2=ia(i,1)+m2
              i3=ia(i,2)+m3
              if( i1.ge.ndm1a .and. i1.le.ndm1b .and.
     &            i2.ge.ndm2a .and. i2.le.ndm2b .and.
     &            i3.ge.ndm3a .and. i3.le.ndm3b )then
                if( mask(i1,i2,i3).gt.0 )then
                  ! There is a discretization points that needs this value assigned
                  ierr1=ierr
                end if
              end if
            end do
            end do   
            end do   
            ierr=ierr1
            if( ierr.eq.0 )then ! this point does not need to be assigned
               ! just set the direction to zero to make the formula valid
               ! we could remove this point from the list
               ierr=-1 ! not needed
               id(i,0)=0
               id(i,1)=0
               id(i,2)=0
            end if
          end if


          if( ierr.gt.0 )then
            ! The extrapolation formula is invalid,
            ! look in all other directions for a valid extrapolation formula
            found=.false.
            ierr1=ierr
            do m3=-1,1
            do m2=-1,1
            do m1=-1,1
              if( ia(i,0)+maxExtrap*m1.ge.ndm(0,0) .and. \
                  ia(i,0)+maxExtrap*m1.le.ndm(1,0) .and. \
                  ia(i,1)+maxExtrap*m2.ge.ndm(0,1) .and. \
                  ia(i,1)+maxExtrap*m2.le.ndm(1,1) .and. \
                  ia(i,2)+maxExtrap*m3.ge.ndm(0,2) .and. \
                  ia(i,2)+maxExtrap*m3.le.ndm(1,2) .and. \
                  mask(ia(i,0)+m1,ia(i,1)+m2,ia(i,2)+m3).ne.0 )then
                ierr1=0
                do mm=2,maxExtrap
                  if( mask(ia(i,0)+mm*m1,ia(i,1)+mm*m2,ia(i,2)+mm*m3).eq.0 )then
                    ! This direction is invalid 
                    ierr1=ierr
                  end if
                end do
              end if
              if( ierr1.eq.0 )then
                found=.true.
                id(i,0)=m1
                id(i,1)=m2
                id(i,2)=m3
                ! Normally the first point in the extrapolation direction should be an interp point
                ! This may not be possible in some very special cases
                if( mask(ia(i,0)+m1,ia(i,1)+m2,ia(i,2)+m3).lt.0 )then
                  ! break from the loop
                  goto 300
                else
                  ! keep looking for a better case
                  ierr1=ierr
                end if
              end if
            end do
            end do
            end do
 300        continue
            if( found )then
              ierr=0
            end if
            if( ierr.eq.0 )then
              if( .false. )then
                ! debug output
               do mm=0,maxExtrap
                 i1=ia(i,0)+mm*id(i,0)
                 i2=ia(i,1)+mm*id(i,1)
                 i3=ia(i,2)+mm*id(i,2)
                 if( mask(i1,i2,i3).gt.0 )then
                   imask(mm,0,0)=1
                 else if( mask(i1,i2,i3).lt.0 )then
                   imask(mm,0,0)=-1
                 else
                   imask(mm,0,0)=0
                 end if
                end do                
               write(*,'("findInterNeigh:INFO: new extrap dir '//
     &           'found, i=",i6," ia=",3i4," id=",3i4," mask=",4i3)') i,
     &           ia(i,0),ia(i,1),ia(i,2),id(i,0),id(i,1),id(i,2),
     &           imask(0,0,0),imask(1,0,0),imask(2,0,0),imask(3,0,0)
              end if

            else if( ierr.gt.0 )then

              ! As a final check -- make sure this point is really needed by some discretization point
              ! This check was added 040808 *wdh*
              ierr1=0 
              do m3=-2,2
              do m2=-2,2
              do m1=-2,2
                i1=ia(i,0)+m1
                i2=ia(i,1)+m2
                i3=ia(i,2)+m3
                if( i1.ge.ndm1a .and. i1.le.ndm1b .and.
     &              i2.ge.ndm2a .and. i2.le.ndm2b .and.
     &              i3.ge.ndm3a .and. i3.le.ndm3b )then
                  if( mask(i1,i2,i3).gt.0 )then
                    j1=i1
                    j2=i2
                    j3=i3
                    ierr1=ierr    ! discr. pt (j1,j2,j3) needs point ia(i,.) to be assigned.
                  end if
                end if
              end do
              end do
              end do
              ierr=ierr1
              if( ierr.gt.0 .and. ierr.ne.2 )then
                write(*,'("findInterNeigh:ERROR: unable to find new extrap direction")') 
                write(*,'(" unused point ia(i,0:2)=(",3i4,") is needed by discr. pt (",3i4,")")') i1,i2,i3,j1,j2,j3
                ! "
              end if

            end if
          end if

          status(i)=ierr


          ! ierr=-1 Ok, ierr=2 ok 
          if( ierr.eq.1 )then

            numberOfErrors=numberOfErrors+1

            write(*,'("findInterNeigh:ERROR i=",i6," ia=",3i4," id=",'//
     &  '3i4," mask=0 at i1,i2,i3=",3i4)') i,ia(i,0),ia(i,1),ia(i,2),
     & id(i,0),id(i,1),id(i,2),i1,i2,i3

            write(*,'("findInterNeigh: ndm1a,ndm1b,...=",6i4)') 
     &          ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b
            if( .false. )then ! print the mask in a nice way
             do m3=-3,3
             do m2=-3,3
             do m1=-3,3
              i1=ia(i,0)+m1
              i2=ia(i,1)+m2
              i3=ia(i,2)+m3
              if( i1.ge.ndm1a .and. i1.le.ndm1b .and.
     &            i2.ge.ndm2a .and. i2.le.ndm2b .and.
     &            i3.ge.ndm3a .and. i3.le.ndm3b )then
                if( mask(i1,i2,i3).gt.0 )then
                  imask(m1,m2,m3)=1
                else if( mask(i1,i2,i3).lt.0 )then
                  imask(m1,m2,m3)=-1
                else
                  imask(m1,m2,m3)=0
                end if
              else
                imask(m1,m2,m3)=2
              end if
             end do
             end do
             end do
            
             write(*,'("mask: (2=outside bounds)",/,(7i3))') imask
            end if
            ! stop 8888
            ! *wdh* 091121 return

          end if

        end do
      end if

      ierr=numberOfErrors

      return
      end   

