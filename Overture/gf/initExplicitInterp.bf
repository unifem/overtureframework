
#beginMacro computeFullWeights(DIMENSION)
   #If #DIMENSION == "2"
     do m2=0,width-1
     do m1=0,width-1
       coeff(i,m1,m2,0)=qr(m1)*qs(m2)
     end do 
     end do
   #Else
     do m3=0,width-1
     do m2=0,width-1
     do m1=0,width-1
       coeff(i,m1,m2,m3)=qr(m1)*qs(m2)*qt(m3)
     end do 
     end do
     end do
   #End 
#endMacro

#Include "initExplicitInterpMacros.h"


#beginMacro computeCoefficients(DIMENSION)

  if( width.eq.3 )then
    computeCoeff3(DIMENSION) 
  else if( width.eq.2 )then
    computeCoeff2(DIMENSION) 
  else if( width.eq.5 )then
    computeCoeff5(DIMENSION) 
  else if( width.eq.7 )then
    computeCoeff7(DIMENSION) 
  else if( width.eq.9 )then
    computeCoeff9(DIMENSION) 
  else if( width.eq.1 )then
    computeCoeff1(DIMENSION) 
  else if( width.eq.4 )then
    computeCoeff4(DIMENSION) 
  else if( width.eq.6 )then
    computeCoeff6(DIMENSION) 
  else if( width.eq.8 )then
    computeCoeff8(DIMENSION) 
  end if

#endMacro



      subroutine initExplicitInterp(ndc1,ndc2,ndc3,ndci,\
          ipar, coeff,ci,pr,ps,pt,gridSpacing,indexStart,\
          variableInterpolationWidth,interpoleeLocation,interpoleeGrid)
c =====================================================================================
c
c   Initialize explicit interpolation
c
c   pr(0:ni-1),ps(0:ni-1),pt(0:ni-1) : temporary storage space needed for 1d, 2d, 3d.
c
c  Return 
c     ipar(6)=useVariableWidthInterpolation    
c
c =====================================================================================
      implicit none

      integer ndc1,ndc2,ndc3,ndci
      real coeff(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real ci(0:ndci-1,0:*)
      real pr(0:*),ps(0:*),pt(0:*)
      real gridSpacing(0:2,0:*)
      integer indexStart(0:2,0:*)
c     integer width(0:*)
      integer variableInterpolationWidth(0:*)
      integer interpoleeLocation(0:ndci-1,0:*)
      integer interpoleeGrid(0:*)
      integer ipar(0:*)

c ------ local variables 
      integer nd,ni,grid,gridi,indexPosition,isCellCentered
      integer m1,m2,m3,ia,ib,i,width,storageOption,useVariableWidthInterpolation,maxWidth
      real ccShift,relativeOffset,x,pri,psi,pti
      real qr(0:20),qs(0:20),qt(0:20)  ! assumes a max interpolation width of 20 *********

      integer precomputeAllCoefficients, precomputeSomeCoefficients, precomputeNoCoefficients     
      parameter( precomputeAllCoefficients=0, precomputeSomeCoefficients=1, precomputeNoCoefficients=2 )
      integer ii

c ---- start statement functions
      #Include "lagrangePolynomials.h"
c ---- end statement functions


      nd            =ipar(0)
      grid          =ipar(1)
      ni            =ipar(2)
      isCellCentered=ipar(3)
      storageOption =ipar(4)
      maxWidth      =ipar(5)

      useVariableWidthInterpolation=0 ! set to 1 if the interpolation width is not constant

      if( isCellCentered.eq.1 ) then
        ccShift=-.5
      else
        ccShift=0.
      end if

       if( nd.eq.2 )then

        ! write(*,*) 'initExplicitInterp:grid,ni=',grid,ni
        i=0
        do while( i.lt.ni )

         ia=i
         width=variableInterpolationWidth(i)
         do while( i.lt.ni .and. variableInterpolationWidth(i).eq.width )
  	  gridi = interpoleeGrid(i)   ! *** could vectorize this loop since list is sorted by interpolee

          pr(i)=ci(i,0)/gridSpacing(0,gridi)+indexStart(0,gridi)-interpoleeLocation(i,0)+ccShift
          ps(i)=ci(i,1)/gridSpacing(1,gridi)+indexStart(1,gridi)-interpoleeLocation(i,1)+ccShift

          ! write(*,*) 'i,gridi,pr,ps=',i,gridi,pr(i),ps(i)

          i=i+1
         end do 
         ib=i-1
       
         if( width.ne.maxWidth )then ! we need to use variable interpolation widths in this case
           useVariableWidthInterpolation=1
         end if

         ! compute coefficients for the point i=ia...ib
         computeCoefficients(2) 
         i=ib+1

       end do

      else if( nd.eq.3 )then

c        do i=0,ni-1
c  	  gridi = interpoleeGrid(i)   ! *** could vectorize this loop since list is sorted by interpolee
c          pr(i)=ci(i,0)/gridSpacing(0,gridi)+indexStart(0,gridi)-interpoleeLocation(i,0)+ccShift
c          ps(i)=ci(i,1)/gridSpacing(1,gridi)+indexStart(1,gridi)-interpoleeLocation(i,1)+ccShift
c          pt(i)=ci(i,2)/gridSpacing(2,gridi)+indexStart(2,gridi)-interpoleeLocation(i,2)+ccShift
c        end do 
c        ia=0
c        ib=ni-1
c        width=variableInterpolationWidth(0)
c        computeCoefficients(3) 

        if( .true. )then
        i=0
        do while( i.lt.ni )

         ia=i
         width=variableInterpolationWidth(i)
         do while( i.lt.ni .and. variableInterpolationWidth(i).eq.width )
  	  gridi = interpoleeGrid(i)   ! *** could vectorize this loop since list is sorted by interpolee

          pr(i)=ci(i,0)/gridSpacing(0,gridi)+indexStart(0,gridi)-interpoleeLocation(i,0)+ccShift
          ps(i)=ci(i,1)/gridSpacing(1,gridi)+indexStart(1,gridi)-interpoleeLocation(i,1)+ccShift
          pt(i)=ci(i,2)/gridSpacing(2,gridi)+indexStart(2,gridi)-interpoleeLocation(i,2)+ccShift
          i=i+1
         end do 
         ib=i-1
       
         if( width.ne.maxWidth )then ! we need to use variable interpolation widths in this case
           useVariableWidthInterpolation=1
         end if

         ! compute coefficients for the point i=ia...ib
         ! write(*,*) '*** width=',width
         computeCoefficients(3) 
         i=ib+1

        end do
       end if

      else  ! 1D

        i=0
        do while( i.lt.ni )

         ia=i
         width=variableInterpolationWidth(i)
         do while( i.lt.ni .and. variableInterpolationWidth(i).eq.width )
  	  gridi = interpoleeGrid(i)   ! *** could vectorize this loop since list is sorted by interpolee
          pr(i)=ci(i,0)/gridSpacing(0,gridi)+indexStart(0,gridi)-interpoleeLocation(i,0)+ccShift
          i=i+1
         end do 
         ib=i-1

         if( width.ne.maxWidth )then ! we need to use variable interpolation widths in this case
           useVariableWidthInterpolation=1
         end if
       
         ! compute coefficients for the point i=ia...ib
         computeCoefficients(1) 
         i=ib+1

       end do
      end if

      ipar(6)=useVariableWidthInterpolation

      return 
      end
