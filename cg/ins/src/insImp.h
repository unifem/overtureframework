! ****************************************************************************************************
!   Define macros that are used to build the full implicit matrices for the INS, VP etc. equations
!
!  This file contains generic macros that are used by the different PDEs
!
!  Used by 
!     insImpINS.bf
!     insImpVP.bf 
!     insImpBL.bf 
! ****************************************************************************************************


c --- See --- op/fortranCoeff/opcoeff.bf 
c             op/include/defineConservative.h
c --- See --- mx/src/interfaceMacros.bf <-- mixes different orders of accuracy 

c -- define bpp macros for coefficient operators (from op/src/stencilCoeff.maple)
#Include opStencilCoeffOrder2.h
#Include opStencilCoeffOrder4.h
#Include opStencilCoeffOrder6.h
! #Include opStencilCoeffOrder8.h


c These next include file will define the macros that will define the difference approximations (in op/src)
c Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
#Include "derivMacroDefinitions.h"

c Define 
c    defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
c       defines -> ur2, us2, ux2, uy2, ...            (2D)
c                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
#Include "defineParametricDerivMacros.h"

! 2D, order=6, components=1
! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)

! defineParametricDerivativeMacros(u,dr,dx,2,2,1,2)
 defineParametricDerivativeMacros(rsxy,dr,dx,3,2,2,2)
 defineParametricDerivativeMacros(rsxy,dr,dx,3,4,2,2)
 defineParametricDerivativeMacros(rsxy,dr,dx,3,6,2,2)

 defineParametricDerivativeMacros(u,dr,dx,3,2,1,2)
 defineParametricDerivativeMacros(ul,dr,dx,3,2,1,2)


! Example to define orders 2,4,6: 
! defineParametricDerivativeMacros(u1,dr1,dx1,2,2,1,6)
! defineParametricDerivativeMacros(u1,dr1,dx1,2,4,1,4)
! defineParametricDerivativeMacros(u1,dr1,dx1,2,6,1,2)


! define macros for conservative operators: (in op/src)
!  defines getConservativeCoeff( OPERATOR,s,coeff ), OPERATOR=divScalarGrad, ...
#Include "conservativeCoefficientMacros.h"

c -- From opcoeff.bf

#beginMacro beginLoops()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoops()
  end if
 end do
 end do
 end do
#endMacro

#beginMacro beginLoopsNoMask()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
#endMacro

#beginMacro endLoopsNoMask()
 end do
 end do
 end do
#endMacro

! This loop will check for mask>0 and mask != interiorBoundaryPoint
#beginMacro beginLoopsMixedBoundary()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  ! if( btest(mask(i1,i2,i3),28) )then
  !   write(*,'("+++ Point i=(",3i5,") is an interiorBoundaryPoint")') i1,i2,i3
  ! end if
  if( mask(i1,i2,i3).gt.0 .and. .not.btest(mask(i1,i2,i3),28) )then
#endMacro

#beginMacro beginMatrixLoops(m1,m2,m3)
 do m3=-halfWidth3,halfWidth3
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
#endMacro

#beginMacro endMatrixLoops()
 end do
 end do
 end do
#endMacro

! ======================================================================================================
! Add the local matrix operator opCoeff to the global matrix coeff in equation "e" and component "c"
! ======================================================================================================
#beginMacro addCoeff(c,e,coeff,opCoeff)
#If $DIM == 2 
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
  coeff(mce2(m1,m2,0,c,e),i1,i2,i3)=coeff(mce2(m1,m2,0,c,e),i1,i2,i3)+(opCoeff(ma2(m1,m2,0)))
 end do
 end do
#Else
 do m3=-halfWidth,halfWidth
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
  coeff(mce3(m1,m2,m3,c,e),i1,i2,i3)=coeff(mce3(m1,m2,m3,c,e),i1,i2,i3)+(opCoeff(ma3(m1,m2,m3)))
 end do
 end do
 end do
#End
#endMacro


! ======================================================================================================
! Assign and add up to 10 different local matrix operators to the global matrix coeff 
! in equation "e" and component "c"
! 
! (Leave final unused arguments empty)
! ======================================================================================================
#beginMacro setCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,op10)
 do m3=-halfWidth3,halfWidth3
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
#If #op10 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))+(op10(MA(m1,m2,m3)))
#Elif #op9 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))
#Elif #op8 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))
#Elif #op7 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))
#Elif #op6 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))
#Elif #op5 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))
#Elif #op4 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))
#Elif #op3 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))
#Elif #op2 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))
#Else
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=(op1(MA(m1,m2,m3)))
#End
 end do
 end do
 end do
#endMacro

#beginMacro setCoeff9(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9)
setCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,) 
#endMacro
#beginMacro setCoeff8(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8)
setCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,,) 
#endMacro
#beginMacro setCoeff7(c,e,coeff,op1,op2,op3,op4,op5,op6,op7)
setCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,,,) 
#endMacro
#beginMacro setCoeff6(c,e,coeff,op1,op2,op3,op4,op5,op6)
setCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,,,,) 
#endMacro
#beginMacro setCoeff5(c,e,coeff,op1,op2,op3,op4,op5)
setCoeff10(c,e,coeff,op1,op2,op3,op4,op5,,,,,) 
#endMacro
#beginMacro setCoeff4(c,e,coeff,op1,op2,op3,op4)
setCoeff10(c,e,coeff,op1,op2,op3,op4,,,,,,) 
#endMacro
#beginMacro setCoeff3(c,e,coeff,op1,op2,op3)
setCoeff10(c,e,coeff,op1,op2,op3,,,,,,,) 
#endMacro
#beginMacro setCoeff2(c,e,coeff,op1,op2)
setCoeff10(c,e,coeff,op1,op2,,,,,,,,) 
#endMacro
#beginMacro setCoeff1(c,e,coeff,op1)
setCoeff10(c,e,coeff,op1,,,,,,,,,) 
#endMacro


! ======================================================================================================
! Add up to 10 different local matrix operators to the global matrix coeff 
! in equation "e" and component "c"
! 
! (Leave final unused arguments empty)
! ======================================================================================================
#beginMacro addCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,op10)
 do m3=-halfWidth3,halfWidth3
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
#If #op10 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))+(op10(MA(m1,m2,m3)))
#Elif #op9 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))
#Elif #op8 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))
#Elif #op7 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))
#Elif #op6 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))
#Elif #op5 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))
#Elif #op4 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))
#Elif #op3 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))
#Elif #op2 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))
#Else
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)=coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)+(op1(MA(m1,m2,m3)))
#End
 end do
 end do
 end do
#endMacro

#beginMacro addCoeff9(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9)
addCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,) 
#endMacro
#beginMacro addCoeff8(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8)
addCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,,) 
#endMacro
#beginMacro addCoeff7(c,e,coeff,op1,op2,op3,op4,op5,op6,op7)
addCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,,,) 
#endMacro
#beginMacro addCoeff6(c,e,coeff,op1,op2,op3,op4,op5,op6)
addCoeff10(c,e,coeff,op1,op2,op3,op4,op5,op6,,,,) 
#endMacro
#beginMacro addCoeff5(c,e,coeff,op1,op2,op3,op4,op5)
addCoeff10(c,e,coeff,op1,op2,op3,op4,op5,,,,,) 
#endMacro
#beginMacro addCoeff4(c,e,coeff,op1,op2,op3,op4)
addCoeff10(c,e,coeff,op1,op2,op3,op4,,,,,,) 
#endMacro
#beginMacro addCoeff3(c,e,coeff,op1,op2,op3)
addCoeff10(c,e,coeff,op1,op2,op3,,,,,,,) 
#endMacro
#beginMacro addCoeff2(c,e,coeff,op1,op2)
addCoeff10(c,e,coeff,op1,op2,,,,,,,,) 
#endMacro
#beginMacro addCoeff1(c,e,coeff,op1)
addCoeff10(c,e,coeff,op1,,,,,,,,,) 
#endMacro





! ======================================================================================================
! Set the local matrix operator opCoeff to the global matrix coeff in equation "e" and component "c"
! ======================================================================================================
#beginMacro setCoeff(c,e,coeff,opCoeff)
#If $DIM == 2 
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
  coeff(mce2(m1,m2,0,c,e),i1,i2,i3)=opCoeff(ma2(m1,m2,0))
 end do
 end do
#Else
 do m3=-halfWidth,halfWidth
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
  coeff(mce3(m1,m2,m3,c,e),i1,i2,i3)=opCoeff(ma3(m1,m2,m3))
 end do
 end do
 end do
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

! ==========================================================================================
!  Evaluate the parametric derivatives of u.
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives, e.g. uur, uus, uurr, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalParametricDerivative(u,uc,uu,MAXDER)
#If $GRIDTYPE eq "curvilinear" 
 #peval evalParametricDerivativesComponents1(u,i1,i2,i3,uc, uu,$DIM,$ORDER,MAXDER)
#Else
 uu=u(i1,i2,i3,uc) ! in the rectangular case just eval the solution
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
#beginMacro getOp(DERIV, u,uc,uu,aj,ud )

 #If $GRIDTYPE eq "curvilinear" 
  #peval getDuD ## DERIV ## $DIM(uu,aj,ud)  ! Note: The perl variables are evaluated when the macro is USED. 
 #Else
  #peval ud = u ## DERIV ## $ORDER(i1,i2,i3,uc)
 #End

#endMacro


! ==========================================================================================
!  Form the local coefficient matrix for an operator. (assumes parametric derivatives have already been evaluated)
!   DERIV   : name of the operator. One of 
!                x,y,z,xx,xy,xz,...
!                laplacian, rr,ss,tt, rrrr,ssss,tttt, 
!                r2Dissipation=rr+ss[+tt]
!                r4Dissipation=rrrr+ssss[+tttt]
!    coeff  : fill in this (local) coefficient matrix
!    AJ     : prefix for the name of the jacobian variables.
! ==========================================================================================
#beginMacro getCoeff( DERIV, coeff, aj )

 #If $GRIDTYPE eq "curvilinear" || #DERIV eq "identity" || #DERIV eq "r2Dissipation" || #DERIV eq "r4Dissipation" 
  #peval DERIV ## CoeffOrder$ORDER\Dim$DIM(coeff,aj)    ! trouble if ## appears after a perl variable
 #Else
  #peval DERIV ## CoeffOrder$ORDER\Dim$DIM\Rectangular(coeff,aj)
 #End

#endMacro



! ==============================================================================================================
!   Fill in the coefficients for a derivative
! ==============================================================================================================
#beginMacro fillCoeff(DERIV,coeff,c,e)
beginLoops()

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,1)

  ! evaluate the coeff operators 

  getCoeff(DERIV, xCoeff,aj)

  addCoeff(c,e,coeff,xCoeff)

endLoops()
#endMacro

! ==============================================================================================================
!   Fill in the coefficients for a derivative *CONSERVATIVE*
!     DERIV : divScalarGrad
! ==============================================================================================================
#beginMacro fillCoeffConservative(DERIV,s,coeff,c,e)
beginLoops()

  getConservativeCoeff( DERIV,s,xCoeff )

  addCoeff(c,e,coeff,xCoeff)

endLoops()
#endMacro




!=======================================================================================
! /Description: Return the equation number for given indices
!  /n (input): component number ( n=0,1,..,numberOfComponents-1 )
!  /i1,i2,i3 (input): grid indices
! /return value : The equation number.
!\end{SparseRepInclude.tex}
!=======================================================================================
#defineMacro indexToEquation( n,i1,i2,i3 ) (n+1+ \
 numberOfComponentsForCoefficients*(i1-equationNumberBase1+\
             equationNumberLength1*(i2-equationNumberBase2+\
             equationNumberLength2*(i3-equationNumberBase3))) + equationOffset)


!===============================================================================================
! /Description:
!     Assign row and column numbers to entries in a sparse matrix.
!    This routine is normally only used for assign equation numbers on CompositeGrids
!  when the equationNumber belongs to a point on a different MappedGrid.
!  Rows and columns in the sparse matrix are numbered according to the values of
!             (n,I1,I2,I3)
!  where n is the component number and (I1,I2,I3) are the coordinate indicies on the grid.
!  The component number n runs from 0 to the numberOfComponentsForCoefficients-1 and is used
!  when solving a system of equations.
!
! /m (input): assign row/column values for the m''th entry in the sparse matrix
! /na,I1a,I2a,I3a (input): defines the row(s)
! /equationNumber (input): defines an equation number
!
!\end{SparseRepInclude.tex}
!===============================================================================================
#beginMacro setEquationNumber(m, ni,i1,i2,i3,  nj,j1,j2,j3 )
 equationNumber(m,i1,i2,i3)=indexToEquation( nj,j1,j2,j3)
#endMacro



!===============================================================================================
! /Description:
!   Specify the classification for a set of Index values
!\end{SparseRepInclude.tex}
!===============================================================================================
#beginMacro setClassify(n,i1,i2,i3, type)
 classify(i1,i2,i3,n)=type
#endMacro 

! =======================================================================
!  Macro to zero out the matrix coefficients for equations e1,e1+1,..,e2
! =======================================================================
#beginMacro zeroMatrixCoefficients( coeff,e1,e2, i1,i2,i3 )
do m=ce(0,e1),ce(0,e2+1)-1
 coeff(m,i1,i2,i3)=0.
end do
#endMacro

! ===============================================================================================
!  Add an extrapolation equation to the matrix
!  Macro args:
!   coeff : coefficient matrix to fill in.
!   c,e : fill in equation e, extrapolate component c
!   i1,i2,i3 : marks the boundary point, ghost points will be assigned to assign
!   orderOfExtrap : order of extrapolation
!   ghost : point line to extrapolate (ghost=1,2,..)
! ===============================================================================================
#beginMacro fillMatrixExtrapolation(coeff, c,e,i1,i2,i3,orderOfExtrap,ghost)
 if( orderOfExtrap.lt.1 .or. orderOfExtrap.gt.maxOrderOfExtrapolation )then
   stop 7734
 end if
 i1m=i1-is1*(ghost)  ! ghost point
 i2m=i2-is2*(ghost)
 i3m=i3-is3*(ghost)
 do m=0,orderOfExtrap
  j1=i1m+is1*m  ! m-th point moving inward from the ghost point (i1,i2,i3)
  j2=i2m+is2*m
  j3=i3m+is3*m
  mm = ce(c,e)+m
  coeff(mm,i1m,i2m,i3m)=extrapCoeff(m,orderOfExtrap) ! m=0,1,2,..

  setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 )  !macro to set equationNumber
 end do
 setClassify(e,i1m,i2m,i3m, extrapolation)            !macro to set classify
#endMacro


! ===============================================================================================
!  Add a Neumann or mixed BC to the matrix
!       a0*I + a1*D_n 
!  Macro args:
!   coeff : coefficient matrix to fill in.
!   c,e : fill in equation e, extrapolate component c
!   i1,i2,i3 : boundary point (will assign equations on the ghost point)
!   an(0:2) : holds the outward normal vector 
!   a0,a1 : coefficients of the mixed BC
! ===============================================================================================
#beginMacro fillMatrixNeumann(coeff, c,e, i1,i2,i3, an, a0,a1 )

 ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
 ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
 opEvalJacobianDerivatives(aj,0)

 ! evaluate the coeff operators 
 ! getCoeff(identity, iCoeff,aj)
 getCoeff(x, xCoeff,aj)
 getCoeff(y, yCoeff,aj)
 getCoeff(identity, iCoeff,aj)
 #If $DIM == 3
  getCoeff(z, zCoeff,aj)
 #End

 i1m=i1-is1  ! ghost point
 i2m=i2-is2
 i3m=i3-is3

 beginMatrixLoops(m1,m2,m3)
  m=MA(m1,m2,m3)
  mm=MCE(m1,m2,m3,c,e)

  #If $DIM == 2
   coeff(mm,i1m,i2m,i3m)=a1*(an(0)*xCoeff(m)+an(1)*yCoeff(m))+a0*iCoeff(m)
  #Else
   coeff(mm,i1m,i2m,i3m)=a1*(an(0)*xCoeff(m)+an(1)*yCoeff(m)+an(2)*zCoeff(m))\
                        +a0*iCoeff(m)
  #End

  ! The equation for pt (e,i1m,i2m,i3m) is centered on (c,i1,i2,i3): 
  setEquationNumber(mm, e,i1m,i2m,i3m,  c,i1+m1,i2+m2,i3+m3 )  !macro to set equationNumber

 endMatrixLoops()
 setClassify(e,i1m,i2m,i3m, ghost1)            !macro to set classify

#endMacro

! ===============================================================================================
!  Add a Vector Symmetry BC to the matrix
!  Macro args:
!   coeff : coefficient matrix to fill in.
!   cmpu,eqnu : fill in equations eqnu,...,eqnu+nd-1 and components cmpu,...,cmpu+nd-1
!   i1,i2,i3 : boundary point, will assign ghost point
! NOTES:
! The vector symmetry condition is the normal component is even, the tangential components are odd functions:
!     E1:    n.u(-1) = - n.u(1)
!     E2:    t.u(-1) = t.u(1)        
!        or -> [ u(-1) - (n.u(-1))n ] = [ u(1) - (n.u(1))n ]
! Combine equtions:
!         [n.u(-1) + n.u(1)]n + [ u(-1) - (n.u(-1))n ] - [ u(1) - (n.u(1))n ] = 0 
! or
!         u(-1) - u(1) + 2*(n.u(1))n = 0 
! ===============================================================================================
#beginMacro fillMatrixVectorSymmetry(coeff, cmpu,eqnu, i1,i2,i3, an )

 ! write(*,'(" VS: i1,i2=",2i2," normal=",2f5.2)') i1,i2,an(0),an(1)

 i1m=i1-is1  ! ghost point
 i2m=i2-is2
 i3m=i3-is3

 do m=0,ndc-1
  coeff(m,i1m,i2m,i3m)=0.  ! init all elements to zero
 end do
 mv(0)=0
 mv(1)=0
 mv(2)=0
 do e=eqnu,eqnu+nd-1
  c=cmpu+e-eqnu

  mv(axis)=2*side-1  
  mm=MCE(mv(0),mv(1),mv(2),c,e)  ! matrix index for ghost pt
  coeff(mm,i1m,i2m,i3m)=1.  ! coeff of ghost point
  setEquationNumber(mm, e,i1m,i2m,i3m,  c,i1+mv(0),i2+mv(1),i3+mv(2) )  !macro to set equationNumber

  mv(axis)=-(2*side-1)
  mm=MCE(mv(0),mv(1),mv(2),c,e)  ! matrix index for first pt inside
  coeff(mm,i1m,i2m,i3m)=-1. 
  setEquationNumber(mm, e,i1m,i2m,i3m,  c,i1+mv(0),i2+mv(1),i3+mv(2) )  !macro to set equationNumber

  ! now add on the term: 2*(n.u(1))n 
  do c=cmpu,cmpu+nd-1
   mm=MCE(mv(0),mv(1),mv(2),c,e)  ! matrix index for first pt inside
   ! write(*,'(" VS: mm,i1,i2,e,c=",5i2," n,n=",2f5.2)') mm,i1,i2,e,c,an(c-cmpu),an(e-eqnu)
   coeff(mm,i1m,i2m,i3m)=coeff(mm,i1m,i2m,i3m) + 2.*an(c-cmpu)*an(e-eqnu)
   setEquationNumber(mm, e,i1m,i2m,i3m,  c,i1+mv(0),i2+mv(1),i3+mv(2) )  !macro to set equationNumber
  end do

  setClassify(e,i1m,i2m,i3m, ghost1)            !macro to set classify
 end do 
#endMacro

! ===============================================================================================
!  Return the normal vector (an(0),an(1),an(2)) for a point (i1,i2,i3) on a face (side,axis)
!  This macro does nothing on Cartesian grids. 
! ===============================================================================================
#beginMacro getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
  #If $GRIDTYPE == "curvilinear" 
    ! get the outward normal for curvilinear grids
    an(0)=rsxy(i1,i2,i3,axis,0)
    an(1)=rsxy(i1,i2,i3,axis,1)
    #If $DIM == 2
      anNorm = (2*side-1)/max( epsX, sqrt( an(0)**2 + an(1)**2 ) )
      an(0)=an(0)*anNorm
      an(1)=an(1)*anNorm
    #Else
      an(2)=rsxy(i1,i2,i3,axis,2)
      anNorm = (2*side-1)/max( epsX, sqrt( an(0)**2 + an(1)**2 + an(2)**2 ) )
      an(0)=an(0)*anNorm
      an(1)=an(1)*anNorm
      an(2)=an(2)*anNorm
    #End
  #End
#endMacro


! *************************************************************************************


! ==============================================================================================================
!   Fill in the coefficients for an equation
! ==============================================================================================================
#beginMacro fillCoeff()
#If $DIM == 2 
 #defineMacro MCE(m1,m2,m3,c,e) mce2(m1,m2,m3,c,e) 
 #defineMacro MA(m1,m2,m3) ma2(m1,m2,m3) 
#Else
 #defineMacro MCE(m1,m2,m3,c,e) mce3(m1,m2,m3,c,e) 
 #defineMacro MA(m1,m2,m3) ma3(m1,m2,m3) 
#End

 ! the next macro is defined in insImpINS.bf, or insImpVP.bf, etc. 
 fillCoeffPDE()

#endMacro

! ======================================================================
! Macro to fill in the matrix with BC-s 
!  OR evaluate the residual on the boundary 
! 
! Implicit parameters: 
!    $DIM : 2 or 3 
!    $GRIDTYPE : "rectangular" or "curvilinear"
! ======================================================================
#beginMacro fillMatrixBoundaryConditions()
if( fillCoefficients.eq.1 .or. evalResidualForBoundaryConditions.eq.1 )then  

#If $DIM == 2 
 #defineMacro MCE(m1,m2,m3,c,e) mce2(m1,m2,m3,c,e) 
 #defineMacro MA(m1,m2,m3) ma2(m1,m2,m3) 
#Else
 #defineMacro MCE(m1,m2,m3,c,e) mce3(m1,m2,m3,c,e) 
 #defineMacro MA(m1,m2,m3) ma3(m1,m2,m3) 
#End

 indexRange(0,0)=n1a
 indexRange(1,0)=n1b
 indexRange(0,1)=n2a
 indexRange(1,1)=n2b
 indexRange(0,2)=n3a
 indexRange(1,2)=n3b

 do axis=0,nd-1
 do side=0,1
  is1=0
  is2=0
  is3=0
  if( axis.eq.0 )then
    is1=1-2*side
    n1a=indexRange(side,axis)
    n1b=n1a
  else if( axis.eq.1 )then
    is2=1-2*side
    n2a=indexRange(side,axis)
    n2b=n2a
  else
    is3=1-2*side
    n3a=indexRange(side,axis)
    n3b=n3a
  end if

  #If $GRIDTYPE == "rectangular" 
    ! define the outward normal for Cartesian grids
    an(0)=0.
    an(1)=0.
    an(2)=0.
    an(axis)=2*side-1
  #End

  ! the next macro is defined in insImpINS.bf, or insImpVP.bf, etc. 
  fillMatrixBoundaryConditionsPDE()
  getBoundaryResidualPDE()


!*   if( side.eq.0 .and. axis.eq.0 )then
!*    fillMatrixBoundaryConditionsOnAFaceINS(normal00)
!*    getBoundaryResidualINS(normal00)
!*   else if( side.eq.1 .and. axis.eq.0 )then    
!*    fillMatrixBoundaryConditionsOnAFaceINS(normal10)
!*    getBoundaryResidualINS(normal10)
!*   else if( side.eq.0 .and. axis.eq.1 )then    
!*    fillMatrixBoundaryConditionsOnAFaceINS(normal01)
!*    getBoundaryResidualINS(normal10)
!*   else if( side.eq.1 .and. axis.eq.1 )then    
!*    fillMatrixBoundaryConditionsOnAFaceINS(normal11)
!*    getBoundaryResidualINS(normal11)
!*   else if( side.eq.0 .and. axis.eq.2 )then    
!*    fillMatrixBoundaryConditionsOnAFaceINS(normal02)
!*    getBoundaryResidualINS(normal02)
!*   else if( side.eq.1 .and. axis.eq.2 )then    
!*    fillMatrixBoundaryConditionsOnAFaceINS(normal12)
!*    getBoundaryResidualINS(normal12)
!*  else
!* !    stop 2077
!*  end if

  ! reset values
  if( axis.eq.0 )then
    n1a=indexRange(0,axis)
    n1b=indexRange(1,axis)
  else if( axis.eq.1 )then
    n2a=indexRange(0,axis)
    n2b=indexRange(1,axis)
  else
    n3a=indexRange(0,axis)
    n3b=indexRange(1,axis)
  end if

 end do ! side
 end do ! axis

end if
#endMacro

! ===================================================================================
! Macro to evaluate the RHS
! ===================================================================================
#beginMacro assignRHS(PDE)
  ! the next macro is defined in insImpINS.bf, or insImpVP.bf, etc. 
  assignRHSPDE()
#endMacro


! ==============================================================================================================
!  Compute the coefficient of the artificial dissipation
! ==============================================================================================================
#beginMacro getArtificialDissipationCoeff(adCoeff, u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z )

 #If $DIM == 2
  adCoeff = ad21 + cd22*( abs(u0x)+abs(u0y) + abs(v0x)+abs(v0y) )
 #Else
  adCoeff = ad21 + cd22*( abs(u0x)+abs(u0y)+abs(u0z) + abs(v0x)+abs(v0y)+abs(v0z) + abs(w0x)+abs(w0y)+abs(w0z) )
 #End

#endMacro 

! Second order dissipation operators: 
#defineMacro uDiss22(u,uc) (u(i1-1,i2,i3,uc)+u(i1,i2-1,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2+1,i3,uc)-4.*u(i1,i2,i3,uc))
#defineMacro uDiss23(u,uc) (u(i1-1,i2,i3,uc)+u(i1,i2-1,i3,uc)+u(i1,i2,i3-1,uc)+u(i1+1,i2,i3,uc)+u(i1,i2+1,i3,uc)+u(i1,i2,i3+1,uc)-6.*u(i1,i2,i3,uc))


#beginMacro INSIMP(NAME,PDE)
      subroutine NAME(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                      mask,xy,rsxy,radiusInverse,  u, ndc, coeff, fe,fi,ul, gv,gvl,dw, ndMatProp,matIndex,matValpc,matVal, bc, \
                      boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData,\
                      nde, equationNumber, classify, \
                      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b, \
                      ipar, rpar, pdb, ierr )
c======================================================================
c 
c             Incompressible Navier Stokes IMPlicit 
c             -------------------------------------
c
c    1. Build the coefficient matrix for implicit methods
c    2. Evaluate the right-hand-side and residual 
c
c nd : number of space dimensions
c nd1a,nd1b,nd2a,nd2b,nd3a,nd3b : array dimensions
c
c mask : 
c xy : 
c rsxy : 
c coeff(m,i1,i2,i3) : array holding the matrix coefficients
c u : holds the current solution, used to form the coeff matrix.
c fe : holds the explicit part when evaluating the RHS
c fi : holds the implicit part when evaluating the RHS
c ul : holds the linearized solution, used when evaluating the linearized operator and RHS
c gv : gridVelocity for moving grids
c dw : distance to the wall for some turbulence models
c 
c======================================================================
      implicit none
      integer nd, ndc, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      integer nde,nr1a,nr1b,nr2a,nr2b,nr3a,nr3b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real coeff(0:ndc-1,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real fe(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real fi(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real ul(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real gvl(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),boundaryCondition(0:1,0:2),indexRange(0:1,0:2),ierr

      integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
      real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

      integer equationNumber(0:nde-1,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer classify(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

      integer ipar(0:*)
      real rpar(0:*)
      
      double precision pdb  ! pointer to data base

      ! -- arrays for variable material properties --
      integer constantMaterialProperties
      integer piecewiseConstantMaterialProperties
      integer variableMaterialProperties
      parameter( constantMaterialProperties=0,\
                 piecewiseConstantMaterialProperties=1,\
                 variableMaterialProperties=2 )
      integer materialFormat,ndMatProp
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

c     ---- local variables -----
      integer c,e,i1,i2,i3,m1,m2,m3,j1,j2,j3,ghostLine,n,i1m,i2m,i3m,i1p,i2p,i3p,ndu
      integer side,axis,is1,is2,is3,mm,eqnTemp,debug,ntdc
      integer kd,kd3,orderOfAccuracy,gridIsMoving,orderOfExtrap,orderOfExtrapolation,orderOfExtrapolationForOutflow
      integer numberOfComponentsForCoefficients,stencilSize
      integer gridIsImplicit,implicitOption,implicitMethod,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD,fillCoefficientsScalarSystem
      integer pc,uc,vc,wc,sc,nc,kc,ec,tc,qc,vsc,rc,grid,m,advectPassiveScalar
      real nu,dt,nuPassiveScalar,adcPassiveScalar
      real gravity(0:2), thermalExpansivity, adcBoussinesq,kThermal
      real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc,adCoeff,adCoeffl
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real yy,yEps, epsX
      real an(0:2),anNorm, advectionCoefficient
      integer checkForInflowAtOutFlow, outflowOption
      integer ok,getInt,getReal
      integer mv(0:2) 
      integer fillCoefficients,evalRightHandSide,evalResidual,evalResidualForBoundaryConditions

      real ugv,vgv,wgv, ugvl,vgvl,wgvl

      integer equationNumberBase1,equationNumberLength1,equationNumberBase2,equationNumberLength2,\
              equationNumberBase3,equationNumberLength3,equationOffset

      integer gridType
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      ! *new* 
      real nuDt,aDt,bDt,nuImp,aExp,aImp,bImp,tImp,kDt,teDt
      real ulterm,vlterm,wlterm,qlterm
      real implicitFactor
      integer nonlinearTermsAreImplicit,evalLinearizedDerivatives

      integer implicitVariation
      integer implicitViscous, implicitAdvectionAndViscous, implicitFullLinearized
      parameter( implicitViscous=0, 
     &           implicitAdvectionAndViscous=1, 
     &           implicitFullLinearized=2 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

      integer \
           noSlipWall, \
           inflowWithVelocityGiven, \
           slipWall, \
           outflow, \
           convectiveOutflow, \
           tractionFree, \
           inflowWithPandTV, \
           dirichletBoundaryCondition, \
           symmetry, \
           axisymmetric, \
           interfaceBoundaryCondition,\
           freeSurfaceBoundaryCondition,\
           neumannBoundaryCondition
      parameter( noSlipWall=1,inflowWithVelocityGiven=2,slipWall=4, \
       outflow=5,convectiveOutflow=14,tractionFree=15, \
       inflowWithPandTV=3, \
        dirichletBoundaryCondition=12, \
        symmetry=11,axisymmetric=13,interfaceBoundaryCondition=17,\
	freeSurfaceBoundaryCondition=31, neumannBoundaryCondition=101 )

      !   classifyTypes from SparseRep.h   

      integer interior,boundary,ghost1,ghost2,ghost3,ghost4,interpolation,periodic,extrapolation,unused
      parameter(interior=1,\
                boundary=2,\
                ghost1=3,\
                ghost2=4,\
                ghost3=5,\
                ghost4=6,\
                interpolation=-1,\
                periodic=-2,\
                extrapolation=-3,\
                unused=0 )

      ! the following define which component to fill for a scalar coeff problem
      integer fillCoeffU,fillCoeffV,fillCoeffW,fillCoeffT
      parameter( fillCoeffU=1,fillCoeffV=2,fillCoeffW=3,fillCoeffT=4 )

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real aj
      real dr(0:2), dx(0:2)

      integer ma2(-5:5,-5:5,0:0), ma3(-5:5,-5:5,-5:5)
      real nDotL(0:2)
      real delta(0:5,0:5)
      integer ncc,width,halfWidth,halfWidth3

      integer maxWidth,maxWidthDim
      parameter( maxWidth=5,maxWidthDim=maxWidth*maxWidth*maxWidth )
      real iCoeff(0:maxWidthDim-1)
      real xCoeff(0:maxWidthDim-1)
      real yCoeff(0:maxWidthDim-1)
      real zCoeff(0:maxWidthDim-1)
      real lapCoeff(0:maxWidthDim-1)
      real divSGradCoeff(0:maxWidthDim-1)
      real dissCoeff(0:maxWidthDim-1)

      real p0x,p0y,p0z
      real ulx,uly,ulz, vlx,vly,vlz, wlx,wly,wlz, plx,ply,plz, qlx,qly,qlz
      real u0x,u0y,u0z,u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
      real v0x,v0y,v0z,v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
      real w0x,w0y,w0z,w0xx,w0xy,w0xz,w0yy,w0yz,w0zz
      real q0x,q0y,q0z,q0xx,q0xy,q0xz,q0yy,q0yz,q0zz
      integer eqn,eqnu,eqnv,eqnw,eqnq,eqnk,eqne
      integer cmp,cmpu,cmpv,cmpw,cmpq,cmpk,cmpe
      integer mce2,mce3,ce
      ! temp variables for coeff macros: 
      real cur,cus,cut,curr,curs,curt,cuss,cust,cutt

      ! --- visco plastic variables ---
      real dr0i,dr1i,dr0dr1,dr0dr2,dr1dr2
      real divNuGradu,divNuGradv,divNuGradw,divNuGradul,divNuGradvl,divNuGradwl
      real dxvsqi(0:2),dtImp    

      ! This include file (created in insImpINS.bf or insImpVP, ...) declares variables needed by each version
      declareInsImpTemporaryVariables()

      integer maxOrderOfExtrapolation
      parameter( maxOrderOfExtrapolation=9 )
      real extrapCoeff(0:maxOrderOfExtrapolation,1:maxOrderOfExtrapolation)

      integer numberOfComponents
      real mixedRHS,mixedCoeff,mixedNormalCoeff,a0,a1

      real rhopc,rhov,   Cppc, Cpv, thermalKpc, thermalKv, Kx, Ky, Kz, Kr, Ks, Kt

      !     --- begin statement functions
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

      ! c=component, e=equation
      ! ncc = number of components for coefficients 
      mce2(m1,m2,m3,c,e) = ma2(m1,m2,m3) + stencilSize*(c+ncc*e)
      mce3(m1,m2,m3,c,e) = ma3(m1,m2,m3) + stencilSize*(c+ncc*e)
      ce(c,e) = stencilSize*(c+ncc*e)

      ! statement functions to access coefficients of mixed-boundary conditions
      mixedRHS(c,side,axis,grid)         =bcData(c+numberOfComponents*(0),side,axis,grid)
      mixedCoeff(c,side,axis,grid)       =bcData(c+numberOfComponents*(1),side,axis,grid)
      mixedNormalCoeff(c,side,axis,grid) =bcData(c+numberOfComponents*(2),side,axis,grid)


      ! -- statement functions for variable material properties
      ! (rho,Cp,k) for materialFormat=piecewiseConstantMaterialProperties
      rhopc(i1,i2,i3)      = matValpc( 0, matIndex(i1,i2,i3))
      Cppc(i1,i2,i3)       = matValpc( 1, matIndex(i1,i2,i3))
      thermalKpc(i1,i2,i3) = matValpc( 2, matIndex(i1,i2,i3))
     
      ! (rho,Cp,k) for materialFormat=variableMaterialProperties
      rhov(i1,i2,i3)      = matVal(i1,i2,i3,0)
      Cpv(i1,i2,i3)       = matVal(i1,i2,i3,1)
      thermalKv(i1,i2,i3) = matVal(i1,i2,i3,2)

      !     --- end statement functions

      data extrapCoeff/                                         \
                       1.,-1.,0.,0.,0.,0.,0.,0.,0.,0.,          \
                       1.,-2.,1.,0.,0.,0.,0.,0.,0.,0.,          \
                       1.,-3.,3.,-1.,0.,0.,0.,0.,0.,0.,         \
                       1.,-4.,6.,-4.,1.,0.,0.,0.,0.,0.,         \
                       1.,-5.,10.,-10.,5.,-1.,0.,0.,0.,0.,      \
                       1.,-6.,15.,-20.,15.,-6.,1.,0.,0.,0.,	\
                       1.,-7.,21.,-35.,35.,-21.,7.,-1.,0.,0.,	\
                       1.,-8.,28.,-56.,70.,-56.,28.,-8.,1.,0.,	\
		       1.,-9.,36.,-84.,126.,-126.,84.,-36.,9.,-1./
      ierr=0
      ! write(*,'("Inside insdt: gridType=",i2)') gridType

      n1a                =ipar(0)
      n1b                =ipar(1)
      n2a                =ipar(2)
      n2b                =ipar(3)
      n3a                =ipar(4)
      n3b                =ipar(5)

      pc                 =ipar(6)
      uc                 =ipar(7)
      vc                 =ipar(8)
      wc                 =ipar(9)
      nc                 =ipar(10)
      sc                 =ipar(11)
      tc                 =ipar(12)
      grid               =ipar(13)
      orderOfAccuracy    =ipar(14)
      gridIsMoving       =ipar(15) ! *************

      implicitVariation  =ipar(16) ! **new**

      fillCoefficients   =ipar(17) ! new 
      evalRightHandSide  =ipar(18) ! new

      gridIsImplicit     =ipar(19)
      implicitMethod     =ipar(20)
      implicitOption     =ipar(21)
      isAxisymmetric     =ipar(22)
      use2ndOrderAD      =ipar(23)
      use4thOrderAD      =ipar(24)
      advectPassiveScalar=ipar(25)
      gridType           =ipar(26)
      turbulenceModel    =ipar(27)
      pdeModel           =ipar(28)  
      numberOfComponentsForCoefficients =ipar(29) ! number of components for coefficients
      stencilSize        =ipar(30)

      equationOffset        = ipar(31)
      equationNumberBase1   = ipar(32)
      equationNumberLength1 = ipar(33)
      equationNumberBase2   = ipar(34)
      equationNumberLength2 = ipar(35)
      equationNumberBase3   = ipar(36)
      equationNumberLength3 = ipar(37)

      indexRange(0,0)    =ipar(38)
      indexRange(1,0)    =ipar(39)
      indexRange(0,1)    =ipar(40)
      indexRange(1,1)    =ipar(41)
      indexRange(0,2)    =ipar(42)
      indexRange(1,2)    =ipar(43)

      orderOfExtrapolation=ipar(44)
      orderOfExtrapolationForOutflow=ipar(45)

      evalResidual      = ipar(46)
      evalResidualForBoundaryConditions=ipar(47)
      debug             = ipar(48)
      numberOfComponents= ipar(49)

      rc                =ipar(50)
      materialFormat    =ipar(51)

      dr(0)             =rpar(0)
      dr(1)             =rpar(1)
      dr(2)             =rpar(2)
      dx(0)             =rpar(3)
      dx(1)             =rpar(4)
      dx(2)             =rpar(5)

      dt                =rpar(6) ! **new**
      implicitFactor    =rpar(7) ! **new**

      nu                =rpar(8)
      ad21              =rpar(9)
      ad22              =rpar(10)
      ad41              =rpar(11)
      ad42              =rpar(12)
      nuPassiveScalar   =rpar(13)
      adcPassiveScalar  =rpar(14)
      ad21n             =rpar(15)
      ad22n             =rpar(16)
      ad41n             =rpar(17)
      ad42n             =rpar(18)
      yEps              =rpar(19) ! for axisymmetric

      gravity(0)        =rpar(20)
      gravity(1)        =rpar(21)
      gravity(2)        =rpar(22)
      thermalExpansivity=rpar(23)
      adcBoussinesq     =rpar(24) ! coefficient of artificial diffusion for Boussinesq T equation 
      kThermal          =rpar(25)

      epsX = 1.e-30  ! epsilon used to avoid division by zero in the normal computation -- should be REAL_MIN*100 ??

      ncc=numberOfComponentsForCoefficients ! number of components for coefficients

      ok = getInt(pdb,'checkForInflowAtOutFlow',checkForInflowAtOutFlow) 
      if( ok.eq.0 )then
        write(*,'("*** NAME:ERROR: checkForInflowAtOutFlow NOT FOUND")') 
      else
        if( debug.gt.4 )then
         write(*,'("*** NAME:checkForInflowAtOutFlow=",i6)') checkForInflowAtOutFlow
        end if
      end if
      ok = getInt(pdb,'outflowOption',outflowOption) 
      if( ok.eq.0 )then
        write(*,'("*** NAME:ERROR: outflowOption NOT FOUND")') 
      else
        if( debug.gt.4 )then
         write(*,'("*** NAME:outflowOption=",i6)') outflowOption
        end if
      end if

      ok = getReal(pdb,'advectionCoefficient',advectionCoefficient) 
      if( ok.eq.0 )then
        write(*,'("*** NAME:ERROR: advectionCoefficient NOT FOUND")') 
      else
        if( debug.gt.4 )then
         write(*,'("*** NAME:advectionCoefficient=",f5.2)') advectionCoefficient
        end if
      end if


      ok = getInt(pdb,'vsc',vsc) 
      if( ok.eq.0 )then
        write(*,'("*** NAME:ERROR: vsc NOT FOUND")') 
      end if

      ntdc = nd   ! number of time dependent components
      if( pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel )then
       ntdc = ntdc+1   ! include Temperature
      end if

      kc=nc
      ec=kc+1

      ok = getInt(pdb,'fillCoefficientsScalarSystem',fillCoefficientsScalarSystem) 
      if( ok.eq.0 )then
        write(*,'("*** NAME:ERROR: fillCoefficientsScalarSystem NOT FOUND")') 
      else
        if( debug.gt.4 )then
         write(*,'("*** NAME:fillCoefficientsScalarSystem=",i6)') fillCoefficientsScalarSystem
        end if
      end if

      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("NAME:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if
      if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
        write(*,'("NAME:ERROR gridType=",i6)') gridType
        stop 2
      end if
      if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
        write(*,'("NAME:ERROR uc,vc,ws=",3i6)') uc,vc,wc
        stop 4
      end if

c      write(*,'("NAME: turbulenceModel=",2i6)') turbulenceModel
c      write(*,'("NAME: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc

      if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. kc.gt.1000) )then
        write(*,'("NAME:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc
        stop 5
      end if


! *****************************************************************************************************8
      ! ** it did not affect performance to use an array to index coeff ***
     
      width = orderOfAccuracy+1      ! width of the MATRIX stencil   *********** fix this **********
      halfWidth = (width-1)/2 

      if( nd.eq.2 )then
       halfWidth3=0
       do i2=-halfWidth,halfWidth
         do i1=-halfWidth,halfWidth
          ma2(i1,i2,0)=i1+halfWidth+width*(i2+halfWidth)
         end do
       end do
      else if( nd.eq.3 )then
       halfWidth3=halfWidth
       do i3=-halfWidth,halfWidth
         do i2=-halfWidth,halfWidth
           do i1=-halfWidth,halfWidth
             ma3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(i3+halfWidth))
           end do
         end do
       end do
      end if

      do e=0,5
      do c=0,5
        if( e.eq.c )then
          delta(c,e)=1.
        else
	  delta(c,e)=0.
        end if
      end do
      end do

      do m=0,2
       dxvsqi(m)=1./(dx(m)**2)
      end do

      if( use2ndOrderAD.ne.0 )then
        if( debug .gt.3 )then
          write(*,'(" NAME: INFO: 2nd order art-diss is on: ad21,ad22=",2(e9.2,1x))') ad21,ad22
        end if
      else
        ad21=0.
        ad22=0. 
      end if
      cd22=ad22/(nd**2)  ! for 2nd-order artificial dissipation
	
      if( pdeModel.eq.BoussinesqModel )then
        if( debug .gt.3 )then
          write(*,'(" NAME: INFO: Boussinesq: kThermal,adcBoussinesq=",2(e9.2,1x))') kThermal,adcBoussinesq
        end if
      end if


      ! *** define constants in the implicit operator
      ! 

      cmpu=uc-uc ! component number for u in the matrix 
      cmpv=vc-uc 
      cmpw=wc-uc 
      qc=tc      ! another name for tc 
      cmpq=tc-uc ! temperature
      cmpk=kc-uc ! k 
      cmpe=ec-uc ! eps 

      ndu=nd     ! number of velocity components in the matrix 
      if( fillCoefficientsScalarSystem.ge.fillCoeffU .and. fillCoefficientsScalarSystem.le.fillCoeffW )then
        ! we are filling a scalar matrix for one component of the velocity 
        ndu=1
        cmpv=cmpu
        cmpw=cmpu
        cmpq=cmpu
      else if( fillCoefficientsScalarSystem.eq.fillCoeffT )then
        ! we are filling a scalar matrix for T 
        ndu=0
        cmpv=cmpu
        cmpw=cmpu
        cmpq=cmpu
      end if

      eqnu=cmpu  ! equation number for u  in the matrix
      eqnv=cmpv
      eqnw=cmpw 
      eqnq=cmpq
      eqnk=cmpk
      eqne=cmpe

      dtImp=dt*implicitFactor

      nuDt= nu*dt*implicitFactor  ! matrix: coefficient of Laplacian in the matrix 
      kDt = kThermal*dt*implicitFactor ! for T equation
      aDt = dt*implicitFactor      ! matrix: coefficient of advection terms in the matrix
      bDt = dt*implicitFactor      ! matrix: coefficient of extra zero-order linearized terms such as u0.x
      teDt = dt*thermalExpansivity*implicitFactor  ! matrix: coeff of buoyancy term 

      nuImp=nu                     ! RHS : nu 
      aImp=1.                      ! RHS: coeff of implicit advection terms
      bImp=1.                      ! RHS: coefficient of extra zero-order linearized terms such as u0.x
      ! nuImp=nu*(1.-implicitFactor) ! for RHS
      ! aImp=(1.-implicitFactor)     ! RHS: coeff of implicit advection terms
      ! aExp=0.                      ! advection terms are implicit 

      ! ** NOTE: we force the buoyancy terms to be explicit below **
      tImp=1.                      ! coefficient of the implicit buoyancy term (set to zero if not implicit)
      if( fillCoefficientsScalarSystem.ne.0 )then
        tImp=0.  ! buoyancy term is not implicit when we solve scalar systems
      end if


      nonlinearTermsAreImplicit=0
      if( implicitVariation.eq.implicitViscous )then
        aDt=0.   ! matrix: advection terms are NOT implicit 
        bDt=0.   ! matrix: zero-order linearized terms are NOT implicit
        ! aExp=1.  ! advection terms are explicit
        aImp=0.  ! RHS: coeff of implicit advection terms
        bImp=0.
        tImp=0.  ! Boussinesq terms are explicit
      else if( implicitVariation.eq.implicitAdvectionAndViscous )then 
        nonlinearTermsAreImplicit=1
        bDt=0.
        bImp=0.
        tImp=1.
      else if( implicitVariation.eq.implicitFullLinearized )then
        nonlinearTermsAreImplicit=1
        ! ++ bDt=0.
        ! ++ bImp=0.
        tImp=1.
      else 
        write(*,'(" NAME: ERROR: unexpected implicitVariation=",i6)') implicitVariation
        stop 7200      
      end if

      ! *** force the buoyancy term to be explicit : (we could add this as an option)
      !    if the main balance in the momentum is grad(p) = - alpha*g*T then we want both these
      !    terms to be explicit. 
      teDt=0.
      tImp=0.


      ! The next macro is defined in insImpXX.bf and is used to look up parameters etc. 
      initializePdeParameters()


      if( debug.gt.3 )then
        if( evalRightHandSide.eq.1 )then
          write(*,'("NAME: *EVAL RHS : implicitOption=",i2," (0=all,1=none,2=sep,3=all)")')  implicitOption
        endif
      endif
      if( debug.gt.7 )then
        if( evalRightHandSide.eq.1 )then
          write(*,'("NAME: ******** EVAL RHS **********")') 
        end if
        if( evalResidual.eq.1 )then
          write(*,'("NAME: ******** EVAL RESIDUAL **********")') 
        end if
        if( fillCoefficients.eq.1 )then
          write(*,'("NAME: ******** FILL COEFF **********")') 
        end if
        ! ' 
  
        write(*,'("NAME: pdeModel,nonLinear,impVar=",3i4)') pdeModel,nonlinearTermsAreImplicit,implicitVariation
        if( fillCoefficients.eq.1 )then
          write(*,'("NAME: stencilSize,ncc=",2i4)') stencilSize,ncc
        end if
        write(*,'("NAME: aDt,bDt,nuDt=",3e10.2)') aDt,bDt,nuDt
        write(*,'("NAME: implicitFactor,nuImp,aImp=",f4.2,1x,3e10.2)') implicitFactor,nuImp,aImp
        !'
      end if

      if( evalRightHandSide.eq.1 .and. (nonlinearTermsAreImplicit.eq.1 .or. use2ndOrderAD.ne.0) )then
        evalLinearizedDerivatives = 1
      else
        evalLinearizedDerivatives = 0
      end if



! Define operator parameters
!   $DIM : number of spatial dimensions
!   $ORDER : order of accuracy of an approximation
!   $GRIDTYPE : rectangular or curvilinear
!   $MATRIX_STENCIL_WIDTH : space in the global coeff matrix was allocated to hold this size stencil
!   $STENCIL_WIDTH : stencil width of the local coeff-matrix (such as xCoeff, yCoeff, lapCoeff, ...)
#perl  $ORDER=2;  $MATRIX_STENCIL_WIDTH=3; $STENCIL_WIDTH=3; 

      if(  nd.eq.2 .and. gridType.eq.rectangular .and. orderOfAccuracy.eq.2 )then
#perl $DIM=2; $GRIDTYPE="rectangular";

       ! fill the coefficients:
       fillCoeff()
       ! fill matrix BCs
       fillMatrixBoundaryConditions()
       ! assign the RHS:
       assignRHS()
      else if( nd.eq.2 .and. gridType.eq.curvilinear .and. orderOfAccuracy.eq.2 )then
#perl $DIM=2; $GRIDTYPE="curvilinear";

       ! fill the coefficients:
       fillCoeff()
       ! fill matrix BCs
       fillMatrixBoundaryConditions()
       ! assign the RHS:
       assignRHS()

      else if(  nd.eq.3 .and. gridType.eq.rectangular .and. orderOfAccuracy.eq.2 )then
#perl $DIM=3; $GRIDTYPE="rectangular";

       ! fill the coefficients:
       fillCoeff()
       ! fill matrix BCs
       fillMatrixBoundaryConditions()
       ! assign the RHS:
       assignRHS()

      else if( nd.eq.3 .and. gridType.eq.curvilinear .and. orderOfAccuracy.eq.2 )then
#perl $DIM=3; $GRIDTYPE="curvilinear";

       ! fill the coefficients:
       fillCoeff()
       ! fill matrix BCs
       fillMatrixBoundaryConditions()
       ! assign the RHS:
       assignRHS()

      else

        if( orderOfAccuracy.ne.2 )then
	  write(*,'("NAME: ERROR - not implemented for orderOfAccuracy=",i4)') orderOfAccuracy
        end if


        stop 9425
      end if

      return
      end

#endMacro

