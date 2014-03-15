c =================================================================================================
c
c  Optimized high-order artificial dissipation as a separate filter step
c =================================================================================================

#beginMacro beginLoopsWithMaskOld()
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
 if( mask(i1,i2,i3).gt.0 )then
 do n=nStart,nEnd
#endMacro

#beginMacro endLoopsWithMaskOld()
 end do
 end if
end do
end do
end do
#endMacro

! Putting the "n" loop outside was slightly faster for a Cartesian grid -- we need to check this more
#beginMacro beginLoopsWithMask()
do n=nStart,nEnd
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
 if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoopsWithMask()
 end if
end do
end do
end do
end do
#endMacro

c ************************************************************************************************
c  This macro is used for looping over the faces of a grid to assign booundary conditions
c
c extra: extra points to assign
c          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
c          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
c numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
c ***********************************************************************************************
#beginMacro beginLoopOverSides(extra,numberOfGhostPoints)
 extra1a=extra
 extra1b=extra
 extra2a=extra
 extra2b=extra
 if( nd.eq.3 )then
   extra3a=extra
   extra3b=extra
 else
   extra3a=0
   extra3b=0
 end if
 if( boundaryCondition(0,0).lt.0 )then
   extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions
   extra1b=extra1a
 else
   if( boundaryCondition(0,0).eq.0 )then
     extra1a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
   end if
   if( boundaryCondition(1,0).eq.0 )then
     extra1b=numberOfGhostPoints
   end if
 end if
 if( boundaryCondition(0,1).lt.0 )then
  extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions
  extra2b=extra2a
 else 
   if( boundaryCondition(0,1).eq.0 )then
     extra2a=numberOfGhostPoints
   end if
   if( boundaryCondition(1,1).eq.0 )then
     extra2b=numberOfGhostPoints
   end if
 end if
 if(  nd.eq.3 .and. boundaryCondition(0,2).lt.0 )then
  extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions
  extra3b=extra3a
 else 
   if( boundaryCondition(0,2).eq.0 )then
     extra3a=numberOfGhostPoints
   end if
   if( boundaryCondition(1,2).eq.0 )then
     extra3b=numberOfGhostPoints
   end if
 end if

 do axis=0,nd-1
 do side=0,1

   n1a=gridIndexRange(0,0)-extra1a
   n1b=gridIndexRange(1,0)+extra1b
   n2a=gridIndexRange(0,1)-extra2a
   n2b=gridIndexRange(1,1)+extra2b
   n3a=gridIndexRange(0,2)-extra3a
   n3b=gridIndexRange(1,2)+extra3b
   if( axis.eq.0 )then
     n1a=gridIndexRange(side,axis)
     n1b=gridIndexRange(side,axis)
   else if( axis.eq.1 )then
     n2a=gridIndexRange(side,axis)
     n2b=gridIndexRange(side,axis)
   else
     n3a=gridIndexRange(side,axis)
     n3b=gridIndexRange(side,axis)
   end if
   is1=0
   is2=0
   is3=0
   if( axis.eq.0 )then
     is1=1-2*side
   else if( axis.eq.1 )then
     is2=1-2*side
   else if( axis.eq.2 )then
     is3=1-2*side
   else
     stop 5
   end if
   
   axisp1=mod(axis+1,nd)
   axisp2=mod(axis+2,nd)
   
   if( n1a.lt.nd1a .or. n1b.gt.nd1b .or. n2a.lt.nd2a .or. n2b.gt.nd2b .or. n3a.lt.nd3a .or. n3b.gt.nd3b )then
     write(*,'("mxFilter: ERROR: in bounds n1a,n1b,n2a,...")')
     stop 0101
   end if

#endMacro

#beginMacro endLoopOverSides()
 end do
 end do
 ! reset these values
 n1a=gridIndexRange(0,0)
 n1b=gridIndexRange(1,0)
 n2a=gridIndexRange(0,1)
 n2b=gridIndexRange(1,1)
 n3a=gridIndexRange(0,2)
 n3b=gridIndexRange(1,2)
#endMacro


! 2nd order dissipation 2D:
#defineMacro FD2_2D(u,i1,i2,i3,c) \
      ( ( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
       -4.*u(i1,i2,i3,c) )

! 2nd order dissipation 3D:
#defineMacro FD2_3D(u,i1,i2,i3,c) \
      (  ( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) \
       -6.*u(i1,i2,i3,c) )

! fourth order dissipation 2D:
#defineMacro FD4_2D(u,i1,i2,i3,c) \
      (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   \
        +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
       -12.*u(i1,i2,i3,c) )

! fourth order dissipation 3D:
#defineMacro FD4_3D(u,i1,i2,i3,c) \
      (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c)+u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) )   \
        +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) \
       -18.*u(i1,i2,i3,c) )


#defineMacro extrap1(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (uu(k1,k2,k3,kc))

#defineMacro extrap2(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (2.*uu(k1,k2,k3,kc)-uu(k1+ks1,k2+ks2,k3+ks3,kc))

#defineMacro extrap3(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (3.*uu(k1,k2,k3,kc)-3.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +   uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc))

#defineMacro extrap4(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (4.*uu(k1,k2,k3,kc)-6.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +4.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc))

#defineMacro extrap5(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (5.*uu(k1,k2,k3,kc)-10.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +10.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-5.*uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc)\
            +uu(k1+4*ks1,k2+4*ks2,k3+4*ks3,kc))


      subroutine cgFilter( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                        gridIndexRange, u, d, mask, boundaryCondition, ipar, rpar, ierr )
c =================================================================================================
c  Optimized high-order artificial dissipation as a separate filter step
c
c   The high-order filter is applied in two stages and means we can apply an 8th order filter
c   on a fourth-order grid or a fourth-order filter on a second order grid. 
c
c
c   u (input) : solution to be filtered 
c   d (input) : work space (assumed set to zero before first call)
c   option = ipar(0) : 
c          option = 0 : compute stage I dissipation and save in d
c          option = 1 : compute state II dissipation and save in u 
c =================================================================================================


      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
              n1a,n1b,n2a,n2b,n3a,n3b, ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real d(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer gridIndexRange(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

c     ... local
      integer option,orderOfArtificialDissipation,nStart,nEnd,debug,myid,init,numberOfStages
      real ad,dt,t
      integer side,axis,axisp1,axisp2,i1,i2,i3,is1,is2,is3,n
      integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b,numberOfGhostPoints

      ierr=0

      n1a=gridIndexRange(0,0)
      n1b=gridIndexRange(1,0)
      n2a=gridIndexRange(0,1)
      n2b=gridIndexRange(1,1)
      n3a=gridIndexRange(0,2)
      n3b=gridIndexRange(1,2)


      option = ipar(0)
      nStart = ipar(1)
      nEnd   = ipar(2)
      orderOfArtificialDissipation = ipar(3)
      debug  = ipar(4)
      myid   = ipar(5)
      init   = ipar(6)
      numberOfStages=ipar(7)

      ad = rpar(0)

      if( numberOfStages.ne.1 .and. numberOfStages.ne.2 )then
        write(*,'(" cgFilter:ERROR: numberOfStages=",i6)') numberOfStages
        stop 1153
      end if

      if( init.eq.1 .and. myid.eq.0 )then
        if( numberOfStages.eq.1 )then
            write(*,'(" cgFilter: One-stage-filter: ad=",e9.3," orderOfDissipation=",i4," nStart,nEnd=",2i4)') ad,orderOfArtificialDissipation,nStart,nEnd
        else 
          if( option.eq.0 )then
            write(*,'(" cgFilter: Stage I orderOfDissipation=",i4," nStart,nEnd=",2i4)') orderOfArtificialDissipation,nStart,nEnd
          else
            write(*,'(" cgFilter: Stage II ad = ",1(e10.2,1x))') ad
          end if
        end if
      end if
      if( orderOfArtificialDissipation.ne.4 .and. orderOfArtificialDissipation.ne.8 .and.\
          orderOfArtificialDissipation.ne.6 )then
        write(*,'(" cgFilter:ERROR: orderOfArtificialDissipation=",i6)') orderOfArtificialDissipation
        ! ' 
        stop 1155
      end if

      if( numberOfStages.eq.2 .and. option .eq.0 )then
        ! Stage I : 
       if( orderOfArtificialDissipation.eq.4 )then
         if( nd.eq.2 )then
          beginLoopsWithMask()
           d(i1,i2,i3,n)=FD2_2D(u,i1,i2,i3,n)
          endLoopsWithMask()
         else
          beginLoopsWithMask()
           d(i1,i2,i3,n)=FD2_3D(u,i1,i2,i3,n)
          endLoopsWithMask()
         end if
       else if( orderOfArtificialDissipation.eq.6 .or. \
                orderOfArtificialDissipation.eq.8 )then
         ! For 6th or 8th order 2-stage we start with a 4th order Stage I
         if( nd.eq.2 )then
          beginLoopsWithMask()
           d(i1,i2,i3,n)=FD4_2D(u,i1,i2,i3,n)
          endLoopsWithMask()
         else
          beginLoopsWithMask()
           d(i1,i2,i3,n)=FD4_3D(u,i1,i2,i3,n)
          endLoopsWithMask()
         end if
       else
         ! unknown orderOfArtificialDissipation
         stop 4040
       end if

       ! ------------ assign ghost points of the dissipation ---------
       if( .false. )then ! turn this off for now -- may not be needed ---
        numberOfGhostPoints=1 ! orderOfArtificialDissipation/4
        extra= numberOfGhostPoints  ! assign extra pts in tangential direction to we assign corners
        beginLoopOverSides(extra,numberOfGhostPoints)
         if( boundaryCondition(side,axis).gt.0 )then
           beginLoopsWithMask()
             d(i1-is1,i2-is2,i3-is3,n)=extrap1(d,i1,i2,i3,n,is1,is2,is3)
           endLoopsWithMask()
         end if
        endLoopOverSides()
       end if
      end if

      if( numberOfStages.eq.2 .and. option .eq.1 )then
        ! Stage II : 
       if( .false. )then
          ! for testing
          beginLoopsWithMask()
           u(i1,i2,i3,n)=u(i1,i2,i3,n) - ad*d(i1,i2,i3,n)
          endLoopsWithMask()
       else if( orderOfArtificialDissipation.eq.4 .or.\
                orderOfArtificialDissipation.eq.6 )then
         if( nd.eq.2 )then
          beginLoopsWithMask()
           u(i1,i2,i3,n)=u(i1,i2,i3,n) + ad*FD2_2D(d,i1,i2,i3,n)
          endLoopsWithMask()
         else
          beginLoopsWithMask()
           u(i1,i2,i3,n)=u(i1,i2,i3,n) + ad*FD2_3D(d,i1,i2,i3,n)
          endLoopsWithMask()
        end if
       else if( orderOfArtificialDissipation.eq.8 )then
         if( nd.eq.2 )then
          beginLoopsWithMask()
           u(i1,i2,i3,n)=u(i1,i2,i3,n) + ad*FD4_2D(d,i1,i2,i3,n)
          endLoopsWithMask()
         else
          beginLoopsWithMask()
           u(i1,i2,i3,n)=u(i1,i2,i3,n) + ad*FD4_3D(d,i1,i2,i3,n)
          endLoopsWithMask()
        end if
       else
         ! unknown orderOfArtificialDissipation
         stop 4041
       end if
      end if


      if( numberOfStages.eq.1 )then
        ! -- One-stage filter --

        ! write(*,'(" cgFilter: Apply one-stage-filter: ad=",e9.3," orderOfDissipation=",i4," nStart,nEnd=",2i4') ad,orderOfArtificialDissipation,nStart,nEnd

       if( orderOfArtificialDissipation.eq.2 )then
         if( nd.eq.2 )then
          beginLoopsWithMask()
           d(i1,i2,i3,n)=u(i1,i2,i3,n) - ad*FD2_2D(u,i1,i2,i3,n)
          endLoopsWithMask()
          beginLoopsWithMask()
           u(i1,i2,i3,n)=d(i1,i2,i3,n)
          endLoopsWithMask()
         else
          beginLoopsWithMask()
           d(i1,i2,i3,n)=u(i1,i2,i3,n) - ad*FD2_3D(u,i1,i2,i3,n)
          endLoopsWithMask()
          beginLoopsWithMask()
           u(i1,i2,i3,n)=d(i1,i2,i3,n) 
          endLoopsWithMask()
        end if
       else if( orderOfArtificialDissipation.eq.4 )then
         if( nd.eq.2 )then
          beginLoopsWithMask()
           d(i1,i2,i3,n)=u(i1,i2,i3,n) - ad*FD4_2D(u,i1,i2,i3,n)
          endLoopsWithMask()
          beginLoopsWithMask()
           u(i1,i2,i3,n)=d(i1,i2,i3,n)
          endLoopsWithMask()
         else
          beginLoopsWithMask()
           d(i1,i2,i3,n)=u(i1,i2,i3,n) - ad*FD4_3D(u,i1,i2,i3,n)
          endLoopsWithMask()
          beginLoopsWithMask()
           u(i1,i2,i3,n)=d(i1,i2,i3,n)
          endLoopsWithMask()
        end if
       else
         ! unknown orderOfArtificialDissipation
         stop 4042
       end if
      end if


      return 
      end
