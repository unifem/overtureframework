c =================================================================================================
c
c  Project the values at interpolation points to satisfy the divergence constraint 
c               div( eps*E) = rho
c
c =================================================================================================

c These next include file will define the macros that will define the difference approximations (in op/src)
c Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
#Include "derivMacroDefinitions.h"

c Define 
c    defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
c       defines -> ur2, us2, ux2, uy2, ...            (2D)
c                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
#Include "defineParametricDerivMacros.h"

! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
! 2D, order=6, components=1
! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)

defineParametricDerivativeMacros(u,dr,dx,3,2,1,2)
defineParametricDerivativeMacros(u,dr,dx,3,4,1,2)


! ==========================================================================================
!  Evaluate the Jacobian and its derivatives (parametric and spatial). 
!    rsxy   : jacobian matrix name 
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalJacobianDerivatives(rsxy,i1,i2,i3,aj,MAXDER)

#If $GRIDTYPE eq "curvilinear"
 ! this next call will define the jacobian and its derivatives (parameteric and spatial)
 #peval evalJacobianDerivatives(rsxy,i1,i2,i3,aj,$DIM,$ORDER,MAXDER)

#End

#endMacro 

! ==========================================================================================
!  Evaluate the parametric derivatives of u.
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives, e.g. uur, uus, uurr, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalParametricDerivative(u,i1,i2,i3,uc,uu,MAXDER)
#If $GRIDTYPE eq "curvilinear" 
 #peval evalParametricDerivativesComponents1(u,i1,i2,i3,uc, uu,$DIM,$ORDER,MAXDER)
#End
#endMacro

! ==========================================================================================
!  Evaluate a derivative. (assumes parametric derivatives have already been evaluated)
!   DERIV   : name of the derivative. One of 
!                x,y,z,xx,xy,xz,...
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives (same name used with opEvalParametricDerivative) 
!    aj     : prefix for the name of the jacobian variables.
!    ud     : derivative is assigned to this variable.
! ==========================================================================================
#beginMacro getOp(DERIV, u,i1,i2,i3,uc,uu,aj,ud )

 #If $GRIDTYPE eq "curvilinear" 
  #peval getDuD ## DERIV ## $DIM(uu,aj,ud)  ! Note: The perl variables are evaluated when the macro is USED. 
 #Else
  #peval ud = u ## DERIV ## $ORDER(i1,i2,i3,uc)
 #End

#endMacro

! ******************************************************************************
!   This macro will evaluate the divergence
! ******************************************************************************
#beginMacro evalDiv(rsxy,aj,u,i1,i2,i3,div)
 opEvalParametricDerivative(u,i1,i2,i3,ex,uu,1)    ! computes uu1r, uu1s 
 opEvalParametricDerivative(u,i1,i2,i3,ey,vv,1)    ! computes vv1r, vv1s 
 getOp(x ,u,i1,i2,i3,ex,uu,aj,u ## x)             ! u.x
 getOp(y ,u,i1,i2,i3,ey,vv,aj,v ## y)             ! v.y
 div = ux+ vy
#endMacro


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

#beginMacro beginLoopsWithMask()
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
#endMacro


! =============================================================================================
!   Macro to evaluate the divergence
! =============================================================================================
#beginMacro getDivergence(i1,i2,i3,div)
  opEvalJacobianDerivatives(rsxy,i1,i2,i3,aj,0)
  evalDiv(rsxy,aj,u,i1,i2,i3,div)
#endMacro

! =============================================================================================
!   Macro to pproject the divergence at discretization points next to interpolation points.
!   Input:
!     $DIM, $ORDER, $GRIDTYPE (perl variables)
! =============================================================================================
#beginMacro projectDivergence()
 beginLoops()
   if( mask(i1,i2,i3).lt.0 )then

     ! interp. pt. found -- find a nearby discretization pt

     ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2

     #If $ORDER eq 2
       #defineMacro MASKCHECK() (mask(j1,j2,j3).gt.0)  
     #Else
       ! only change interp points in the 2nd layer
       #defineMacro MASKCHECK() ( j1.ge.n1a .and. j1.le.n1b .and. j2.ge.n2a .and. j2.le.n2b .and. mask(j1,j2,j3).gt.0 .and. mask((i1+j1)/2,(i2+j2)/2,(i3+j3)/2).lt.0 )
     #End

     ! *********** fix me: define bounds in terms of n1a,... ********
     do j3=i3-w3,i3+w3
     do j2=i2-w2,i2+w2,w2
     do j1=i1-w1,i1+w1,w1
     #If $DIM eq 2
      if( iabs(j1-i1)+iabs(j2-i2).eq.numGhost .and. MASKCHECK() )then
     #Else
      if( iabs(j1-i1)+iabs(j2-i2)+iabs(j3-i3).eq.numGhost .and. MASKCHECK() )then
     #End


       ! Enforce div(E)=0 at point jv by adjusting point iv

       getDivergence(j1,j2,j3,res)

       !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
       !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
       ! ux = ajrx*uur+ajsx*uus
       ! vy = ajry*vvr+ajsy*vvs

       ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
       #If $GRIDTYPE eq "rectangular"

        #If $ORDER eq 2
         ax = (i1-j1)/(2.*dx(0))
         ay = (i2-j2)/(2.*dx(1))
         #If $DIM eq 3
          az = (i3-j3)/(2.*dx(2))
         #End
        #Elif $ORDER eq 4
         !ax = (i1-j1)*8./(12.*dx(0))  ! first ghost line coeff
         !ay = (i2-j2)*8./(12.*dx(1))
         ax = -(i1-j1)*.5/(12.*dx(0))
         ay = -(i2-j2)*.5/(12.*dx(1))
         #If $DIM eq 3
          !az = (i3-j3)*8./(12.*dx(2))
          az = -(i3-j3)*.5/(12.*dx(2))
         #End
        #Else
          stop 7755
        #End

       #Elif $GRIDTYPE eq "curvilinear"

        #If $ORDER eq 2
         ar = (i1-j1)/(2.*dr(0))
         as = (i2-j2)/(2.*dr(1))
         #If $DIM eq 3
          at = (i3-j3)/(2.*dr(2))
         #End
        #Elif $ORDER eq 4
         !ar = (i1-j1)*8./(12.*dr(0))
         !as = (i2-j2)*8./(12.*dr(1))
         ar = -(i1-j1)*.5/(12.*dr(0))
         as = -(i2-j2)*.5/(12.*dr(1))
         #If $DIM eq 3
          !at = (i3-j3)*8./(12.*dr(2))
          at = -(i3-j3)*.5/(12.*dr(2))
         #End
        #Else
          stop 7755
        #End

        #If $DIM eq 2
         ax = ajrx*ar+ajsx*as
         ay = ajry*ar+ajsy*as
        #Elif $DIM eq 3 
         ax = ajrx*ar+ajsx*as+ajtx*at
         ay = ajry*ar+ajsy*as+ajty*at
         az = ajrz*ar+ajsz*as+ajtz*at
        #Else
          ERROR
        #End
       #Else
         error
       #End

       #If $DIM eq 2
         aSq = ax**2 + ay**2
       #Else
         aSq = ax**2 + ay**2 + az**2
       #End

       ! adjust the interpolation pt: 
       u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
       u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq   
       #If $DIM eq 3
        u(i1,i2,i3,ez) = u(i1,i2,i3,ez) - res*az/aSq   
       #End

       ! check ...
       if( .true. .or. debug.gt.2 )then 
         getDivergence(j1,j2,j3,res)
         write(*,'(" projectInterp: interp: grid=",i4," i1,i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,i2,j1,j2,res
       end if

     end if
     end do
     end do
     end do

   end if
 endLoops()
#endMacro

      subroutine mxProjectInterp( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                        gridIndexRange, u, mask, rsxy, xy, boundaryCondition, ipar, rpar, ierr )
c =================================================================================================
c  Project the values at interpolation points to satisfy the divergence constraint 
c               div( eps*E) = rho
c 
c   u (input) : solution to be projected 
c 
c =================================================================================================


      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
              n1a,n1b,n2a,n2b,n3a,n3b, ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer gridIndexRange(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

c     ... local
      integer option,ex,ey,ez,hx,hy,hz,e1,e2,e3,debug,myid,orderOfAccuracy
      real omega,dt,t
      integer side,axis,axisp1,axisp2,i1,i2,i3,j1,j2,j3,k1,k2,k3,is1,is2,is3,grid,gridType
      integer numGhost,numGhost3,w1,w2,w3
      real res,ax,ay,az,aSq,ar,as,at,dx(0:2),dr(0:2)
      logical found

      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)

      ! for new evaluation method:
      real ux,uy,uz,uxx,uxy,uyy,uxz,uyz,uzz
      real vx,vy,vz,vxx,vxy,vyy,vxz,vyz,vzz
      real wx,wy,wz,wxx,wxy,wyy,wxz,wyz,wzz

      declareTemporaryVariables(2,2)
      declareParametricDerivativeVariables(uu,3)   ! declare temp variables uu, uur, uus, ...
      declareParametricDerivativeVariables(vv,3)   ! declare temp variables uu, uur, uus, ...
      declareParametricDerivativeVariables(ww,3)   ! declare temp variables uu, uur, uus, ...
      declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)


      ierr=0


      option = ipar(0)
      ex     = ipar(1)
      ey     = ipar(2)
      ez     = ipar(3) 
      hx     = ipar(4)
      hy     = ipar(5)
      hz     = ipar(6)
      debug  = ipar(7)
      myid   = ipar(8)
      orderOfAccuracy=ipar(9)
      grid   = ipar(10)
      gridType=ipar(11)

      omega = rpar(0)  ! for potential future use
      dt    = rpar(1)
      t     = rpar(2)
      dx(0) = rpar(3)
      dx(1) = rpar(4)
      dx(2) = rpar(5)
      dr(0) = rpar(6)
      dr(1) = rpar(7)
      dr(2) = rpar(8)

      e1 = ex
      e2 = ey
      e3 = e2 + 1 ! hz or ez

      ! we just need to include 1 ghost point since we change only the interpolation point
      ! next to the interior point. 
      ! numGhost=1   ! orderOfAccuracy/2

      numGhost=orderOfAccuracy/2

      numGhost3=numGhost
      w1=numGhost
      w2=numGhost
      w3=numGhost
      if( nd.eq.2 )then
        numGhost3=0
        w3=0
      end if

      !  *** add ghost so we find interp pts **
      n1a=gridIndexRange(0,0)-numGhost
      n1b=gridIndexRange(1,0)+numGhost
      n2a=gridIndexRange(0,1)-numGhost
      n2b=gridIndexRange(1,1)+numGhost
      n3a=gridIndexRange(0,2)-numGhost3
      n3b=gridIndexRange(1,2)+numGhost3

      if( .false. .or. (t.le. dt .and. myid.eq.0) )then
        write(*,'(" mxProjectInterp: init : t,dt = ",2(e10.2,1x))') t,dt
        if( .true. .or. debug.gt.3 )then
          write(*,'(" grid=",i4," orderOfAccuracy=",i4,", debug=",i6)') grid,orderOfAccuracy,debug
          write(*,'(" gridIndexRange= ",3(2i5,1x))') gridIndexRange
          write(*,'(" mxProjectInterp: ex,ey,hz = ",3i2)') ex,ey,hz
          write(*,'(" mxProjectInterp: n1a,n1b,n2a,n2b,n3a,n3b = ",3(2i5,1x))') n1a,n1b,n2a,n2b,n3a,n3b
        end if
      end if

! #defineMacro divr(i1,i2,i3) ( (u(i1+1,i2,i3,ex)-u(i1-1,i2,i3,ex))/(2.*dx(0)) +\
!                               (u(i1,i2+1,i3,ey)-u(i1,i2-1,i3,ey))/(2.*dx(1)) )

      if( nd.eq.2 .and. gridType.eq.rectangular .and. orderOfAccuracy.eq.2 )then

        #perl $DIM=2; $GRIDTYPE="rectangular"; $ORDER=2;
        projectDivergence();

      else if( nd.eq.2 .and. gridType.eq.curvilinear .and. orderOfAccuracy.eq.2 )then

        #perl $DIM=2; $GRIDTYPE="curvilinear"; $ORDER=2;
        projectDivergence();

      else if( nd.eq.2 .and. gridType.eq.rectangular .and. orderOfAccuracy.eq.4 )then

        #perl $DIM=2; $GRIDTYPE="rectangular"; $ORDER=4;
        projectDivergence();

      else if( nd.eq.2 .and. gridType.eq.curvilinear .and. orderOfAccuracy.eq.4 )then

        #perl $DIM=2; $GRIDTYPE="curvilinear"; $ORDER=4;
        projectDivergence();

      else if( nd.eq.3 .and. gridType.eq.rectangular .and. orderOfAccuracy.eq.2 )then

        #perl $DIM=3; $GRIDTYPE="rectangular"; $ORDER=2;
        projectDivergence();

      else if( nd.eq.3 .and. gridType.eq.curvilinear .and. orderOfAccuracy.eq.2 )then

        #perl $DIM=3; $GRIDTYPE="curvilinear"; $ORDER=2;
        projectDivergence();

      else if( nd.eq.3 .and. gridType.eq.rectangular .and. orderOfAccuracy.eq.4 )then

        #perl $DIM=3; $GRIDTYPE="rectangular"; $ORDER=4;
        projectDivergence();

      else if( nd.eq.3 .and. gridType.eq.curvilinear .and. orderOfAccuracy.eq.4 )then

        #perl $DIM=3; $GRIDTYPE="curvilinear"; $ORDER=4;
        projectDivergence();

      else

        write(*,'("mxProjectInterp:ERROR: not implemented for nd,gridType,orderOfAccuracy=",3i3)') nd,gridType,orderOfAccuracy

        stop 12763

      end if


      return 
      end
