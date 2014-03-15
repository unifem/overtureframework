c This macro can be used to turn on the checking of the mask
#beginMacro beginCheckForMask()
#endMacro
#beginMacro endCheckForMask()
#endMacro

c This macro defines the inner loops -- we treat ratio=2 as a special case - not sure if this is faster?
#beginMacro LOOPS( FORMULA )
 if( ratio1.eq.2 .and. ratio2.eq.2 )then
  do c=ca,cb
  do j3=nta,ntb
  i3=j3*ratio3
  do j2=nsa,nsb
  i2=j2*2
   do j1=nra,nrb
    beginCheckForMask()
     i1=j1*2  ! could add 2 instead??
     FORMULA
    endCheckForMask()
   end do
  end do
  end do
  end do
 else 
  ! general ratios
  do c=ca,cb
  do j3=nta,ntb
  i3=j3*ratio3
  do j2=nsa,nsb
  i2=j2*ratio2
   do j1=nra,nrb
    beginCheckForMask()
     i1=j1*ratio1
     FORMULA
    endCheckForMask()
   end do
  end do
  end do
  end do
 end if
#endMacro



#beginMacro INTERP_LOOPS(ND,UPDATE)

if( centering1.ne.vertex .or. centering2.ne.vertex .or. centering3.ne.vertex )then
  write(*,*) 'interpCoarseFromFine:ERROR:cell-centred not implemented'
  stop 1
end if

if( option.eq.fullWeighting110 )then
 ! full weighting on the x-y plane
 LOOPS( UPDATE .25*uf(i1,i2,i3,c)+.125*(uf(i1-1,i2,i3,c)+uf(i1+1,i2,i3,c)+uf(i1,i2-1,i3,c)+uf(i1,i2+1,i3,c)) \
            +.0625*(uf(i1-1,i2-1,i3,c)+uf(i1+1,i2-1,i3,c)+uf(i1-1,i2+1,i3,c)+uf(i1+1,i2+1,i3,c)) )

else if( option.eq.fullWeighting111 )then
 ! 3D full weighting
 LOOPS( UPDATE .125*uf(i1,i2,i3,c)\
  +.0625*(uf(i1-1,i2,i3,c)+uf(i1+1,i2,i3,c)+uf(i1,i2-1,i3,c)+uf(i1,i2+1,i3,c)+uf(i1,i2,i3-1,c)+uf(i1,i2,i3+1,c)) \
 +.03125*(uf(i1-1,i2-1,i3,c)+uf(i1+1,i2-1,i3,c)+uf(i1-1,i2+1,i3,c)+uf(i1+1,i2+1,i3,c)\
         +uf(i1-1,i2,i3-1,c)+uf(i1+1,i2,i3-1,c)+uf(i1-1,i2,i3+1,c)+uf(i1+1,i2,i3+1,c)\
         +uf(i1,i2-1,i3-1,c)+uf(i1,i2+1,i3-1,c)+uf(i1,i2-1,i3+1,c)+uf(i1,i2+1,i3+1,c))\
+.015625*(uf(i1-1,i2-1,i3-1,c)+uf(i1+1,i2-1,i3-1,c)+uf(i1-1,i2+1,i3-1,c)+uf(i1+1,i2+1,i3-1,c)\
         +uf(i1-1,i2-1,i3+1,c)+uf(i1+1,i2-1,i3+1,c)+uf(i1-1,i2+1,i3+1,c)+uf(i1+1,i2+1,i3+1,c)) )

else if( option.eq.fullWeighting100 )then
 ! full weighting along x-axis
 LOOPS( UPDATE .5*uf(i1,i2,i3,c)+.25*(uf(i1-1,i2,i3,c)+uf(i1+1,i2,i3,c)) )

else if( option.eq.fullWeighting010 )then
 ! full weighting along y-axis
 LOOPS( UPDATE .5*uf(i1,i2,i3,c)+.25*(uf(i1,i2-1,i3,c)+uf(i1,i2+1,i3,c)) )

else if( option.eq.fullWeighting001 )then
  ! full weighting along z-axis
 LOOPS( UPDATE .5*uf(i1,i2,i3,c)+.25*(uf(i1,i2,i3-1,c)+uf(i1,i2,i3+1,c)) )

else if( option.eq.fullWeighting101 )then
  ! full weighting on x-z plane
 LOOPS( UPDATE .25*uf(i1,i2,i3,c)+.125*(uf(i1-1,i2,i3,c)+uf(i1+1,i2,i3,c)+uf(i1,i2,i3-1,c)+uf(i1,i2,i3+1,c)) \
            +.0625*(uf(i1-1,i2,i3-1,c)+uf(i1+1,i2,i3-1,c)+uf(i1-1,i2,i3+1,c)+uf(i1+1,i2,i3+1,c)) )

else if( option.eq.fullWeighting011 )then
  ! full weighting on y-z plane
 LOOPS( UPDATE .25*uf(i1,i2,i3,c)+.125*(uf(i1,i2-1,i3,c)+uf(i1,i2+1,i3,c)+uf(i1,i2,i3-1,c)+uf(i1,i2,i3+1,c)) \
            +.0625*(uf(i1,i2-1,i3-1,c)+uf(i1,i2+1,i3-1,c)+uf(i1,i2-1,i3+1,c)+uf(i1,i2+1,i3+1,c)) )


else if( option.eq.injection )then
  do c=ca,cb
  do j3=nta,ntb
  i3=j3*ratio3
  do j2=nsa,nsb
  i2=j2*ratio2
   do j1=nra,nrb
    beginCheckForMask()
     i1=j1*ratio1
     UPDATE uf(i1,i2,i3,c)
    endCheckForMask()
   end do
  end do
  end do
  end do


else
  write(*,*) 'interpCoarseFromFine:ERROR:un-implemented options'
  stop 1
end if 

#endMacro




      subroutine interpCoarseFromFine( ndfra,ndfrb,ndfsa,ndfsb,ndfta,ndftb,uf,\
                                 ndcra,ndcrb,ndcsa,ndcsb,ndcta,ndctb,uc,\
                                 nd,nra,nrb,nsa,nsb,nta,ntb, \
                                 width,ratios, ndca,ca,cb, ishift, centerings, option, update, mask, ipar )
c ==================================================================================
c Interpolate coarse grid values from fine grid values.
c
c  uf : fine grid patch
c  uc : coarse grid patch
c  uc(nra:nrb, nsa:nsb, nta:ntb, ca:ca) - interpolate these values.
c  width : interpolation width 
c ratio(1:3) : refinement ratios in each direction
c ishift(1:3) : if 0 prefer a stencil to the right, if 1 prefer a stencil to the left (when there is a choice)
c              When the width is odd there is a choice of stencil for some points
c centerings(1:3) : 0=vertex centred, 1=cell centred
c option : one of injection,fullWeighting100,fullWeighting010,fullWeighting001,fullWeighting110,
c    fullWeighting101,fullWeighting011,fullWeighting111
c update : 0=set uc=interpolant(uf) 1=set uc=uc+interpolant(uf)
c ==================================================================================
      ! implicit none
      real uf(ndfra:ndfrb,ndfsa:ndfsb,ndfta:ndftb,ndca:*)
      real uc(ndcra:ndcrb,ndcsa:ndcsb,ndcta:ndctb,ndca:*)
      integer ca,cb,width,centerings(*)
      integer ratios(*),ishift(*),option,update
      integer mask(ndcra:ndcrb,ndcsa:ndcsb,ndcta:ndctb)  ! mask is from the coarse grid *wdh* 040504
      integer ipar(0:*)
      integer i1,i2,i3,j1,j2,j3,c,ratio,ratio1,ratio2,ratio3
      integer centering,centering1,centering2,centering3

      integer injection,fullWeighting100,fullWeighting010,fullWeighting001,fullWeighting110,\
         fullWeighting101,fullWeighting011,fullWeighting111
      parameter( injection=0,fullWeighting100=1,fullWeighting010=2,fullWeighting001=3,fullWeighting110=4,\
         fullWeighting101=5,fullWeighting011=6,fullWeighting111=7)
      integer vertex,cell
      parameter( vertex=0, cell=1 )

      integer maskOption
      integer doNotUseMask, maskGreaterThanZero,maskEqualZero
      parameter( doNotUseMask=0, maskGreaterThanZero=1, maskEqualZero=2 )
c.. begin statement functions
c.. end statement functions


      maskOption=ipar(0)

      ratio1=ratios(1)
      ratio2=ratios(2)
      ratio3=ratios(3)

      centering1=centerings(1)
      centering2=centerings(2)
      centering3=centerings(3)
      centering=centering1

      if( nd.eq.1 )then
        ratio2=1
        ratio3=1
      else if( nd.eq.2 )then
        ratio3=1
      else
      end if

      ratio=max(ratio1,ratio2,ratio3)


      i1Shift=ishift(1) ! if equal to 1 to prefer a left shifted stencil when there is a choice
      i2Shift=ishift(2) 
      i3Shift=ishift(3) 

      i1offset =centering  ! offset to shift i1 to be positive so division by ratio always shifts to the left
      if( nra.lt.0 ) i1offset=i1offset+1-nra/ratio  
      i2offset =centering  ! offset to shift i2 
      if( nsa.lt.0 ) i2offset=i2offset+1-nsa/ratio
      i3offset =centering  ! offset to shift i3 
      if( nta.lt.0 ) i3offset=i3offset+1-nta/ratio

      i3=ndfta ! defaults for 1D and 2D
      j3=ndcta
      i2=ndfsa
      j2=ndcsa

#beginMacro InterpolateLoopsMacro()
 if( update.eq.0 )then
   !   Here we set uc(j1,j2,j3,c)=interpolant(uf)
   if( nd.eq.2 )then
    INTERP_LOOPS(2,uc(j1,j2,j3,c)=)
   else if( nd.eq.3 )then
     INTERP_LOOPS(3,uc(j1,j2,j3,c)=)
   else if( nd.eq.1 )then
     INTERP_LOOPS(1,uc(j1,j2,j3,c)=)
   else
     write(*,*) 'interpFineFromCoarse:ERROR:nd=',nd
     stop 1
   end if
 else
   !   Here we set uc(j1,j2,j3,c)=uc(j1,j2,j3,c)+interpolant(uf)
   if( nd.eq.2 )then
    INTERP_LOOPS(2,uc(j1,j2,j3,c)=uc(j1,j2,j3,c)+)
   else if( nd.eq.3 )then
     INTERP_LOOPS(3,uc(j1,j2,j3,c)=uc(j1,j2,j3,c)+)
   else if( nd.eq.1 )then
     INTERP_LOOPS(1,uc(j1,j2,j3,c)=uc(j1,j2,j3,c)+)
   else
     write(*,*) 'interpFineFromCoarse:ERROR:nd=',nd
     stop 1
   end if
 end if
#endMacro

      if( maskOption.eq.doNotUseMask )then

        InterpolateLoopsMacro()

      else if( maskOption.eq.maskGreaterThanZero )then

        ! **** redefine the macro to set the mask ****
#beginMacro beginCheckForMask()
if( mask(j1,j2,j3).gt.0 )then
#endMacro
#beginMacro endCheckForMask()
endif
#endMacro

        InterpolateLoopsMacro()

      else if( maskOption.eq.maskEqualZero )then

        ! **** redefine the macro to set the mask ****
#beginMacro beginCheckForMask()
if( mask(j1,j2,j3).eq.0 )then
#endMacro
#beginMacro endCheckForMask()
endif
#endMacro

        InterpolateLoopsMacro()

      else
         stop 6241
      end if

      return
      end

c c   Here is where we call the main macro
c 
c       INTERP()
c 
c #beginMacro beginCheckForMask()
c if( mask(j1,j2,j3).gt.0 )then  ! fixed to (j1,j2,j3) *wdh* 040504
c #endMacro
c #beginMacro endCheckForMask()
c endif
c #endMacro
c 
c c     Here is the version that checks the mask    
c       INTERP(WithMask)

