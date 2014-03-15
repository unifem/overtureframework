! **************************************************************************************************
!
!   Ogmg optimized boundary conditions for 3D and fourth order accuracy 
!
! wdh 100509
! **************************************************************************************************


! These next include file will define the macros that will define the difference approximations (in op/src)
! Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
#Include "derivMacroDefinitions.h"

! Define 
!    defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
!       defines -> ur2, us2, ux2, uy2, ...            (2D)
!                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
#Include "defineParametricDerivMacros.h"

! 2D, order=6, components=1
! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)

! defineParametricDerivativeMacros(u,dr,dx,2,2,1,2)
defineParametricDerivativeMacros(rsxy,dr,dx,3,2,2,2)
defineParametricDerivativeMacros(rsxy,dr,dx,3,4,2,2)
! defineParametricDerivativeMacros(rsxy,dr,dx,3,6,2,2)

 defineParametricDerivativeMacros(u,dr,dx,3,2,0,3)
 defineParametricDerivativeMacros(u,dr,dx,3,4,0,3)

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
#beginMacro getOp(DERIV, u,uc,uu,aj,ud )

 #If $GRIDTYPE eq "curvilinear" 
  #peval getDuD ## DERIV ## $DIM(uu,aj,ud)  ! Note: The perl variables are evaluated when the macro is USED. 
 #Else
  #peval ud = u ## DERIV ## $ORDER(i1,i2,i3,uc)
 #End

#endMacro

! ==========================================================================================
!  Evaluate the Jacobian and its derivatives (parametric and spatial). 
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalJacobianDerivatives(aj,MAXDER)

#If $GRIDTYPE eq "curvilinear"
 ! this next call will define the jacobian and its derivatives (parameteric and spatial)
 #peval evalJacobianDerivatives(rsxy,i1,i2,i3,aj,$DIM,$ORDER,MAXDER)

#End

#endMacro 

! This next macro will evaluate r,s,t derivatives of the jacobian 
! u = jacobian name (rsxy), v=prefix for derivatives: vrxr, vrys, 
#beginMacro evalParametricJacobianDerivatives(u,i1,i2,i3,v,DIM,ORDER,MAXDERIV)
#If DIM == 2
evalParametricDerivativesComponents2(u,i1,i2,i3,0,0, v ## rx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,0, v ## sx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,0,1, v ## ry,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,1, v ## sy,DIM,ORDER,MAXDERIV)
#End
#If DIM == 3
evalParametricDerivativesComponents2(u,i1,i2,i3,0,0, v ## rx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,0, v ## sx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,2,0, v ## tx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,0,1, v ## ry,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,1, v ## sy,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,2,1, v ## ty,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,0,2, v ## rz,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,2, v ## sz,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,2,2, v ## tz,DIM,ORDER,MAXDERIV)
#End
#endMacro

! ==========================================================================================
!  Evaluate the Jacobian and its parametric derivatives
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalParametricJacobianDerivatives(aj,MAXDER)

#If $GRIDTYPE eq "curvilinear"
 ! this next call will define the jacobian and its derivatives (parameteric and spatial)
 #peval evalParametricJacobianDerivatives(rsxy,i1,i2,i3,aj,$DIM,$ORDER,MAXDER)

#End

#endMacro 



! ==========================================================================================
!  Evaluate the parametric derivatives of u.
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives, e.g. uur, uus, uurr, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalParametricDerivative(u,uc,uu,MAXDER)
#If $GRIDTYPE eq "curvilinear" 
 !peval evalParametricDerivativesComponents1(u,i1,i2,i3,uc, uu,$DIM,$ORDER,MAXDER)
 #peval evalParametricDerivativesComponents0(u,i1,i2,i3, uu,$DIM,$ORDER,MAXDER)
#Else
 uu=u(i1,i2,i3,uc) ! in the rectangular case just eval the solution
#End
#endMacro


#beginMacro loops(expression)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  expression
end do
end do
end do
#endMacro

#beginMacro loops2(e1,e2)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  e1
  e2
end do
end do
end do
#endMacro

! use the mask 
#beginMacro loopsMaskGT(expression)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
    expression
  end if
end do
end do
end do
#endMacro

#beginMacro loops2MaskGT(e1,e2)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
    e1
    e2
  end if
end do
end do
end do
#endMacro


! =====================================================================================
! extrapolate a ghost point
! =====================================================================================
#beginMacro extrapolatePoint()
 if( orderOfExtrapolation.eq.3 )then
   u(i1-is1,i2-is2,i3-is3)=\
    3.*u(i1      ,i2      ,i3      )\
   -3.*u(i1+  is1,i2+  is2,i3+  is3)\
      +u(i1+2*is1,i2+2*is2,i3+2*is3)
 else if( orderOfExtrapolation.eq.4 )then
   u(i1-is1,i2-is2,i3-is3)=\
     4.*u(i1      ,i2      ,i3      )\
    -6.*u(i1+  is1,i2+  is2,i3+  is3)\
    +4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -u(i1+3*is1,i2+3*is2,i3+3*is3)
 else if( orderOfExtrapolation.eq.5 )then
   u(i1-is1,i2-is2,i3-is3)=\
     5.*u(i1      ,i2      ,i3      )\
   -10.*u(i1+  is1,i2+  is2,i3+  is3)\
   +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
    -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
       +u(i1+4*is1,i2+4*is2,i3+4*is3)
 else if( orderOfExtrapolation.eq.6 )then
   u(i1-is1,i2-is2,i3-is3)=\
     6.*u(i1      ,i2      ,i3      )\
   -15.*u(i1+  is1,i2+  is2,i3+  is3)\
   +20.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
   -15.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
    +6.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
       -u(i1+5*is1,i2+5*is2,i3+5*is3)
 else if( orderOfExtrapolation.eq.7 )then
   u(i1-is1,i2-is2,i3-is3)=\
     7.*u(i1      ,i2      ,i3      )\
   -21.*u(i1+  is1,i2+  is2,i3+  is3)\
   +35.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
   -35.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
   +21.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
    -7.*u(i1+5*is1,i2+5*is2,i3+5*is3)\
       +u(i1+6*is1,i2+6*is2,i3+6*is3)
 else if( orderOfExtrapolation.eq.2 )then
   u(i1-is1,i2-is2,i3-is3)=\
    2.*u(i1      ,i2      ,i3      )\
      -u(i1+  is1,i2+  is2,i3+  is3)
 else
   write(*,*) 'bc3dOrder4:ERROR:'
   write(*,*) ' orderOfExtrapolation=',orderOfExtrapolation
   stop 1
 end if
#endMacro
 
#beginMacro extrapolateSecondGhostPoint()
 u(i1-2*is1,i2-2*is2,i3-2*is3)=\
            5.*u(i1-  is1,i2-  is2,i3-  is3)\
          -10.*u(i1      ,i2      ,i3      )\
          +10.*u(i1+  is1,i2+  is2,i3+  is3)\
           -5.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
              +u(i1+3*is1,i2+3*is2,i3+3*is3)
#endMacro




! This next file defines the macro call appearing in the next function
! #Include "neumannEquationBC.h"
#Include "neumannEquationForcing.h"

! #Include "neumannEquationBC.new.h"

! Here is the 3d approximation: 
#Include "neumannEquationBC3d.h"

!*************************** This version is consistent with lineSmoothOpt.bf *******************
! Define the fourth-order accurate neumann BC which uses the normal derivative of the equation on the
! boundary. 
#beginMacro neumannAndEquation(FORCING,DIR,DIM)

 if( equationToSolve.ne.laplaceEquation )then
   write(*,'("Ogmg:bc3dOrder4:ERROR: equation!=laplace")')
   write(*,'("equationToSolve=",i2)') equationToSolve
   write(*,'("gridType=",i2)') gridType
   stop 6064
 end if


 ! for testing:
 ff=1.e8
 g=1.e8
 ffr=1.e8
 ffs=1.e8
 fft=1.e8
 grr=1.e8
 gss=1.e8
 gtt=1.e8

 if( gridType.eq.rectangular )then
   if( a1.eq.0. )then
     write(*,*) 'bc3dOrder4:ERROR: a1=0!'
     stop 2
   end if
   
   ! write(*,'("bc3dOrder4:4th-order neumannAndEqn")') 

   drn=dx(axis)
   nsign = 2*side-1
   br2=-nsign*a0/(a1*nsign)

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
    j3=i3-is3
   do i2=n2a+is2,n2b+is2
    j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1


    ! the rhs for the mixed BC is stored in the ghost point value of 
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,rectangular,DIR,DIM)


    ! u.xx + u.yy + u.zz = f   (1)
    ! a1*nsign*ux + a0*u = g   (2) 
    !  u.xxx = f.x - ( u.xyy + u.xzz ) = f.x - ( g.yy + g.zz - a0*( u.yy+u.zz )/(a1*nsign) )
    !        = f.x - ( g.yy + g.zz - a0*( f - u.xx )/(a1*nsign) )   (3)
    ! Solve (2) and (3) for u(-1) and u(-2)
    #If #DIR eq "R"
      un=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      gb= ffr - (gss +gtt - a0*ff )/(a1*nsign) ! This is u_xxx + a0/(a1*nsign)*( u_xx )

      u(i1-is1,i2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1+is1,i2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*un*drn)/(3.-br2*drn)
      u(i1-2*is1,i2,i3) = u(i1+2*is1,i2,i3) +16*br2*drn/(3.-br2*drn)*u(i1+is1,i2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*un*drn**2*br2+12*un*drn+8*gb*drn**3)/(3.-br2*drn)

    #Elif #DIR eq "S"
      un=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      gb= ffs - (grr +gtt - a0*ff )/(a1*nsign) ! This is u_yyy + a0/(a1*nsign)*( u_yy )

      u(i1,i2-is2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1,i2+is2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*un*drn)/(3.-br2*drn)
      u(i1,i2-2*is2,i3) = u(i1,i2+2*is2,i3) +16*br2*drn/(3.-br2*drn)*u(i1,i2+is2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*un*drn**2*br2+12*un*drn+8*gb*drn**3)/(3.-br2*drn)


    #Elif #DIR eq "T"
      un=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      gb= fft - (grr +gss - a0*ff )/(a1*nsign) ! This is u_zzz + a0/(a1*nsign)*( u_zz )

      u(i1,i2,i3-is3) = (3.+br2*drn)/(3.-br2*drn)*u(i1,i2,i3+is3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*un*drn)/(3.-br2*drn)
      u(i1,i2,i3-2*is3) = u(i1,i2,i3+2*is3) +16*br2*drn/(3.-br2*drn)*u(i1,i2,i3+is3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*un*drn**2*br2+12*un*drn+8*gb*drn**3)/(3.-br2*drn)

      ! write(*,'("bc3dOrder4: g,ff,fft,grr,gss=",5e10.2)') g,ff,fft,grr,gss

    #Else
      stop 8892
    #End
   
!   write(*,'(''bcopt: i1,i2,i3,g,ffr,gss,u,uss ='',3i3,5f11.6)') i1,i2,i3,g,ffr,gss,u(i1,i2,i3),uss
!  write(*,'('' i1,i2,i3,ur,urrr,ffr,gss ='',3i3,4e11.2)') i1,i2,i3,ur,urrr,ffr,gss
!  write(*,'('' i1,i2,i3,f,f,f='',3i3,4e11.2)') i1,i2,i3,f(i1,i2,i3),f(i1+2*is1,i2+2*is2,i3),f(i1+is1,i2+is2,i3)
!      u(j1,j2,j3)=u(i1+is1,i2+is2,i3+is3)
!      u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)

    else if( mask(i1,i2,i3).lt.0 )then
      ! *wdh* 100616 -- extrap ghost outside interp 
     extrapolatePoint()
     extrapolateSecondGhostPoint()
    end if
  end do
  end do
  end do



 else
   ! **** curvilinear case ****

   ! write(*,*) 'bc3dOrder4:4th-order neumann (curvilinear- DIR)'


   nsign = 2*side-1
   drn=dr(axis)
   cf1=3.*nsign
   alpha1=a1*nsign

   ! Assume that a0 is constant for now:
   a0r=0.
   a0s=0.
   a0t=0.
   a0rr=0.
   a0rs=0.
   a0rt=0.
   a0ss=0.
   a0st=0.
   a0tt=0.

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
     j3=i3-is3
   do i2=n2a+is2,n2b+is2
     j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1

    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,curvilinear,DIR,DIM)

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(aj,MAXDER) : MAXDER = max number of derivatives to precompute.
  ! opEvalParametricJacobianDerivatives(aj,2)

  ! We need 2 parameteric and 1 real derivative. Do this for now: 
  opEvalJacobianDerivatives(aj,2)

  ! evaluate forward derivatives of the current solution: 

  ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
  ! MAXDER = max number of parametric derivatives to precompute.
  ! ** opEvalParametricDerivative(u,uc,uu,3)

  ! Evaluate the tangential derivatives (NOTE: These must be consistent with lineSmoothOpt)
  uu = u(i1,i2,i3)
  #If #DIR eq "R"
   uus=us4(i1,i2,i3)
   uuss=uss4(i1,i2,i3)
   uusss=usss2(i1,i2,i3)

   uut=ut4(i1,i2,i3)
   uutt=utt4(i1,i2,i3)
   uuttt=uttt2(i1,i2,i3)

   uust=ust4(i1,i2,i3)

   uusst=usst2(i1,i2,i3)
   uustt=ustt2(i1,i2,i3)
  #Elif #DIR eq "S"
   uur=ur4(i1,i2,i3)
   uurr=urr4(i1,i2,i3)
   uurrr=urrr2(i1,i2,i3)

   uut=ut4(i1,i2,i3)
   uutt=utt4(i1,i2,i3)
   uuttt=uttt2(i1,i2,i3)

   uurt=urt4(i1,i2,i3)

   uurrt=urrt2(i1,i2,i3)
   uurtt=urtt2(i1,i2,i3)
  #Elif #DIR eq "T"
   uur=ur4(i1,i2,i3)
   uurr=urr4(i1,i2,i3)
   uurrr=urrr2(i1,i2,i3)

   uus=us4(i1,i2,i3)
   uuss=uss4(i1,i2,i3)
   uusss=usss2(i1,i2,i3)

   uurs=urs4(i1,i2,i3)

   uurrs=urrs2(i1,i2,i3)
   uurss=urss2(i1,i2,i3)
  #Else
    stop 9966
  #End

  neumannAndEquationBC3dOrder4(DIR)


!   uurst=(gst-a0*uust-a0s*uut-a0t*uus-anS*uusst-anSt*uuss- anSs*uust-anSst*uus-anT*uustt-anTt*uust-anTs*uutt-anTst*uut- anRs*uurt-anRt*uurs-anRst*uur)/anR

 ! gdExact(ntd,nxd,nyd,nzd,x,y,z,n,t,value)
!!$ call gdExact(0,0,0,0,xy(i1-is1,i2-is2,i3-is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2),0,0.,ue0)
!!$ call gdExact(0,0,0,0,xy(i1-2*is1,i2-2*is2,i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,i2-2*is2,i3-2*is3,2),0,0.,ue1)
!!$  if( i2.eq.1 .and. i3.eq.1 )then
!!$    write(*,'(/,"bc3d4:******")')
!!$  end if
!!$  write(*,'("bc3d4: i1,i2,i3=",3i3," u(-1),ue, u(-2),ue =",6f10.4)') i1,i2,i3,u(i1-is1,i2-is2,i3-is3),ue0,u(i1-2*is1,i2-2*is2,i3-2*is3),ue1
!!$  write(*,'("bc3d4: n1a,n1b,n2a,n2b,n3a,n3b =",6i5)') n1a,n1b,n2a,n2b,n3a,n3b
!!$
!!$  ! write(*,'(" f(j1,.,.)=",20f10.4)') ((f(j1,m2,m3),m2=n2a,n2b),m3=n3a,n3b)
!!$  ! write(*,'(" f(j1,i2,i3)=",20f10.4)') f(j1,i2,i3)
!!$  ! write(*,'("bc3d4: j1,i2p1,i3p1=",6i5)') j1,i2p1,i3p1
!!$  ! write(*,'(" f(j1,i2m1,i3m1),f(j1,i2p1,i3m1),f(j1,i2m1,i3p1),f(j1,i2p1,i3p1)=",20f10.4)')f(j1,i2m1,i3m1),f(j1,i2p1,i3m1),f(j1,i2m1,i3p1),f(j1,i2p1,i3p1)
!!$  #If #DIR eq "R"
!!$    write(*,'(" gv(0,-1,-1),...=",20f10.4)') gv(0,-1,-1),gv(0, 1,-1),gv(0,-1, 1),gv(0, 1, 1)
!!$  #End
!!$  #If #DIR eq "S"
!!$    write(*,'(" gv(-1,0,-1),...=",20f10.4)') gv(-1,0,-1),gv(1, 0,-1),gv(-1,0,1),gv(1, 0, 1)
!!$  #End
!!$
!!$  write(*,'(" g,gs,gt,gss,gtt,gst=",20f10.4)') g,gs,gt,gss,gtt,gst
!!$  write(*,'(" uurst : gst,uurst,uur,uus,uut, uuss,uutt,uust,uusst,uustt=",20f10.4)') gst,uurst,uur,uus,uut, uuss,uutt,uust,uusst,uustt
!!$
!!$  write(*,'("  : uur,uurs,uurss,uurt,uurtt,uurst=",6f10.4)') uur,uurs,uurss,uurt,uurtt,uurst
!!$  write(*,'("  : uurr,uurrs,uurrt,unnn2,un4=",6f10.4)') uurr,uurrs,uurrt,unnn2,un4
!!$  write(*,'("  : uu,uur,uus,uurr,uurs,uuss,uurt,uust,uutt=",9f10.4)') uu,uur,uus,uurr,uurs,uuss,uurt,uust,uutt
!!$  write(*,'("  g,gs,gt,gss,gtt,ff,ffs,fft=",12e10.2)') g,gs,gt,gss,gtt,ff,ffs,fft
!!$  write(*,'("     : cRR,cSS,cTT,cRS,cRT,cST,ccR,ccS,ccT,c0=",10f10.4)') cRR,cSS,cTT,cRS,cRT,cST,ccR,ccS,ccT,c0

  ! ******* TEMP *********
  ! u(i1-is1,i2-is2,i3-is3)=ue0
  ! u(i1-2*is1,i2-2*is2,i3-2*is3)=ue1

!!$
!!$  write(*,'("     : ajtxx,ajtyy,ajtzz,ajtxy,ajtxz,ajtyz,ajtx,ajty,ajtz=",10f10.4)') ajtxx,ajtyy,ajtzz,ajtxy,ajtxz,ajtyz,ajtx,ajty,ajtz
!!$
!!$  ! uurrs is bad
!!$  ! uurrs=(ffs-cSS*uusss-cTT*uustt-cRS*uurss-cRT*uurst-cST*uusst-ccR*uurs-ccS*uuss-ccT*uust-c0*uus-cSSs*uuss-cTTs*uutt-cRSs*uurs-cRTs*uurt-cSTs*uust-ccRs*uur-ccSs*uus-ccTs*uut-c0s*uu-cRRs*uurr)/cRR
!!$  write(*,'(" ffs,uusss,uustt,uurss,uurst,uusst,cSSs,cTTs,cRSs,cRTs,cSTs,ccRs,ccSs,ccTs,cRRs : =",15f10.4)') ffs,uusss,uustt,uurss,uurst,uusst,cSSs,cTTs,cRSs,cRTs,cSTs,ccRs,ccSs,ccTs,cRRs
!!$  ! uurst is bad
!!$  !  uurst=(gst-a0*uust-a0s*uut-a0t*uus-anS*uusst-anSt*uuss-anSs*uust-anSst*uus-anT*uustt-anTt*uust-anTs*uutt-anTst*uut-anRs*uurt-anRt*uurs-anRst*uur)/anR
!!$  write(*,'("  anR,gst,a0,a0s,a0t,anS,anSt,anSs,anSst,anT,anTt,anTs,anTst,anRs,anRt,anRst: =",20f10.4)')anR,gst,a0,a0s,a0t,anS,anSt,anSs,anSst,anT,anTt,anTs,anTst,anRs,anRt,anRst
!!$  ! anTst: 
!!$  !  anRst=a1*(n1*ajrxst+n2*ajryst+n3*ajrzst +n1s*ajrxt+n2s*ajryt+n3s*ajrzt +n1t*ajrxs+n2t*ajrys+n3t*ajrzs +n1st*ajrx+n2st*ajry+n3st*ajrz)
!!$  write(*,'("  ajrxst,ajryst,ajrzst,ajrxt,ajryt,ajrzt,n1st,n2st,n3st: =",20f10.4)')ajrxst,ajryst,ajrzst,ajrxt,ajryt,ajrzt,n1st,n2st,n3st
!!$  ! n1st=ajrxst*ani + ajrxt*anis + ajrxs*anit + ajrx*anist
!!$  write(*,'("  n1st: ajrxst,ajrxt,ajrxs,anist =",20f10.4)')ajrxst,ajrxt,ajrxs,anist
!!$
!!$  write(*,'("  cRRr,cSSr,cTTr,cRSr,cRTr,cSTr: =",20e10.2)') cRRr,cSSr,cTTr,cRSr,cRTr,cSTr
!!$  !write(*,'("  ccRr,ccSr,ccTr,c0: =",20e10.2)') ccRr,ccSr,ccTr,c0
!!$  write(*,'("  ccRr,ajrxxr,ajryyr,ajrzzr: =",20e10.2)') ccRr,ajrxxr,ajryyr,ajrzzr
!!$  write(*,'("  ccSr,ajsxxr,ajsyyr,ajszzr: =",20e10.2)') ccSr,ajsxxr,ajsyyr,ajszzr
!!$  write(*,'("  ccTr,ajtxxr,ajtyyr,ajtzzr: =",20e10.2)') ccTs,ajtxxr,ajtyyr,ajtzzr


! *****************
!$$$    ! gdExact(ntd,nxd,nyd,nzd,x,y,z,n,t,value)
!$$$    call gdExact(0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue0)
!$$$    
!$$$    call gdExact(0,0,0,0,xy(i1-2,i2,i3,0),xy(i1-2,i2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1-1,i2,i3,0),xy(i1-1,i2,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1+1,i2,i3,0),xy(i1+1,i2,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1+2,i2,i3,0),xy(i1+2,i2,i3,1),0.,0,0.,ue4)
!$$$
!$$$
!$$$    ure   = (ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(0))
!$$$    urre=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(0)**2)
!$$$    urrre = (-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(0)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-2,i3,0),xy(i1,i2-2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+2,i3,0),xy(i1,i2+2,i3,1),0.,0,0.,ue4)
!$$$
!$$$    use=(ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(1))
!$$$    usse=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(1)**2)
!$$$    ussse=(-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(1)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue4)
!$$$    urse=(ue4-ue3-ue2+ue1)/(4.*dr(0)*dr(1))
! *****************


    else if( mask(i1,i2,i3).lt.0 )then
      ! *wdh* 100616 -- extrap ghost outside interp 
     extrapolatePoint()
     extrapolateSecondGhostPoint()
    end if
  end do
  end do
  end do

 end if
#endMacro

#beginMacro setBogus10(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10)
 x1=bogus
 x2=bogus
 x3=bogus
 x4=bogus
 x5=bogus
 x6=bogus
 x7=bogus
 x8=bogus
 x9=bogus
 x10=bogus
#endMacro
#beginMacro setBogus15(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15)
 x1=bogus
 x2=bogus
 x3=bogus
 x4=bogus
 x5=bogus
 x6=bogus
 x7=bogus
 x8=bogus
 x9=bogus
 x10=bogus
 x11=bogus
 x12=bogus
 x13=bogus
 x14=bogus
 x15=bogus
#endMacro

      subroutine bc3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ndc, c, u,f,mask,rsxy, xy,
     &    bc, boundaryCondition, ipar, rpar )
! ===================================================================================
!  Optimised Boundary conditions.
!
!  useCoefficients: 1=use the c array.
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)


!     --- local variables ----
      
      real ue1,ue2,ue3,ue4,ue5,ue6,ue7,ue8,ue9
      real ue0,use,usse,ussse,urrree
      real ure,urrre,ursse, urre,urrf,urse,usssee,ussf, urrse, urrte, urtte
      real ccTm1,ajtzzm1


      integer is1,is2,is3,orderOfAccuracy,gridType,level,debug,
     &        side,axis,useForcing,bc3dOrder4ion4,solveEquationWithBC
      integer i1m1,i1p1,i2m1,i2p1,i3m1,i3p1
      integer m1a,m1b,m2a,m2b,m3a,m3b,nn, m1,m2,m3
      real dr(0:2), dx(0:2)

      real dxn
      real nsign,cd,cg,cf,ga,gb,gc

      integer useCoefficients,orderOfExtrapolation
      integer dirichlet,neumann,mixed,equation,extrapolation,
     &        combination,equationToSecondOrder,mixedToSecondOrder,
     &        evenSymmetry,oddSymmetry,extrapolateTwoGhostLines
      parameter( 
     &     dirichlet=1,
     &     neumann=2,
     &     mixed=3,
     &     equation=4,
     &     extrapolation=5,
     &     combination=6,
     &     equationToSecondOrder=7,
     &     mixedToSecondOrder=8,
     &     evenSymmetry=9,
     &     oddSymmetry=10,
     &     extrapolateTwoGhostLines=11 )

      integer rectangular,curvilinear
      parameter(
     &     rectangular=0,
     &     curvilinear=1)

       integer equationToSolve
       integer userDefined,laplaceEquation,divScalarGradOperator,
     &  heatEquationOperator,variableHeatEquationOperator,
     &   divScalarGradHeatOperator,secondOrderConstantCoefficients,
     & axisymmetricLaplaceEquation
      parameter(
     & userDefined=0,
     & laplaceEquation=1,
     & divScalarGradOperator=2,              ! div[ s[x] grad ]
     & heatEquationOperator=3,               ! I + c0*Delta
     & variableHeatEquationOperator=4,       ! I + s[x]*Delta
     & divScalarGradHeatOperator=5,  ! I + div[ s[x] grad ]
     & secondOrderConstantCoefficients=6,
     & axisymmetricLaplaceEquation=7 )

      integer dirichletFirstGhostLineBC,neumannFirstGhostLineBC
      integer dirichletSecondGhostLineBC,neumannSecondGhostLineBC
      integer useSymmetry,useEquationToFourthOrder,
     & useEquationToSecondOrder,useExtrapolation
      parameter(
     &  useSymmetry=0,
     &  useEquationToFourthOrder=1,
     &  useEquationToSecondOrder=2,
     &  useExtrapolation=3 )

      integer i1,i2,i3,j1,j2,j3,mGhost,mg1,mg2
      integer m11,m12,m13,m14,m15,
     &        m21,m22,m23,m24,m25,
     &        m31,m32,m33,m34,m35,
     &        m41,m42,m43,m44,m45,
     &        m51,m52,m53,m54,m55
      integer    m111,m211,m311,m411,m511,
     &           m121,m221,m321,m421,m521,
     &           m131,m231,m331,m431,m531,
     &           m141,m241,m341,m441,m541,
     &           m151,m251,m351,m451,m551,
     &           m112,m212,m312,m412,m512,
     &           m122,m222,m322,m422,m522,
     &           m132,m232,m332,m432,m532,
     &           m142,m242,m342,m442,m542,
     &           m152,m252,m352,m452,m552,
     &           m113,m213,m313,m413,m513,
     &           m123,m223,m323,m423,m523,
     &           m133,m233,m333,m433,m533,
     &           m143,m243,m343,m443,m543,
     &           m153,m253,m353,m453,m553,
     &           m114,m214,m314,m414,m514,
     &           m124,m224,m324,m424,m524,
     &           m134,m234,m334,m434,m534,
     &           m144,m244,m344,m444,m544,
     &           m154,m254,m354,m454,m554,
     &           m115,m215,m315,m415,m515,
     &           m125,m225,m325,m425,m525,
     &           m135,m235,m335,m435,m535,
     &           m145,m245,m345,m445,m545,
     &           m155,m255,m355,m455,m555
      real op2d, op3d,op2dSparse4,op3dSparse4, op2d4

!     ...variables for 4th-order Neumann BC    
      real drn
      real cf1,cf2,cg1,cg2
      real us,uss,usss,ur,urr,urrr,urs,urss,urrs

      real rxSq,rxNorm,rxNorms,rxNormss,rxNormr,rxNormrr
      real a1s,a1ss,a2s,a2ss,a1r,a1rr,a2r,a2rr

      real a0,a1,a2,alpha1,alpha2
      real rxi,ryi,sxi,syi,rxr,rxs,sxr,sxs,ryr,rys,syr,sys
!      real rxxi,ryyi,sxxi,syyi
!      real rxrr,rxrs,rxss,ryrr,ryrs,ryss
!      real sxrr,sxrs,sxss,syrr,syrs,syss
!      real rxx,ryy,sxx,syy
!      real rxxr,ryyr,rxxs,ryys, sxxr,syyr,sxxs,syys
      real rxNormI,rxNormIs,rxNormIss,rxNormIr,rxNormIrr
      real sxNormI,sxNormIs,sxNormIss,sxNormIr,sxNormIrr
      real n1,n1r,n1rr, n1s,n1ss, n1t,n1tt, n1rs, n1rt, n1st
      real n2,n2r,n2rr, n2s,n2ss, n2t,n2tt, n2rs, n2rt, n2st 
      real n3,n3r,n3rr, n3s,n3ss, n3t,n3tt, n3rs, n3rt, n3st 
      real an1,an1s,an1ss,an2,an2s,an2ss,an1r,an1rr,an2r,an2rs,an2rr
      real ff,ffs,ffr,g,gs,gss,gr,grr,grs,grt, gt,gst,gtt, fft,ffst,fftt
      real gr0,gr1,gr2, gs0,gs1,gs2, gt0,gt1,gt2
      real c11,c11r,c11s,c12,c12r,c12s,c22,c22r,c22s,c1,c1r,c1s,c2,c2r,c2s
      real b0,b1,b2,b3,bf,br2
      real unnn2,un4,dn,un

      real fv(-1:1,-1:1,-1:1), gv(-1:1,-1:1,-1:1)

!      real rzi,szi,txi,tyi,tzi
!      real rxt, ryt, rzt, sxt, syt, szt
!      real rzr,rzs,rzt, szr,szs,szt, txr,txs,txt, tyr,tys,tyt, tzr, tzs,tzt
!      real rxrt, rxst, rxtt, ryrt, ryst, rytt, rzrr, rzrs, rzrt, rzss, rzst, rztt
!      real sxrt, sxst, sxtt, syrt, syst, sytt, szrr, szrs, szrt, szss, szst, sztt
!      real txrt, txst, txtt, tyrt, tyst, tytt, tzrr, tzrs, tzrt, tzss, tzst, tztt

      integer ax1,ax2
      integer iv(0:2),dv(0:2),mdim(0:1,0:2)

      real cxx,cyy,czz,cxy,cxz,cyz,cx,cy,cz,c0
      real cRR,cSS,cTT,cRS,cRT,cST,ccR,ccS,ccT
      real cRRr,cSSr,cTTr,cRSr,cRTr,cSTr,ccRr,ccSr,ccTr,c0r
      real cRRs,cSSs,cTTs,cRSs,cRTs,cSTs,ccRs,ccSs,ccTs,c0s
      real cRRt,cSSt,cTTt,cRSt,cRTt,cSTt,ccRt,ccSt,ccTt,c0t

      real ani,anir,anis,anit, anirr,anirs, anirt, aniss, anist, anitt
      real anR, anRr,anRs,anRt, anRrr,anRrs, anRrt, anRss, anRst, anRtt
      real anS, anSr,anSs,anSt, anSrr,anSrs, anSrt, anSss, anSst, anStt
      real anT, anTr,anTs,anTt, anTrr,anTrs, anTrt, anTss, anTst, anTtt
      real a0r,a0s,a0t, a0rr,a0ss,a0tt,a0rs,a0rt,a0st
      real bogus

      real ajrxxr
      real ajrxxs
      real ajrxxt
      real ajrxyr
      real ajrxys
      real ajrxyt
      real ajrxzr
      real ajrxzs
      real ajrxzt
      real ajryxr
      real ajryxs
      real ajryxt
      real ajryyr
      real ajryys
      real ajryyt
      real ajryzr
      real ajryzs
      real ajryzt
      real ajrzxr
      real ajrzxs
      real ajrzxt
      real ajrzyr
      real ajrzys
      real ajrzyt
      real ajrzzr
      real ajrzzs
      real ajrzzt
      real ajsxxr
      real ajsxxs
      real ajsxxt
      real ajsxyr
      real ajsxys
      real ajsxyt
      real ajsxzr
      real ajsxzs
      real ajsxzt
      real ajsyxr
      real ajsyxs
      real ajsyxt
      real ajsyyr
      real ajsyys
      real ajsyyt
      real ajsyzr
      real ajsyzs
      real ajsyzt
      real ajszxr
      real ajszxs
      real ajszxt
      real ajszyr
      real ajszys
      real ajszyt
      real ajszzr
      real ajszzs
      real ajszzt
      real ajtxxr
      real ajtxxs
      real ajtxxt
      real ajtxyr
      real ajtxys
      real ajtxyt
      real ajtxzr
      real ajtxzs
      real ajtxzt
      real ajtyxr
      real ajtyxs
      real ajtyxt
      real ajtyyr
      real ajtyys
      real ajtyyt
      real ajtyzr
      real ajtyzs
      real ajtyzt
      real ajtzxr
      real ajtzxs
      real ajtzxt
      real ajtzyr
      real ajtzys
      real ajtzyt
      real ajtzzr
      real ajtzzs
      real ajtzzt


      integer ma,mb,mc
      real ca,cb,cc

!     --- start statement function ----
      integer kd
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real h12,h22,d12,d22

      declareTemporaryVariables(2,2)
      declareParametricDerivativeVariables(uu,3)   ! declare temp variables uu, uur, uus, ...
      declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)


!.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)


      h12(kd) = 1./(2.*dx(kd))
      h22(kd) = 1./(dx(kd)**2)
      d12(kd) = 1./(2.*dr(kd))
      d22(kd) = 1./(dr(kd)**2)

!     The next macro call will define the difference approximation statement functions
!      defineDifferenceOrder2Components0(u,RX)
!      defineDifferenceOrder4Components0(u,RX)

!     --- end statement functions ----

      
      ! for debugging initialize variables to a bogus value --

      bogus=1.e20
      if( .true. )then
       setBogus15(drn,cf1,cf2,cg1,cg2,us,uss,usss,ur,urr,urrr,urs,urss,urrs,urrs)
       setBogus15(rxSq,rxNorm,rxNorms,rxNormss,rxNormr,rxNormrr,a1s,a1ss,a2s,a2ss,a1r,a1rr,a2r,a2rr,a2rr)
 
       setBogus15(a0,a1,a2,alpha1,alpha2,rxi,ryi,sxi,syi,rxr,rxs,sxr,sxs,ryr,rys)
       setBogus15(syr,sys,rxNormI,rxNormIs,rxNormIss,rxNormIr,rxNormIrr,sxNormI,sxNormIs,sxNormIss,sxNormIr,sxNormIrr,syr,sys,rxNormI)
       setBogus10(n1,n1r,n1rr, n1s,n1ss, n1t,n1tt, n1rs, n1rt, n1st)
       setBogus10(n2,n2r,n2rr, n2s,n2ss, n2t,n2tt, n2rs, n2rt, n2st)
       setBogus10(n3,n3r,n3rr, n3s,n3ss, n3t,n3tt, n3rs, n3rt, n3st)
       setBogus10(an1,an1s,an1ss,an2,an2s,an2ss,an1r,an1rr,an2r,an2rs)
       setBogus15(an2rr,ff,ffs,ffr,g,gs,gss,gr,grr,grs,grt, gt,gst,gtt, fft)
       setBogus15(ffst,fftt,gr0,gr1,gr2, gs0,gs1,gs2, gt0,gt1,gt2,gs2, gt0,gt1,gt2)
       setBogus15(c11,c11r,c11s,c12,c12r,c12s,c22,c22r,c22s,c1,c1r,c1s,c2,c2r,c2s)
 
       setBogus10(b0,b1,b2,b3,bf,br2,unnn2,un4,dn,un)
       setBogus10(cxx,cyy,czz,cxy,cxz,cyz,cx,cy,cz,c0)
       setBogus10(cRR,cSS,cTT,cRS,cRT,cST,ccR,ccS,ccT,ccT)
       setBogus10(cRRr,cSSr,cTTr,cRSr,cRTr,cSTr,ccRr,ccSr,ccTr,c0r)
       setBogus10(cRRt,cSSt,cTTt,cRSt,cRTt,cSTt,ccRt,ccSt,ccTt,c0t)
 
       setBogus10(ani,anir,anis,anit, anirr,anirs, anirt, aniss, anist, anitt)
       setBogus10(anR, anRr,anRs,anRt, anRrr,anRrs, anRrt, anRss, anRst, anRtt)
       setBogus10(anS, anSr,anSs,anSt, anSrr,anSrs, anSrt, anSss, anSst, anStt)
       setBogus10(anT, anTr,anTs,anTt, anTrr,anTrs, anTrt, anTss, anTst, anTtt)
 
       ajrxxr=bogus
       ajrxxs=bogus
       ajrxxt=bogus
       ajrxyr=bogus
       ajrxys=bogus
       ajrxyt=bogus
       ajrxzr=bogus
       ajrxzs=bogus
       ajrxzt=bogus
       ajryxr=bogus
       ajryxs=bogus
       ajryxt=bogus
       ajryyr=bogus
       ajryys=bogus
       ajryyt=bogus
       ajryzr=bogus
       ajryzs=bogus
       ajryzt=bogus
       ajrzxr=bogus
       ajrzxs=bogus
       ajrzxt=bogus
       ajrzyr=bogus
       ajrzys=bogus
       ajrzyt=bogus
       ajrzzr=bogus
       ajrzzs=bogus
       ajrzzt=bogus
       ajsxxr=bogus
       ajsxxs=bogus
       ajsxxt=bogus
       ajsxyr=bogus
       ajsxys=bogus
       ajsxyt=bogus
       ajsxzr=bogus
       ajsxzs=bogus
       ajsxzt=bogus
       ajsyxr=bogus
       ajsyxs=bogus
       ajsyxt=bogus
       ajsyyr=bogus
       ajsyys=bogus
       ajsyyt=bogus
       ajsyzr=bogus
       ajsyzs=bogus
       ajsyzt=bogus
       ajszxr=bogus
       ajszxs=bogus
       ajszxt=bogus
       ajszyr=bogus
       ajszys=bogus
       ajszyt=bogus
       ajszzr=bogus
       ajszzs=bogus
       ajszzt=bogus
       ajtxxr=bogus
       ajtxxs=bogus
       ajtxxt=bogus
       ajtxyr=bogus
       ajtxys=bogus
       ajtxyt=bogus
       ajtxzr=bogus
       ajtxzs=bogus
       ajtxzt=bogus
       ajtyxr=bogus
       ajtyxs=bogus
       ajtyxt=bogus
       ajtyyr=bogus
       ajtyys=bogus
       ajtyyt=bogus
       ajtyzr=bogus
       ajtyzs=bogus
       ajtyzt=bogus
       ajtzxr=bogus
       ajtzxs=bogus
       ajtzxt=bogus
       ajtzyr=bogus
       ajtzys=bogus
       ajtzyt=bogus
       ajtzzr=bogus
       ajtzzs=bogus
       ajtzzt=bogus
      end if


      side                 =ipar(0)
      axis                 =ipar(1)
      useCoefficients      =ipar(2)
      orderOfExtrapolation =ipar(3)
      gridType             =ipar(4)
      orderOfAccuracy      =ipar(5)
      useForcing           =ipar(6)
      equationToSolve      =ipar(7)
      bc3dOrder4ion4            =ipar(8)
      solveEquationWithBC  =ipar(9)
      level                =ipar(10)
      debug                =ipar(11)

      dirichletFirstGhostLineBC =ipar(12)
      neumannFirstGhostLineBC   =ipar(13)
      dirichletSecondGhostLineBC=ipar(14)
      neumannSecondGhostLineBC  =ipar(15)

      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      a0                   =rpar(3)
      a1                   =rpar(4)
      dr(0)                =rpar(5)
      dr(1)                =rpar(6)
      dr(2)                =rpar(7)
      ! signForJacobian      =rpar(8)

      if( debug.gt.7 )then
        write(*,'(" bc3dOrder4: bc=",i2," level=",i1," order=",i1," n1bc=",i2," n2bc=",i2)') 
     & bc,level,orderOfAccuracy,
     & neumannFirstGhostLineBC,neumannSecondGhostLineBC
      end if


      if( orderOfAccuracy.ne.4 )then
        write(*,'(" bc3dOrder4: ERROR: order is not equal to 4!")')
        stop 4444
      end if

      if( nd.ne.3 )then
        write(*,'(" bc3dOrder4: ERROR: nd is not equal to 3!")')
        stop 4445
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
        stop 4446
      end if

      if( orderOfACcuracy.eq.4 )then
        m11=1     
        m21=2     
        m31=3     
        m41=4     
        m51=5     
        m12=6     
        m22=7     
        m32=8     
        m42=9     
        m52=10    
        m13=11    
        m23=12    
        m33=13    
        m43=14    
        m53=15    
        m14=16    
        m24=17    
        m34=18    
        m44=19    
        m54=20    
        m15=21    
        m25=22    
        m35=23    
        m45=24    
        m55=25   
      end if


      if( bc.eq.equation .and. orderOfAccuracy.eq.4 )then

        ! *************************************************************
        ! *********  Fourth-order accurate Neumann or mixed BC's ******
        ! *************************************************************
        ! write(*,*) 'bc3dOrder4:equation4, bc3dOrder4ion4=',bc3dOrder4ion4

        if( neumannSecondGhostLineBC.eq.useEquationToSecondOrder )then
          if( debug.gt.15 )then
            write(*,'("  bc3dOrder4:order4:l=",i2," neumann-AndEqn...")') level
          end if
          ! write(*,*) 'bc3dOrder4:NE n1a,n1b,n2a,n2b=',n1a,n1b,n2a,n2b

          ! define m1a,m1b,.. to equal n1a,n1b,.. except for periodic directions
          m1a=n1a
          m1b=n1b
          m2a=n2a
          m2b=n2b
          m3a=n3a
          m3b=n3b
          if( boundaryCondition(0,0).lt.0 )then
            m1a=m1a-1
            m1b=m1b+1
          end if
          if( boundaryCondition(0,1).lt.0 )then
            m2a=m2a-1
            m2b=m2b+1
          end if
          if( boundaryCondition(0,2).lt.0 )then
            m3a=m3a-1
            m3b=m3b+1
          end if
#perl $DIM=3; $ORDER=4; $GRIDTYPE="curvilinear";

          if( axis.eq.0 .and. nd.eq.3 )then
            neumannAndEquation(forcing,R,3)
          else if( axis.eq.1 .and. nd.eq.3 )then
            neumannAndEquation(forcing,S,3)
          else if( axis.eq.2 .and. nd.eq.3 )then
            neumannAndEquation(forcing,T,3)
          else
            stop 4410
          end if

        else 
          write(*,*) 'bc3dOrder4:order4:ERROR:neumannSecondGhostLineBC=',neumannSecondGhostLineBC
          stop 7711

        end if

      else
        write(*,*) 'bc3dOrder4:ERROR: bc,orderOfAccuracy=',bc,orderOfAccuracy
        stop 44443
      end if


      return
      end


