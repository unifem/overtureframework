! **************************************************************************************************
!   Define the full implicit matrix for the incompressible Navier-Stokes
! **************************************************************************************************


! Macro's for forming the general implicit matrix
#Include "insImp.h"


! The next include file defines conservative approximations to coefficent matrices
#Include "consCoeff.h"


! ======================================================================================================
! Set up to 10 different local matrix operators to the global matrix coeff 
! in equation "e" and component "c" IN THE GHOST POINT (i1m,i2m,i3m) 
!
! ALSO SET EQUATION NUMBERS and CLASSIFY  
! 
! (Leave final unused arguments empty)
! ======================================================================================================
#beginMacro setCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,op10)
 setClassify(e,i1m,i2m,i3m, ghost1)            !macro to set classify

 do m3=-halfWidth3,halfWidth3
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
#If #op10 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))+(op10(MA(m1,m2,m3)))
#Elif #op9 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))
#Elif #op8 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))
#Elif #op7 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))
#Elif #op6 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))
#Elif #op5 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))
#Elif #op4 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))
#Elif #op3 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))
#Elif #op2 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))
#Else
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=(op1(MA(m1,m2,m3)))
#End
  ! The equation for pt (e,i1m,i2m,i3m) is centered on (c,i1,i2,i3): 
  setEquationNumber(MCE(m1,m2,m3,c,e), e,i1m,i2m,i3m,  c,i1+m1,i2+m2,i3+m3 )  !macro to set equationNumber

 end do
 end do
 end do
#endMacro

#beginMacro setCoeffGhost9(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9)
setCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,) 
#endMacro
#beginMacro setCoeffGhost8(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8)
setCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,,) 
#endMacro
#beginMacro setCoeffGhost7(c,e,coeff,op1,op2,op3,op4,op5,op6,op7)
setCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,,,) 
#endMacro
#beginMacro setCoeffGhost6(c,e,coeff,op1,op2,op3,op4,op5,op6)
setCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,,,,) 
#endMacro
#beginMacro setCoeffGhost5(c,e,coeff,op1,op2,op3,op4,op5)
setCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,,,,,) 
#endMacro
#beginMacro setCoeffGhost4(c,e,coeff,op1,op2,op3,op4)
setCoeffGhost10(c,e,coeff,op1,op2,op3,op4,,,,,,) 
#endMacro
#beginMacro setCoeffGhost3(c,e,coeff,op1,op2,op3)
setCoeffGhost10(c,e,coeff,op1,op2,op3,,,,,,,) 
#endMacro
#beginMacro setCoeffGhost2(c,e,coeff,op1,op2)
setCoeffGhost10(c,e,coeff,op1,op2,,,,,,,,) 
#endMacro
#beginMacro setCoeffGhost1(c,e,coeff,op1)
setCoeffGhost10(c,e,coeff,op1,,,,,,,,,) 
#endMacro

! ======================================================================================================
! Add up to 10 different local matrix operators to the global matrix coeff 
! in equation "e" and component "c" IN THE GHOST POINT (i1m,i2m,i3m) 
! 
! (Leave final unused arguments empty)
! ======================================================================================================
#beginMacro addCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,op10)
 do m3=-halfWidth3,halfWidth3
 do m2=-halfWidth,halfWidth
 do m1=-halfWidth,halfWidth
#If #op10 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))+(op10(MA(m1,m2,m3)))
#Elif #op9 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))+(op9(MA(m1,m2,m3)))
#Elif #op8 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))+(op8(MA(m1,m2,m3)))
#Elif #op7 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))+(op7(MA(m1,m2,m3)))
#Elif #op6 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))+\
       (op6(MA(m1,m2,m3)))
#Elif #op5 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))+(op5(MA(m1,m2,m3)))
#Elif #op4 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+\
       (op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))+(op4(MA(m1,m2,m3)))
#Elif #op3 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))+(op3(MA(m1,m2,m3)))
#Elif #op2 ne ""
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+(op1(MA(m1,m2,m3)))+(op2(MA(m1,m2,m3)))
#Else
   coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)=coeff(MCE(m1,m2,m3,c,e),i1m,i2m,i3m)+(op1(MA(m1,m2,m3)))
#End
  ! The equation for pt (e,i1m,i2m,i3m) is centered on (c,i1,i2,i3): 
  setEquationNumber(MCE(m1,m2,m3,c,e), e,i1m,i2m,i3m,  c,i1+m1,i2+m2,i3+m3 )  !macro to set equationNumber
 end do
 end do
 end do
#endMacro

#beginMacro addCoeffGhost9(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9)
addCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,op9,) 
#endMacro
#beginMacro addCoeffGhost8(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8)
addCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,op8,,) 
#endMacro
#beginMacro addCoeffGhost7(c,e,coeff,op1,op2,op3,op4,op5,op6,op7)
addCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,op7,,,) 
#endMacro
#beginMacro addCoeffGhost6(c,e,coeff,op1,op2,op3,op4,op5,op6)
addCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,op6,,,,) 
#endMacro
#beginMacro addCoeffGhost5(c,e,coeff,op1,op2,op3,op4,op5)
addCoeffGhost10(c,e,coeff,op1,op2,op3,op4,op5,,,,,) 
#endMacro
#beginMacro addCoeffGhost4(c,e,coeff,op1,op2,op3,op4)
addCoeffGhost10(c,e,coeff,op1,op2,op3,op4,,,,,,) 
#endMacro
#beginMacro addCoeffGhost3(c,e,coeff,op1,op2,op3)
addCoeffGhost10(c,e,coeff,op1,op2,op3,,,,,,,) 
#endMacro
#beginMacro addCoeffGhost2(c,e,coeff,op1,op2)
addCoeffGhost10(c,e,coeff,op1,op2,,,,,,,,) 
#endMacro
#beginMacro addCoeffGhost1(c,e,coeff,op1)
addCoeffGhost10(c,e,coeff,op1,,,,,,,,,) 
#endMacro


! Here are the coefficients of the free surface BCs -- needs normal an(0:2) and delta function
#defineMacro CSF(n,a,b) (delta(a,b)*an(n) + delta(n,a)*an(b) + delta(n,b)*an(a) - 2.*an(n)*an(a)*an(b))

! ===============================================================================================
!  Add a FREE SURFACE (vector)  BC to the matrix
!  Macro args:
!   coeff : coefficient matrix to fill in.
!   cmpu,eqnu : fill in equations eqnu,...,eqnu+nd-1 and components cmpu,...,cmpu+nd-1
!   i1,i2,i3 : boundary point, will assign ghost point
!
! NOTES:
!  See the file surfins.pdf for a derivation of the equations used here
!    (div(v))*n + (I-n n^T)(tauv.n )/mu = RHS 
!
!   =>  sum_a sum_b  CSF(n,a,b) * (partial u_a/partial x_b)  = RHS(n),   n=0,1,2
!  
! ===============================================================================================
#beginMacro fillMatrixFreeSurface(coeff, cmpu,eqnu, i1,i2,i3, an )

 i1m=i1-is1  ! ghost point
 i2m=i2-is2
 i3m=i3-is3

 getNormalForCurvilinearGrid(side,axis,i1,i2,i3)

 ! write(*,'(" IMP: FREE SURFACE i1,i2=",2i2," ndu=",i4," normal=",2e10.2)') i1,i2,ndu,an(0),an(1)
 ! write(*,'("    :              i1m,i2m,i3m=",3i3)') i1m,i2m,i3m
 ! write(*,'("    : c000,c001,c010,c011=",4e10.2)') CSF(0,0,0),CSF(0,0,1),CSF(0,1,0),CSF(0,1,1)
 ! write(*,'("    : c100,c101,c110,c111=",4e10.2)') CSF(1,0,0),CSF(1,0,1),CSF(1,1,0),CSF(1,1,1)

 ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
 ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
 opEvalJacobianDerivatives(aj,0)

 ! evaluate the coeff operators 
 ! getCoeff(identity, iCoeff,aj)
 getCoeff(x, xCoeff,aj)
 getCoeff(y, yCoeff,aj)
 #If $DIM == 3
  getCoeff(z, zCoeff,aj)
 #End

 do m=0,ndc-1
  coeff(m,i1m,i2m,i3m)=0.  ! init all elements to zero
 end do
 
 #If $DIM == 2
  ! --- 2D ---
  if( fillCoefficientsScalarSystem.eq.0 )then
   ! Fill in the coupled equations for u and v  
   do n=0,nd-1
     ! equation n:  (equation numbers and classify  are set in these calls)
     setCoeffGhost2(cmpu,eqnu+n,coeff,CSF(n,0,0)*xCoeff,CSF(n,0,1)*yCoeff)
     addCoeffGhost2(cmpv,eqnu+n,coeff,CSF(n,1,0)*xCoeff,CSF(n,1,1)*yCoeff)
   end do

  else if( fillCoefficientsScalarSystem.eq.fillCoeffU )then
    ! We decouple the coupled velocity components: Only add coefficients of u 
    n=0 
    setCoeffGhost2(cmpu,eqnu,coeff,CSF(n,0,0)*xCoeff,CSF(n,0,1)*yCoeff)

  else if( fillCoefficientsScalarSystem.eq.fillCoeffv )then
    ! We decouple the coupled velocity components: Only add components of v 
    n=1 
    setCoeffGhost2(cmpu,eqnu,coeff,CSF(n,1,0)*xCoeff,CSF(n,1,1)*yCoeff)

  end if

 #Else
  ! ---  3D ---
  if( fillCoefficientsScalarSystem.eq.0 )then
   ! Fill in the coupled equations for u, v and w
   ! write(*,'("(i1,i2,i3) = ",3i3)') i1,i2,i3
   do n=0,nd-1
     ! equation n:   (equation numbers and classify  are set in these calls)
     ! write(*,'("n = ",1i3)') n
     ! write(*,'("    : cn00,cn01,cn02",3e10.2)') CSF(n,0,0),CSF(n,0,1),CSF(n,0,2)
     ! write(*,'("    : cn10,cn11,cn12",3e10.2)') CSF(n,1,0),CSF(n,1,1),CSF(n,1,2)
     ! write(*,'("    : cn20,cn21,cn22",3e10.2)') CSF(n,2,0),CSF(n,2,1),CSF(n,2,2)

     setCoeffGhost3(cmpu,eqnu+n,coeff,CSF(n,0,0)*xCoeff,CSF(n,0,1)*yCoeff,CSF(n,0,2)*zCoeff)
     addCoeffGhost3(cmpv,eqnu+n,coeff,CSF(n,1,0)*xCoeff,CSF(n,1,1)*yCoeff,CSF(n,1,2)*zCoeff)
     addCoeffGhost3(cmpw,eqnu+n,coeff,CSF(n,2,0)*xCoeff,CSF(n,2,1)*yCoeff,CSF(n,2,2)*zCoeff)
   end do

  else if( fillCoefficientsScalarSystem.eq.fillCoeffU )then
    ! We decouple the coupled velocity components: Only add coefficients of u 
    n=0 
    setCoeffGhost3(cmpu,eqnu,coeff,CSF(n,0,0)*xCoeff,CSF(n,0,1)*yCoeff,CSF(n,0,2)*zCoeff)

  else if( fillCoefficientsScalarSystem.eq.fillCoeffv )then
    ! We decouple the coupled velocity components: Only add components of v 
    n=1 
    setCoeffGhost3(cmpu,eqnu,coeff,CSF(n,0,0)*xCoeff,CSF(n,0,1)*yCoeff,CSF(n,0,2)*zCoeff)

  else if( fillCoefficientsScalarSystem.eq.fillCoeffw )then
    ! We decouple the coupled velocity components: Only add components of w
    n=2
    setCoeffGhost3(cmpu,eqnu,coeff,CSF(n,0,0)*xCoeff,CSF(n,0,1)*yCoeff,CSF(n,0,2)*zCoeff)

  end if  
 #End
 
#endMacro



! =============================================================
! macro to declare temporary variables:
! =============================================================
#beginMacro declareInsImpTemporaryVariables()
 declareTemporaryVariables(2,2)
 declareParametricDerivativeVariables(uu,3)   ! declare temp variables uu, uur, uus, ...
 declareParametricDerivativeVariables(vv,3) 
 declareParametricDerivativeVariables(ww,3) 
 declareParametricDerivativeVariables(pp,3) 
 declareParametricDerivativeVariables(qq,3) 
 declareParametricDerivativeVariables(uul,3)   ! declare temp variables uu, uur, uus, ...
 declareParametricDerivativeVariables(vvl,3) 
 declareParametricDerivativeVariables(wwl,3) 
 declareParametricDerivativeVariables(qql,3) 
 declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)
 real radi 
 real yyCoeff(0:maxWidthDim-1)
 integer bc0

 real scale,dsgT
 real K0ph,K0mh,K1ph,K1mh,K2ph,K2mh
 real Kzzm,Kzmz,Kmzz,Kzzz,Kpzz,Kzpz,Kzzp
 real ajzzm,ajzmz,ajmzz,ajzzz,ajpzz,ajzpz,ajzzp

 real au11ph,au11mh,au22ph,au22mh,au33ph,au33mh,au11mzz,au11zzz,au11pzz,au22zmz,au22zzz,au22zpz,au33zzm,au33zzz,au33zzp,\
      au12pzz,au12zzz,au12mzz,au13pzz,au13zzz,au13mzz,au21zpz,au21zzz,au21zmz,au23zpz,au23zzz,au23zmz,\
      au31zzp,au31zzz,au31zzm,au32zzp,au32zzz,au32zzm

#endMacro

! =====================================================================================
! This macro is used in insImp.h and is used to look up parameters etc. for this PDE
! =====================================================================================
#beginMacro initializePdeParameters()
 if( pdeModel.ne.standardModel .and. pdeModel.ne.BoussinesqModel )then
   write(*,'("insImpINS:ERROR: unexpected pdeModel=",i3)') pdeModel
   stop 2734
 end if
 ! make sure these are initialized (used in evaluation of adCoeffl even if cd22=0. )
 ulx=0.
 uly=0.
 ulz=0.
 vlx=0.
 vly=0.
 vlz=0.
 wlx=0.
 wly=0.
 wlz=0.

 ! write(*,'("insImpINS: Entering ... fillCoefficientsScalarSystem=",i2," materialFormat=",i3)') fillCoefficientsScalarSystem,materialFormat
 ! write(*,'("insImpINS: kDt=",e10.2)') kDt

 ! --- Output rho, Cp and kThermal t=0 for testing ---
 if( .false. .and. materialFormat.ne.0 .and. (nd1b-nd1a)*(nd2b-nd2a).lt. 1000 )then

  write(*,'("insImpINS: variable material properties rho,Cp,kThermal for T")')
  write(*,'("insImpINS: rho:")')
  i3=nd3a
  do i2=nd2b,nd2a,-1
    if( materialFormat.eq.piecewiseConstantMaterialProperties )then
     write(*,'(100(f5.1))') (rhopc(i1,i2,i3),i1=nd1a,nd1b)
    else
     write(*,'(100(f5.1))') (rhov(i1,i2,i3),i1=nd1a,nd1b)
    end if
  end do 
  write(*,'("insImpINS: Cp:")')
  do i2=nd2b,nd2a,-1
    if( materialFormat.eq.piecewiseConstantMaterialProperties )then
     write(*,'(100(f5.1))') (Cppc(i1,i2,i3),i1=nd1a,nd1b)
    else
     write(*,'(100(f5.1))') (Cpv(i1,i2,i3),i1=nd1a,nd1b)
    end if
  end do 
  write(*,'("insImpINS: thermalConductivity:")')
  do i2=nd2b,nd2a,-1
    if( materialFormat.eq.piecewiseConstantMaterialProperties )then
     write(*,'(100(f5.1))') (thermalKpc(i1,i2,i3),i1=nd1a,nd1b)
    else
     write(*,'(100(f5.1))') (thermalKv(i1,i2,i3),i1=nd1a,nd1b)
    end if
  end do 

 end if

#endMacro

! =============================================================
! *** NOT USED ANYMORE ***
! macro to compute (variable) material properties
! Compute:
!   kDt : (K/(rho*cp)) * dt*implicitFactor
!       : K = thermal conductivity
! =============================================================
! #beginMacro getMaterialProperties()
!  if( materialFormat.eq.constantMaterialProperties )then
!    ! const material properties -- do nothing
!  else if( materialFormat.eq.piecewiseConstantMaterialProperties )then
!    ! piecewise constant material properties
!    kDt = dt*implicitFactor*( thermalKpc(i1,i2,i3)/( rhopc(i1,i2,i3)*Cppc(i1,i2,i3) ) )
!    ! write(*,'(" (i1,i2)=",i3,",",i3,") Kpc = ",e10.2)') i1,i2,thermalKpc(i1,i2,i3)/( rhopc(i1,i2,i3)*Cppc(i1,i2,i3) )
!  else if( materialFormat.eq.variableMaterialProperties )then 
!    ! variable material properties
!    kDt = dt*implicitFactor*( thermalKv(i1,i2,i3)/( rhov(i1,i2,i3)*Cpv(i1,i2,i3) ) )
!    ! write(*,'(" (i1,i2)=",i3,",",i3,") Kv = ",e10.2)') i1,i2,thermalKv(i1,i2,i3)/( rhov(i1,i2,i3)*Cpv(i1,i2,i3) )
!  end if
! #endMacro

! =============================================================================================================
!  Get the coefficients for the conservative discretization of 
!          (1/rho*cp)* div( K grad) 
!  
!  Macro parameters:
!    scale0 : scale the coefficients by this value 
! =============================================================================================================
#beginMacro getDivGradCoefficientsTemperature(scale0)
#If $DIM == 2 

 ! ---------- 2D -----------

 ! Get coefficients at nearby points: 
 if( materialFormat.eq.constantMaterialProperties )then
   ! const material properties 
   stop 11199
 else if( materialFormat.eq.piecewiseConstantMaterialProperties )then
   ! piecewise constant material properties
   scale = (scale0)/( rhopc(i1,i2,i3)*Cppc(i1,i2,i3) )
   Kzmz=thermalKpc(i1  ,i2-1,i3)
   Kmzz=thermalKpc(i1-1,i2  ,i3)
   Kzzz=thermalKpc(i1  ,i2  ,i3)
   Kpzz=thermalKpc(i1+1,i2  ,i3)
   Kzpz=thermalKpc(i1  ,i2+1,i3)
 else if( materialFormat.eq.variableMaterialProperties )then 
   ! variable material properties
   scale = (scale0)/( rhov(i1,i2,i3)*Cpv(i1,i2,i3) )
   Kzmz=thermalKv(i1  ,i2-1,i3)
   Kmzz=thermalKv(i1-1,i2  ,i3)
   Kzzz=thermalKv(i1  ,i2  ,i3)
   Kpzz=thermalKv(i1+1,i2  ,i3)
   Kzpz=thermalKv(i1  ,i2+1,i3)

 end if

 !  ---- Dx( K*u.x ) + Dy( K*u.y ) ----

 #If $GRIDTYPE eq "curvilinear"
  ! evaluate the jacobian at nearby points:
  ajzmz = ajac2d(i1  ,i2-1,i3)
  ajmzz = ajac2d(i1-1,i2  ,i3)
  ajzzz = ajac2d(i1  ,i2  ,i3)
  ajpzz = ajac2d(i1+1,i2  ,i3)
  ajzpz = ajac2d(i1  ,i2+1,i3)
 
  ! 1. Get coefficients au11ph, au11mh, au22ph, etc. for 
  !          Dx( K*u.x ) + Dy( K*u.y ) 
  getCoeffForDxADxPlusDyBDy(au, Kzmz,Kmzz,Kzzz,Kpzz,Kzpz, Kzmz,Kmzz,Kzzz,Kpzz,Kzpz )
 
  ! scaling factors: 
  dr0i = (scale)/(ajzzz*dr(0)**2)
  dr1i = (scale)/(ajzzz*dr(1)**2)
  dr0dr1 = (scale)/(ajzzz*4.*dr(0)*dr(1))
 
  scaleCoefficients( au11ph,au11mh,au22ph,au22mh,au12pzz,au12mzz,au21zpz,au21zmz )


 #Elif $GRIDTYPE eq "rectangular"

   K0ph = .5*( Kpzz+Kzzz )  ! K(i1+1/2,i2,i3)
   K0mh = .5*( Kzzz+Kmzz )  ! K(i1-1/2,i2,i3)

   K1ph = .5*( Kzpz+Kzzz )  ! K(i1,i2+1/2,i3)
   K1mh = .5*( Kzzz+Kzmz )  ! K(i1,i2-1/2,i3)


   au11ph = K0ph*dxvsqi(0)*(scale)
   au11mh = K0mh*dxvsqi(0)*(scale)
   au22ph = K1ph*dxvsqi(1)*(scale)
   au22mh = K1mh*dxvsqi(1)*(scale)
   au12pzz=0.
   au12mzz=0.
   au21zpz=0.
   au21zmz=0.
  
 #Else
   stop 1101
 #End

 ! write(*,'(" (i1,i2)=(",i3,",",i3,") Kzmz,Kmzz,Kzz,Kpzz,Kzpz = ",5e10.2)') i1,i2,Kzmz,Kmzz,Kzzz,Kpzz,Kzpz
 ! write(*,'(" scale,dtImp =",2e10.2)') scale,dtImp
 ! write(*,'(" au11ph,au11mh,au22ph,au22mh=",4e10.2)') au11ph,au11mh,au22ph,au22mh


#Else

 ! ---------- 3D -----------

 ! Get coefficients at nearby points: 
 if( materialFormat.eq.constantMaterialProperties )then
   ! const material properties 
   stop 11199
 else if( materialFormat.eq.piecewiseConstantMaterialProperties )then
   ! piecewise constant material properties
   scale = (scale0)/( rhopc(i1,i2,i3)*Cppc(i1,i2,i3) )
   Kzzm=thermalKpc(i1  ,i2  ,i3-1)
   Kzmz=thermalKpc(i1  ,i2-1,i3  )
   Kmzz=thermalKpc(i1-1,i2  ,i3  )
   Kzzz=thermalKpc(i1  ,i2  ,i3  )
   Kpzz=thermalKpc(i1+1,i2  ,i3  )
   Kzpz=thermalKpc(i1  ,i2+1,i3  )
   Kzzp=thermalKpc(i1  ,i2  ,i3+1)
 else if( materialFormat.eq.variableMaterialProperties )then 
   ! variable material properties
   scale = (scale0)/( rhov(i1,i2,i3)*Cpv(i1,i2,i3) )
   Kzzm=thermalKv(i1  ,i2  ,i3-1)
   Kzmz=thermalKv(i1  ,i2-1,i3  )
   Kmzz=thermalKv(i1-1,i2  ,i3  )
   Kzzz=thermalKv(i1  ,i2  ,i3  )
   Kpzz=thermalKv(i1+1,i2  ,i3  )
   Kzpz=thermalKv(i1  ,i2+1,i3  )
   Kzzp=thermalKv(i1  ,i2  ,i3+1)
 end if

 !  ---- Dx( K*T.x ) + Dy( K*T.y ) + Dz( K*T.z ) ----

 #If $GRIDTYPE eq "curvilinear"
  ! evaluate the jacobian at nearby points:
  ajzzm = ajac3d(i1  ,i2  ,i3-1)
  ajzmz = ajac3d(i1  ,i2-1,i3  )
  ajmzz = ajac3d(i1-1,i2  ,i3  )
  ajzzz = ajac3d(i1  ,i2  ,i3  )
  ajpzz = ajac3d(i1+1,i2  ,i3  )
  ajzpz = ajac3d(i1  ,i2+1,i3  )
  ajzzp = ajac3d(i1  ,i2  ,i3+1)
 
  ! ------------------------------------------------------------------------------------------------------------
  ! au. Get coefficients au11ph, au11mh, au22ph, etc. for 
  !          Dx( K*u.x ) + Dy( K*u.y ) + Dz( K*u.z )  
  getCoeffForDxADxPlusDyBDyPlusDzCDz(au, Kzzm,Kzmz,Kmzz,Kzzz,Kpzz,Kzpz,Kzzp, \
                                         Kzzm,Kzmz,Kmzz,Kzzz,Kpzz,Kzpz,Kzzp, \
                                         Kzzm,Kzmz,Kmzz,Kzzz,Kpzz,Kzpz,Kzzp )
  ! scaling factors: 
  dr0i = (scale)/(ajzzz*dr(0)**2)
  dr1i = (scale)/(ajzzz*dr(1)**2)
  dr2i = (scale)/(ajzzz*dr(2)**2)
  dr0dr1 = (scale)/(ajzzz*4.*dr(0)*dr(1))
  dr0dr2 = (scale)/(ajzzz*4.*dr(0)*dr(2))
  dr1dr2 = (scale)/(ajzzz*4.*dr(1)*dr(2))
 
  scaleCoefficients3d( au11ph,au11mh,au22ph,au22mh,au33ph,au33mh,au12pzz,au12mzz,au13pzz,au13mzz,au21zpz,au21zmz,au23zpz,au23zmz,au31zzp,au31zzm,au32zzp,au32zzm )


 #Elif $GRIDTYPE eq "rectangular"

   K0ph = .5*( Kpzz+Kzzz )  ! K(i1+1/2,i2,i3)
   K0mh = .5*( Kzzz+Kmzz )  ! K(i1-1/2,i2,i3)

   K1ph = .5*( Kzpz+Kzzz )  ! K(i1,i2+1/2,i3)
   K1mh = .5*( Kzzz+Kzmz )  ! K(i1,i2-1/2,i3)

   K2ph = .5*( Kzzp+Kzzz )  ! K(i1,i2,i3+1/2)
   K2mh = .5*( Kzzz+Kzzm )  ! K(i1,i2,i3-1/2)

   ! equation for T
   !    Dx( K*T.x ) + Dy( K*T.y ) + Dz( K*T.z )  
   au11ph = K0ph*dxvsqi(0)*(scale)
   au11mh = K0mh*dxvsqi(0)*(scale)
   au22ph = K1ph*dxvsqi(1)*(scale)
   au22mh = K1mh*dxvsqi(1)*(scale)
   au33ph = K2ph*dxvsqi(2)*(scale)
   au33mh = K2mh*dxvsqi(2)*(scale)
   au12pzz=0.
   au12mzz=0.
   au13pzz=0.
   au13mzz=0.
   au21zpz=0.
   au21zmz=0.
   au23zpz=0.
   au23zmz=0.
   au31zzp=0.
   au31zzm=0.
   au32zzp=0.
   au32zzm=0.

 #Else
   stop 1102
 #End

#End
#endMacro


! ==============================================================================================================
!   Fill in the coefficients for the Incompressible Navier-Stokes Equations
! ==============================================================================================================
#beginMacro fillCoeffPDE()
if( fillCoefficients.eq.1 )then


 if( fillCoefficientsScalarSystem.ge.fillCoeffU .and. fillCoefficientsScalarSystem.le.fillCoeffW )then
   ! --- fill coefficients for a scalar system for a velocity component: I - nu*Delta - A.D. 

   ! write(*,'("@@@@ insImpINS: (u,v,w) fillCoefficientsScalarSystem=",i4)') fillCoefficientsScalarSystem
   ! '

 beginLoops()

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,1)

  ! evaluate the coeff operators 
  getCoeff(identity, iCoeff,aj)
  getCoeff(laplacian, lapCoeff,aj)

  ! dissCoeff = dr^2*D_rr + ds^2*D_ss [ + dt^2*D_tt ] 
  getCoeff(r2Dissipation, dissCoeff,aj )

  ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
  ! MAXDER = max number of parametric derivatives to precompute.
  opEvalParametricDerivative(u,uc,uu,1)    ! computes uur, uus 
  ! Evaluate the spatial derivatives of u (uses uur, uus):
  getOp(x, u,uc,uu,aj,u0x)       ! u.x
  getOp(y, u,uc,uu,aj,u0y)       ! u.y
  #If $DIM == 3
   getOp(z, u,uc,uu,aj,u0z)       ! u.z
  #End

  ! parametric derivatives of v: 
  opEvalParametricDerivative(u,vc,vv,1)        ! computes vvr, vvs 
  getOp(x, u,vc,vv,aj,v0x)       ! v.x
  getOp(y, u,vc,vv,aj,v0y)       ! v.y 
  #If $DIM == 3
   getOp(z, u,vc,vv,aj,v0z)      ! v.z

   opEvalParametricDerivative(u,wc,ww,1)        ! computes wwr, wws 
   getOp(x, u,wc,ww,aj,w0x)      ! w.x
   getOp(y, u,wc,ww,aj,w0y)      ! w.y 
   getOp(z, u,wc,ww,aj,w0z)      ! w.z
  #End

 getArtificialDissipationCoeff(adCoeff, u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z )

 ! Form : I - nuDt*Delta -adCoeff*dt*( urr + uss + .. )
 setCoeff3(cmpu,eqnu,coeff,iCoeff,-nuDt*lapCoeff,-adCoeff*dt*dissCoeff)

 #If $DIM == 2 
  if( isAxisymmetric.eq.1 )then
    ! add axisymmetric corrections 
    radi = radiusInverse(i1,i2,i3)
    if( radi.ne.0. )then
     getCoeff(y, yCoeff,aj)
     if( fillCoefficientsScalarSystem.eq.fillCoeffU )then
      addCoeff1(cmpu,eqnu,coeff, -nuDt*radi*yCoeff)                        ! -nu*dt*( U_r/r )
     else
      addCoeff2(cmpu,eqnu,coeff, -nuDt*radi*yCoeff, nuDt*radi**2*iCoeff)   ! -nu*dt*( V_r/r - V/r^2 )
     end if
    else
     ! corrections on the axis
     getCoeff(yy, yyCoeff,aj)
     if( fillCoefficientsScalarSystem.eq.fillCoeffU )then
      addCoeff1(cmpu,eqnu,coeff, -nuDt*yyCoeff)                            ! -nu*dt*( U_rr )
     else
      addCoeff1(cmpu,eqnu,coeff, -nuDt*.5*yyCoeff)                         ! -nu*dt*( .5*V_rr )
     end if
    end if
    
  end if

 #End

 endLoops()


 else if( fillCoefficientsScalarSystem.eq.fillCoeffT )then
   ! --- fill coefficients for a scalar system for the Temperature: I - kappa*Delta - A.D. 

  write(*,'("@@@@ insImpINS: T: fillCoefficientsScalarSystem=",i4)') fillCoefficientsScalarSystem

 beginLoops()

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,1)

  ! evaluate the coeff operators 
  getCoeff(identity, iCoeff,aj)
  getCoeff(laplacian, lapCoeff,aj)

  ! dissCoeff = dr^2*D_rr + ds^2*D_ss [ + dt^2*D_tt ] 
  getCoeff(r2Dissipation, dissCoeff,aj )

  if( materialFormat.eq.constantMaterialProperties )then
    ! constant material properties
    setCoeff3(cmpu,eqnu,coeff,iCoeff,-kDt*lapCoeff,-adcBoussinesq*dt*dissCoeff)
  else
   ! -- variable material properties : 
   ! get coefficients scaled by -dt*implicitFactor: 
   getDivGradCoefficientsTemperature(-dtImp)
   ! Set coeff = - dtImp*div( K grad)
   #If $DIM == 2 
     setDivTensorGradCoeff2d(cmpu,eqnu,au11ph,au11mh,au22ph,au22mh,au12pzz,au12mzz,au21zpz,au21zmz)
   #Else
     setDivTensorGradCoeff3d(cmpu,eqnu,au)
   #End

   addCoeff2(cmpu,eqnu,coeff,iCoeff,-adcBoussinesq*dt*dissCoeff)
  endif


 #If $DIM == 2 

  if( isAxisymmetric.eq.1 )then
    ! add axisymmetric corrections 

    ! *** FIX ME FOR variable material properties ***
    if( materialFormat.ne.constantMaterialProperties )then
      stop 6205
    end if

    radi = radiusInverse(i1,i2,i3)
    if( radi.ne.0. )then
     getCoeff(y, yCoeff,aj)
     addCoeff1(cmpu,eqnu,coeff, -kDt*radi*yCoeff)                        ! -nu*dt*( U_r/r )
    else
     ! corrections on the axis
     getCoeff(yy, yyCoeff,aj)
     addCoeff1(cmpu,eqnu,coeff, -kDt*yyCoeff)                            ! -nu*dt*( U_rr )
    end if
  end if

 #End

 endLoops()


 else
   ! --- fill coefficients for the full system ---

 ! write(*,'("&&&&& insImpINS: fill coefficients for a full system, pdeModel=",i3)') pdeModel

 beginLoops()

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,1)

  ! evaluate the coeff operators 
  getCoeff(identity, iCoeff,aj)
  getCoeff(laplacian, lapCoeff,aj)
  getCoeff(x, xCoeff,aj)
  getCoeff(y, yCoeff,aj)
  #If $DIM == 3
   getCoeff(z, zCoeff,aj)
  #End

  ! dissCoeff = dr^2*D_rr + ds^2*D_ss [ + dt^2*D_tt ] 
  getCoeff(r2Dissipation, dissCoeff,aj )

  ! for testing, get coeff for div( s grad )
  ! getOpCoeffDivScalarGrad(s(i1,i2,i3,0))

  ! **** get a fourth order dissipation u.rrrr + u.ssss 

  ! evaluate forward derivatives of the current solution: 

  ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
  ! MAXDER = max number of parametric derivatives to precompute.
  opEvalParametricDerivative(u,uc,uu,1)    ! computes uur, uus 
  ! Evaluate the spatial derivatives of u (uses uur, uus):
  getOp(x, u,uc,uu,aj,u0x)       ! u.x
  getOp(y, u,uc,uu,aj,u0y)       ! u.y
  #If $DIM == 3
   getOp(z, u,uc,uu,aj,u0z)       ! u.z
  #End

  ! parametric derivatives of v: 
  opEvalParametricDerivative(u,vc,vv,1)        ! computes vvr, vvs 
  getOp(x, u,vc,vv,aj,v0x)       ! v.x
  getOp(y, u,vc,vv,aj,v0y)       ! v.y 
  #If $DIM == 3
   getOp(z, u,vc,vv,aj,v0z)      ! v.z

   opEvalParametricDerivative(u,wc,ww,1)        ! computes vvr, vvs 
   getOp(x, u,wc,ww,aj,w0x)       ! w.x
   getOp(y, u,wc,ww,aj,w0y)       ! w.y 
   getOp(z, u,wc,ww,aj,w0z)       ! w.z
  #End

 getArtificialDissipationCoeff(adCoeff, u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z )

 if( gridIsMoving.ne.0 )then
   ugv = uu - gv(i1,i2,i3,0)
   vgv = vv - gv(i1,i2,i3,1)
  #If $DIM == 3
   wgv = ww - gv(i1,i3,i3,2)
  #End
 else
   ugv = uu
   vgv = vv
  #If $DIM == 3
   wgv = ww
  #End
 end if

 #If $DIM == 2 
  ! Form : I - nuDt*Delta + aDt*u*Dx + aDt*v*Dy + ...
  !    u0*ux + v0*uy + u*u0x + v*u0y 
  ! moving: (u-gv)*ux =  u*ux - gv*ux -> u0*ux+u*u0x-gv*ux = (u0-gv)*ux + u0x*u (linearized form)
  setCoeff6(cmpu,eqnu,coeff,iCoeff,-nuDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,bDt*u0x*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpv,eqnu,coeff, bDt*u0y*iCoeff)  ! v*u0y in u eqn
  ! addCoeff1(cmpv,eqnu,coeff, bDt*iCoeff)  ! for testing -- add v to u-eqn
  !    u0*vx + v0*vy + u*v0x + v*v0y 
  setCoeff6(cmpv,eqnv,coeff,iCoeff,-nuDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,bDt*v0y*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpu,eqnv,coeff, bDt*v0x*iCoeff)  ! u*v0x in v eqn

  if( isAxisymmetric.eq.1 )then
    ! add axisymmetric corrections 
    radi = radiusInverse(i1,i2,i3)
    if( radi.ne.0. )then
     addCoeff1(cmpu,eqnu,coeff, -nuDt*radi*yCoeff)                        ! -nu*dt*( U_r/r )
     addCoeff2(cmpv,eqnv,coeff, -nuDt*radi*yCoeff, nuDt*radi**2*iCoeff)   ! -nu*dt*( V_r/r - V/r^2 )
    else
     ! corrections on the axis
     getCoeff(yy, yyCoeff,aj)
     addCoeff1(cmpu,eqnu,coeff, -nuDt*yyCoeff)                            ! -nu*dt*( U_rr )
     addCoeff1(cmpv,eqnv,coeff, -nuDt*.5*yyCoeff)                         ! -nu*dt*( .5*V_rr )
    end if
    
  end if

 #Else
  ! Form : I - nuDt*Delta + aDt*u*Dx + aDt*v*Dy + aDt*w*dz
  !    u0*ux + v0*uy + w0*uz + u*u0x + v*u0y + w*u0z 
  setCoeff7(cmpu,eqnu,coeff,iCoeff,-nuDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,bDt*u0x*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpv,eqnu,coeff, bDt*u0y*iCoeff)  ! v*u0y in u eqn
  addCoeff1(cmpw,eqnu,coeff, bDt*u0z*iCoeff)  ! w*u0z in u eqn

  !    u0*vx + v0*vy + w0*vz + u*v0x + v*v0y + w*v0z 
  setCoeff7(cmpv,eqnv,coeff,iCoeff,-nuDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,bDt*v0y*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpu,eqnv,coeff, bDt*v0x*iCoeff)  ! u*v0x in u eqn
  addCoeff1(cmpw,eqnv,coeff, bDt*v0z*iCoeff)  ! w*v0z in u eqn

  !    u0*wx + v0*wy + w0*wz + u*w0x + v*w0y + w*w0z 
  setCoeff7(cmpw,eqnw,coeff,iCoeff,-nuDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,bDt*w0z*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpu,eqnw,coeff, bDt*w0x*iCoeff)  ! u*w0x in u eqn
  addCoeff1(cmpv,eqnw,coeff, bDt*w0y*iCoeff)  ! v*w0y in u eqn

 #End

 if( pdeModel.eq.BoussinesqModel )then

  ! ----------------------------------------------------------------------------
  ! ---------- add the temperature equation to the full system -----------------
  ! ----------------------------------------------------------------------------

  opEvalParametricDerivative(u,qc,qq,1)        ! computes parametric derivatives of T: qqr, qqs 
  getOp(x, u,qc,qq,aj,q0x)       ! q.x
  getOp(y, u,qc,qq,aj,q0y)       ! q.y 
  #If $DIM == 3
   getOp(z, u,qc,qq,aj,q0z)      ! q.z
  #End

 #If $DIM == 2 

  if( materialFormat.eq.constantMaterialProperties )then
    ! constant material properties:
    setCoeff5(cmpq,eqnq,coeff,iCoeff,-kDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,-adcBoussinesq*dt*dissCoeff)
  else
   ! get coefficients scaled by -dt*implicitFactor: 
   getDivGradCoefficientsTemperature(-dtImp)
   setDivTensorGradCoeff2d(cmpq,eqnq,au11ph,au11mh,au22ph,au22mh,au12pzz,au12mzz,au21zpz,au21zmz)
   addCoeff4(cmpq,eqnq,coeff,iCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,-adcBoussinesq*dt*dissCoeff)
  endif


  addCoeff1(cmpu,eqnq,coeff, bDt*q0x*iCoeff)  ! u*q0x  
  addCoeff1(cmpv,eqnq,coeff, bDt*q0y*iCoeff)  ! v*q0y 

  addCoeff1(cmpq,eqnu,coeff, teDt*gravity(0)*iCoeff)  ! add buoyancy to u eqn 
  addCoeff1(cmpq,eqnv,coeff, teDt*gravity(1)*iCoeff)  ! add buoyancy to v eqn 

  if( isAxisymmetric.eq.1 )then
    ! add axisymmetric corrections 

    ! *** FIX ME FOR variable material properties ***
    if( materialFormat.ne.constantMaterialProperties )then
      stop 6206
    end if

    radi = radiusInverse(i1,i2,i3)
    if( radi.ne.0. )then
     addCoeff1(cmpq,eqnq,coeff, -kDt*radi*yCoeff)                        ! -nu*dt*( U_r/r )
    else
     ! corrections on the axis
     getCoeff(yy, yyCoeff,aj)
     addCoeff1(cmpq,eqnq,coeff, -kDt*yyCoeff)                            ! -nu*dt*( U_rr )
    end if
  end if

 #Else
  if( materialFormat.eq.constantMaterialProperties )then
    ! constant material properties:
    setCoeff6(cmpq,eqnq,coeff,iCoeff,-kDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,-adcBoussinesq*dt*dissCoeff)
  else
    ! get coefficients scaled by -dt*implicitFactor: 
    getDivGradCoefficientsTemperature(-dtImp)
    setDivTensorGradCoeff3d(cmpq,eqnq,au)
    addCoeff5(cmpq,eqnq,coeff,iCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,-adcBoussinesq*dt*dissCoeff)
  endif

  addCoeff1(cmpu,eqnq,coeff, bDt*q0x*iCoeff)  ! u*q0x 
  addCoeff1(cmpv,eqnq,coeff, bDt*q0y*iCoeff)  ! v*q0y 
  addCoeff1(cmpw,eqnq,coeff, bDt*q0z*iCoeff)  ! w*q0z 

  addCoeff1(cmpq,eqnu,coeff, teDt*gravity(0)*iCoeff)  ! add buoyancy to u eqn 
  addCoeff1(cmpq,eqnv,coeff, teDt*gravity(1)*iCoeff)  ! add buoyancy to v eqn 
  addCoeff1(cmpq,eqnw,coeff, teDt*gravity(2)*iCoeff)  ! add buoyancy to w eqn 

 #End

 endif  ! pdeModel.eq.BoussinesqModel

 endLoops()
end if 

end if
#endMacro

!   coeff(MCE(-1,-1,0,cmp,eqn),i1,i2,i3)=  a12mzz+a21zmz
!   coeff(MCE( 0,-1,0,cmp,eqn),i1,i2,i3)=                      a22mh 
!   coeff(MCE( 1,-1,0,cmp,eqn),i1,i2,i3)= -a12pzz-a21zmz 
!   coeff(MCE(-1, 0,0,cmp,eqn),i1,i2,i3)=         a11mh
!   coeff(MCE( 0, 0,0,cmp,eqn),i1,i2,i3)= -a11ph-a11mh -a22ph -a22mh
!   coeff(MCE( 1, 0,0,cmp,eqn),i1,i2,i3)=  a11ph
!   coeff(MCE(-1, 1,0,cmp,eqn),i1,i2,i3)= -a12mzz-a21zpz
!   coeff(MCE( 0, 1,0,cmp,eqn),i1,i2,i3)=               a22ph
!   coeff(MCE( 1, 1,0,cmp,eqn),i1,i2,i3)=  a12pzz+a21zpz

!  coeff(MCE(-1,-1,-1,cmp,eqn),i1,i2,i3)= 0.
!  coeff(MCE( 0,-1,-1,cmp,eqn),i1,i2,i3)=         A ## 23zmz+A ## 32zzm
!  coeff(MCE( 1,-1,-1,cmp,eqn),i1,i2,i3)= 0.
!  coeff(MCE(-1, 0,-1,cmp,eqn),i1,i2,i3)=  A ## 13mzz+A ## 31zzm
!  coeff(MCE( 0, 0,-1,cmp,eqn),i1,i2,i3)=                                    A ## 33mh
!  coeff(MCE( 1, 0,-1,cmp,eqn),i1,i2,i3)= -A ## 13pzz-A ## 31zzm
!  coeff(MCE(-1, 1,-1,cmp,eqn),i1,i2,i3)= 0.
!  coeff(MCE( 0, 1,-1,cmp,eqn),i1,i2,i3)=        -A ## 23zpz-A ## 32zzm
!  coeff(MCE( 1, 1,-1,cmp,eqn),i1,i2,i3)= 0.
!
!  coeff(MCE(-1,-1, 0,cmp,eqn),i1,i2,i3)=  A ## 12mzz+A ## 21zmz
!  coeff(MCE( 0,-1, 0,cmp,eqn),i1,i2,i3)=                      A ## 22mh 
!  coeff(MCE( 1,-1, 0,cmp,eqn),i1,i2,i3)= -A ## 12pzz-A ## 21zmz 
!  coeff(MCE(-1, 0, 0,cmp,eqn),i1,i2,i3)=         A ## 11mh
!  coeff(MCE( 0, 0, 0,cmp,eqn),i1,i2,i3)= -A ## 11ph-A ## 11mh -A ## 22ph -A ## 22mh -A ## 33ph -A ## 33mh
!  coeff(MCE( 1, 0, 0,cmp,eqn),i1,i2,i3)=  A ## 11ph
!  coeff(MCE(-1, 1, 0,cmp,eqn),i1,i2,i3)= -A ## 12mzz-A ## 21zpz
!  coeff(MCE( 0, 1, 0,cmp,eqn),i1,i2,i3)=               A ## 22ph
!  coeff(MCE( 1, 1, 0,cmp,eqn),i1,i2,i3)=  A ## 12pzz+A ## 21zpz
!
!  coeff(MCE(-1,-1, 1,cmp,eqn),i1,i2,i3)= 0.
!  coeff(MCE( 0,-1, 1,cmp,eqn),i1,i2,i3)=       -A ## 23zmz-A ## 32zzp
!  coeff(MCE( 1,-1, 1,cmp,eqn),i1,i2,i3)= 0.
!  coeff(MCE(-1, 0, 1,cmp,eqn),i1,i2,i3)= -A ## 13mzz-A ## 31zzp
!  coeff(MCE( 0, 0, 1,cmp,eqn),i1,i2,i3)=                            A ## 33ph
!  coeff(MCE( 1, 0, 1,cmp,eqn),i1,i2,i3)=  A ## 13pzz+A ## 31zzp
!  coeff(MCE(-1, 1, 1,cmp,eqn),i1,i2,i3)= 0.
!  coeff(MCE( 0, 1, 1,cmp,eqn),i1,i2,i3)=        A ## 23zpz+A ## 32zzp 
!  coeff(MCE( 1, 1, 1,cmp,eqn),i1,i2,i3)= 0.

! ===========================================================================================
! Macro to evaluate 
!         dsg = div (scalar grad) u(i1,i2,i3,c)
!
! This macro assumes that the coefficients A12mzz,A21zmz, ... have already been computed 
!
! Parameters:
!   dsg (output)
!   u 
!   c = component
!   A = prefix in the names of the coefficients: A12mzz,A21zmz, ...
! Implicit parameters:
!   $ORDER
!   $DIM 
! ===========================================================================================
#beginMacro evalDivScalarGrad( dsg, u, c, A )

#If $DIM eq "2"
 #If $ORDER eq "2" 
  ! 2D, order 2:
  dsg = \
  ( (A ## 12mzz+A ## 21zmz)*u(i1-1,i2-1,i3,c) + (A ## 22mh)*u(i1,i2-1,i3,c)+(-A ## 12pzz-A ## 21zmz)*u(i1+1,i2-1,i3,c)\
   +(A ## 11mh)*u(i1-1,i2,i3,c)+(-A ## 11ph-A ## 11mh -A ## 22ph -A ## 22mh)*u(i1,i2,i3,c)+(A ## 11ph)*u(i1+1,i2,i3,c)\
   +(-A ## 12mzz-A ## 21zpz)*u(i1-1,i2+1,i3,c)+(A ## 22ph)*u(i1,i2+1,i3,c)+(A ## 12pzz+A ## 21zpz)*u(i1+1,i2+1,i3,c) )
 #Else
   ! Finish me for higher order
   stop 1063
 #End

#Elif $DIM eq "3"

 #If $ORDER eq "2" 
  ! 3D, order 2:
  dsg = \
  ( ( A ## 23zmz+A ## 32zzm)*u(i1  ,i2-1,i3-1,c) \
   +( A ## 13mzz+A ## 31zzm)*u(i1-1,i2  ,i3-1,c) \
   +( A ## 33mh            )*u(i1  ,i2  ,i3-1,c) \
   +(-A ## 13pzz-A ## 31zzm)*u(i1+1,i2  ,i3-1,c) \
   +(-A ## 23zpz-A ## 32zzm)*u(i1  ,i2+1,i3-1,c) \
   +( A ## 12mzz+A ## 21zmz)*u(i1-1,i2-1,i3  ,c) \
   +( A ## 22mh            )*u(i1  ,i2-1,i3  ,c) \
   +(-A ## 12pzz-A ## 21zmz)*u(i1+1,i2-1,i3  ,c) \
   +( A ## 11mh            )*u(i1-1,i2  ,i3  ,c) \
   +(-A ## 11ph-A ## 11mh -A ## 22ph -A ## 22mh -A ## 33ph -A ## 33mh)*u(i1  ,i2  ,i3  ,c) \
   +( A ## 11ph            )*u(i1+1,i2  ,i3  ,c) \
   +(-A ## 12mzz-A ## 21zpz)*u(i1-1,i2+1,i3  ,c) \
   +( A ## 22ph            )*u(i1  ,i2+1,i3  ,c) \
   +( A ## 12pzz+A ## 21zpz)*u(i1+1,i2+1,i3  ,c) \
   +(-A ## 23zmz-A ## 32zzp)*u(i1  ,i2-1,i3+1,c) \
   +(-A ## 13mzz-A ## 31zzp)*u(i1-1,i2  ,i3+1,c) \
   +( A ## 33ph            )*u(i1  ,i2  ,i3+1,c) \
   +( A ## 13pzz+A ## 31zzp)*u(i1+1,i2  ,i3+1,c) \
   +( A ## 23zpz+A ## 32zzp)*u(i1  ,i2+1,i3+1,c) )
 #Else
   ! Finish me for higher order
   stop 1063
 #End
#End
#endMacro


! ===================================================================================
! Macro to evaluate the RHS for the INS equations
! ===================================================================================
#beginMacro assignRHSPDE()
if( evalRightHandSide.eq.1 .or. evalResidual.eq.1 )then

! **** to do : optimize this for backward-Euler : fe=0, fi=0 !!

! NOTE: For moving grid problems we must eval the RHS as some mask==0 (exposed) points
beginLoopsNoMask()
  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,1)

  ! evaluate forward derivatives of the current solution: 

  ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
  ! MAXDER = max number of parametric derivatives to precompute.
  opEvalParametricDerivative(u,uc,uu,2)
  ! Evaluate the spatial derivatives of u:
  getOp(x, u,uc,uu,aj,u0x)       ! u.x
  getOp(y, u,uc,uu,aj,u0y)       ! u.y

  getOp(xx, u,uc,uu,aj,u0xx)       ! u.xx
  getOp(yy, u,uc,uu,aj,u0yy)       ! u.yy

  ! Evaluate the spatial derivatives of v:
  opEvalParametricDerivative(u,vc,vv,2)
  getOp(x, u,vc,vv,aj,v0x)       ! v.x
  getOp(y, u,vc,vv,aj,v0y)       ! v.y 

  getOp(xx, u,vc,vv,aj,v0xx)       ! v.xx
  getOp(yy, u,vc,vv,aj,v0yy)       ! v.yy

  ! Evaluate the spatial derivatives of p:
  opEvalParametricDerivative(u,pc,pp,1)
  getOp(x, u,pc,pp,aj,p0x)       ! p.x
  getOp(y, u,pc,pp,aj,p0y)       ! p.y 

 #If $DIM == 3

  getOp(z, u,uc,uu,aj,u0z)       ! u.z
  getOp(z, u,vc,vv,aj,v0z)       ! v.z
  getOp(z, u,pc,pp,aj,p0z)       ! p.z

  getOp(zz, u,uc,uu,aj,u0zz)       ! u.zz
  getOp(zz, u,vc,vv,aj,v0zz)       ! v.zz

  opEvalParametricDerivative(u,wc,ww,2)
  getOp(x, u,wc,ww,aj,w0x)       ! w.x
  getOp(y, u,wc,ww,aj,w0y)       ! w.y
  getOp(z, u,wc,ww,aj,w0z)       ! w.z

  getOp(xx, u,wc,ww,aj,w0xx)       ! w.xx
  getOp(yy, u,wc,ww,aj,w0yy)       ! w.yy
  getOp(zz, u,wc,ww,aj,w0zz)       ! w.zz

 #End

 if( evalLinearizedDerivatives.eq.1 )then

  #If $DIM == 2
   opEvalParametricDerivative(ul,uc,uul,1)
   getOp(x, ul,uc,uul,aj,ulx)       ! ul.x
   getOp(y, ul,uc,uul,aj,uly)       ! ul.y
 
   opEvalParametricDerivative(ul,vc,vvl,1)
   getOp(x, ul,vc,vvl,aj,vlx)       ! vl.x
   getOp(y, ul,vc,vvl,aj,vly)       ! vl.y
  #Else
   opEvalParametricDerivative(ul,uc,uul,1)
   getOp(x, ul,uc,uul,aj,ulx)       ! ul.x
   getOp(y, ul,uc,uul,aj,uly)       ! ul.y
   getOp(z, ul,uc,uul,aj,ulz)       ! ul.y
   opEvalParametricDerivative(ul,vc,vvl,1)
   getOp(x, ul,vc,vvl,aj,vlx)       ! vl.x
   getOp(y, ul,vc,vvl,aj,vly)       ! vl.y
   getOp(z, ul,vc,vvl,aj,vlz)       ! vl.y
   opEvalParametricDerivative(ul,wc,wwl,1)
   getOp(x, ul,wc,wwl,aj,wlx)       ! wl.x
   getOp(y, ul,wc,wwl,aj,wly)       ! wl.y
   getOp(z, ul,wc,wwl,aj,wlz)       ! wl.y
  #End
 end if

 if( pdeModel.eq.BoussinesqModel )then
  ! Evaluate the spatial derivatives of q:
  opEvalParametricDerivative(u,qc,qq,2)
  getOp(x, u,qc,qq,aj,q0x)       ! q.x
  getOp(y, u,qc,qq,aj,q0y)       ! q.y 
  getOp(xx,u,qc,qq,aj,q0xx)       ! q.xx
  getOp(yy,u,qc,qq,aj,q0yy)       ! q.yy
  #If $DIM == 3
   getOp(z, u,qc,qq,aj,q0z)       ! q.z  *wdh* 080720
   getOp(zz,u,qc,qq,aj,q0zz)      ! q.zz
  #End
 end if

 if( gridIsMoving.ne.0 )then
   ugv = uu - gv(i1,i2,i3,0)
   vgv = vv - gv(i1,i2,i3,1)
  #If $DIM == 3
   wgv = ww - gv(i1,i3,i3,2)
  #End
 else
   ugv = uu
   vgv = vv
  #If $DIM == 3
   wgv = ww
  #End
 end if

 ! eval the nonlinear coeff. of artificial dissipation: 
 getArtificialDissipationCoeff(adCoeff, u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z )

 #If $DIM == 2
 if( evalRightHandSide.eq.1 )then


  if( gridIsImplicit.eq.0 )then

    ! ********** explicit *********
    fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y - p0x + nu*(u0xx+u0yy) + adCoeff*uDiss22(u,uc)
    fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y - p0y + nu*(v0xx+v0yy) + adCoeff*uDiss22(u,vc)

    if( isAxisymmetric.eq.1 )then
      radi=radiusInverse(i1,i2,i3)
      if( radi.ne.0. )then
        fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + nu*( u0y*radi )            ! add u_r/r 
        fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + nu*( (v0y -vv*radi)*radi ) ! add v_r/r - v/r^2 
      else
        fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + nu*( u0yy )    ! add u_rr 
        fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + nu*( .5*v0yy ) ! add .5*vrr
      end if
    end if


    if( pdeModel.eq.BoussinesqModel )then

     fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) -thermalExpansivity*gravity(0)*qq 
     fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) -thermalExpansivity*gravity(1)*qq 

     fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y + adcBoussinesq*uDiss22(u,qc)

     if( materialFormat.eq.constantMaterialProperties )then
       fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + kThermal*(q0xx+q0yy)
     else
      ! -- variable material properties : 
      ! evaluate dsgT = div( K grad (T) )/(rho*Cp)
      getDivGradCoefficientsTemperature(1.)
      evalDivScalarGrad( dsgT , u, qc, au )

      fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + dsgT
     endif

     if( isAxisymmetric.eq.1 )then
       if( materialFormat.ne.constantMaterialProperties )then
         stop 6644
       end if
       radi=radiusInverse(i1,i2,i3)
       if( radi.ne.0. )then
         fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + kThermal*( q0y*radi )            ! add u_r/r 
       else
         fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + kThermal*( q0yy )    ! add u_rr 
       end if
     end if

    end if

  else 
    ! ********** implicit *********

    ! eval the nonlinear coeff. of artificial dissipation for the linearized solution:
    getArtificialDissipationCoeff(adCoeffl, ulx,uly,ulz, vlx,vly,vlz, wlx,wly,wlz )

    ! implicit method -- compute explicit part
    fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y - p0x + (adCoeff-adCoeffl)*uDiss22(u,uc)
    fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y - p0y + (adCoeff-adCoeffl)*uDiss22(u,vc)
    if( nonlinearTermsAreImplicit.eq.1 )then
      ! include linearized terms u0*ulx + ul*u0x 

      if( gridIsMoving.ne.0 )then
       ugvl = uul - gvl(i1,i2,i3,0)
       vgvl = vvl - gvl(i1,i2,i3,1)
      else
       ugvl = uul
       vgvl = vvl
      end if

      ! ulterm = uul*u0x + vvl*u0y + bImp*(uu*ulx +  vv*uly)
      ! vlterm = uul*v0x + vvl*v0y + bImp*(uu*vlx +  vv*vly)

      ! moving: (u-gv)*ux ->  u*ux - gv*ux -> ul*ux +u*ulx +gvl*ux = (ul-gvl)*ux + ulx*u 

      ulterm = ugvl*u0x + vgvl*u0y + bImp*(uu*ulx +  vv*uly)
      vlterm = ugvl*v0x + vgvl*v0y + bImp*(uu*vlx +  vv*vly)

      ! ++ ulterm = uul*u0x + vvl*u0y + bImp*(uu*ulx )
      ! ++ vlterm = uul*v0x + vvl*v0y + bImp*(vv*vly)
      ! ++ ulterm = uul*u0x + vvl*u0y + bImp*( vv )
      ! ++ ulterm = uul*u0x + vvl*u0y 
      ! ++ vlterm = uul*v0x + vvl*v0y 

      fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + ulterm
      fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + vlterm
    end if

    if( implicitOption.eq.computeImplicitTermsSeparately )then
      ! implicit method -- compute implicit part 
      fi(i1,i2,i3,uc) = nuImp*(u0xx+u0yy)+ adCoeffl*uDiss22(u,uc)   !  I think nuImp==nu now
      fi(i1,i2,i3,vc) = nuImp*(v0xx+v0yy)+ adCoeffl*uDiss22(u,vc) 
      if( nonlinearTermsAreImplicit.eq.1 )then
        ! include linearized terms u0*ulx + ul*u0x 
        fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) - aImp*( ulterm )
        fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) - aImp*( vlterm )
      end if
      if( isAxisymmetric.eq.1 )then
       radi=radiusInverse(i1,i2,i3)
       if( radi.ne.0. )then
         fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) + nuImp*( u0y*radi )            ! add u_r/r 
         fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) + nuImp*( (v0y -vv*radi)*radi ) ! add v_r/r - v/r^2 
       else
         fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) + nuImp*( u0yy )    ! add u_rr 
         fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) + nuImp*( .5*v0yy ) ! add .5*vrr
       end if
      end if

    end if

    if( pdeModel.eq.BoussinesqModel )then

     ! tImp=1 if the buoyancy term is implicit, 0 if explicit
     fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) -(1.-tImp)*thermalExpansivity*gravity(0)*qq 
     fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) -(1.-tImp)*thermalExpansivity*gravity(1)*qq 
     fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y 
 
     if( nonlinearTermsAreImplicit.eq.1 )then
       ! include linearized terms u0*ulx + ul*u0x 
 
       opEvalParametricDerivative(ul,qc,qql,1)
       getOp(x, ul,qc,qql,aj,qlx)       ! ql.x
       getOp(y, ul,qc,qql,aj,qly)       ! ql.y
 
       qlterm = ugvl*q0x + vgvl*q0y + bImp*(uu*qlx +  vv*qly)
       fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + qlterm
     end if
 
     if( implicitOption.eq.computeImplicitTermsSeparately )then
       ! implicit method -- compute implicit part 
       fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) -tImp*thermalExpansivity*gravity(0)*qq 
       fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) -tImp*thermalExpansivity*gravity(1)*qq 

       fi(i1,i2,i3,qc) = adcBoussinesq*uDiss22(u,qc)  
       if( materialFormat.eq.constantMaterialProperties )then
         fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) + kThermal*(q0xx+q0yy)
       else
        ! -- variable material properties : 
        ! evaluate dsgT = div( K grad (T) )/(rho*Cp)
        getDivGradCoefficientsTemperature(1.)
        evalDivScalarGrad( dsgT , u, qc, au )
        fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) + dsgT
       endif
       if( nonlinearTermsAreImplicit.eq.1 )then
         ! include linearized terms u0*ulx + ul*u0x 
         fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) - aImp*( qlterm )
       end if
       if( isAxisymmetric.eq.1 )then
        radi=radiusInverse(i1,i2,i3)
        if( radi.ne.0. )then
          fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) + kThermal*( q0y*radi ) 
        else
          fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) + kThermal*( q0yy )    
        end if
       end if
     end if

    end if

  end if

 end if

 if( evalResidual.eq.1 )then
   ! residual in 2D: (NOTE: currently ul is not available when evaluating the residual)
   fe(i1,i2,i3,uc) = fi(i1,i2,i3,uc) - ugv*u0x -vgv*u0y - p0x + nu*(u0xx+u0yy)+ adCoeff*uDiss22(u,uc)
   fe(i1,i2,i3,vc) = fi(i1,i2,i3,vc) - ugv*v0x -vgv*v0y - p0y + nu*(v0xx+v0yy)+ adCoeff*uDiss22(u,vc)
   if( isAxisymmetric.eq.1 )then
     radi=radiusInverse(i1,i2,i3)
     if( radi.ne.0. )then
       fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + nu*( u0y*radi )            ! add u_r/r 
       fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + nu*( (v0y -vv*radi)*radi ) ! add v_r/r - v/r^2 
     else
       fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + nu*( u0yy )    ! add u_rr 
       fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + nu*( .5*v0yy ) ! add .5*vrr
     end if
   end if
   if( pdeModel.eq.BoussinesqModel )then

    fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) -thermalExpansivity*gravity(0)*qq 
    fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) -thermalExpansivity*gravity(1)*qq 
    fe(i1,i2,i3,qc) = fi(i1,i2,i3,qc) - ugv*q0x -vgv*q0y  + adcBoussinesq*uDiss22(u,qc)
    if( materialFormat.eq.constantMaterialProperties )then
      fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + kThermal*(q0xx+q0yy)
    else
     ! -- variable material properties : 
     ! evaluate dsgT = div( K grad (T) )/(rho*Cp)
     getDivGradCoefficientsTemperature(1.)
     evalDivScalarGrad( dsgT , u, qc, au )
     fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + dsgT
    endif

    if( isAxisymmetric.eq.1 )then
     radi=radiusInverse(i1,i2,i3)
     if( radi.ne.0. )then
       fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + kThermal*( q0y*radi )  ! add u_r/r 
     else
       fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + kThermal*( q0yy )      ! add u_rr 
     end if
    end if
   end if
 end if

 #Else

  ! ---- 3D -----

 if( evalRightHandSide.eq.1 )then


  if( gridIsImplicit.eq.0 )then
    ! explicit
    fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y -wgv*u0z - p0x+ nu*(u0xx+u0yy+u0zz) + adCoeff*uDiss23(u,uc)
    fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y -wgv*v0z - p0y+ nu*(v0xx+v0yy+v0zz) + adCoeff*uDiss23(u,vc)
    fe(i1,i2,i3,wc) = -ugv*w0x -vgv*w0y -wgv*w0z - p0z+ nu*(w0xx+w0yy+w0zz) + adCoeff*uDiss23(u,wc) 

    if( pdeModel.eq.BoussinesqModel )then
     fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) -thermalExpansivity*gravity(0)*qq
     fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) -thermalExpansivity*gravity(1)*qq
     fe(i1,i2,i3,wc) = fe(i1,i2,i3,wc) -thermalExpansivity*gravity(2)*qq
     fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y -wgv*q0z + adcBoussinesq*uDiss23(u,qc)

     if( materialFormat.eq.constantMaterialProperties )then
       fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + kThermal*(q0xx+q0yy+q0zz)
     else
      ! -- variable material properties : 
      ! evaluate dsgT = div( K grad (T) )/(rho*Cp)
      getDivGradCoefficientsTemperature(1.)
      evalDivScalarGrad( dsgT , u, qc, au )

      fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + dsgT
     endif

    end if
  else 

    ! eval the nonlinear coeff. of artificial dissipation for the linearized solution:
    getArtificialDissipationCoeff(adCoeffl, ulx,uly,ulz, vlx,vly,vlz, wlx,wly,wlz )

    ! implicit method -- compute explicit part
    fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y -wgv*u0z - p0x + (adCoeff-adCoeffl)*uDiss23(u,uc)
    fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y -wgv*v0z - p0y + (adCoeff-adCoeffl)*uDiss23(u,vc)
    fe(i1,i2,i3,wc) = -ugv*w0x -vgv*w0y -wgv*w0z - p0z + (adCoeff-adCoeffl)*uDiss23(u,wc)

    if( nonlinearTermsAreImplicit.eq.1 )then
      ! include linearized terms u0*ulx + ul*u0x + ...
      if( gridIsMoving.ne.0 )then
       ugvl = uul - gvl(i1,i2,i3,0)
       vgvl = vvl - gvl(i1,i2,i3,1)
       wgvl = wwl - gvl(i1,i3,i3,2)
      else
       ugvl = uul
       vgvl = vvl
       wgvl = wwl
      end if
      ! ulterm = uul*u0x + vvl*u0y + bImp*(uu*ulx +  vv*uly)
      ulterm = ugvl*u0x + vgvl*u0y + wgvl*u0z + bImp*(uu*ulx + vv*uly + ww*ulz)
      vlterm = ugvl*v0x + vgvl*v0y + wgvl*v0z + bImp*(uu*vlx + vv*vly + ww*vlz)
      wlterm = ugvl*w0x + vgvl*w0y + wgvl*w0z + bImp*(uu*wlx + vv*wly + ww*wlz)

      fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + ulterm
      fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + vlterm
      fe(i1,i2,i3,wc) = fe(i1,i2,i3,wc) + wlterm
    end if

    if( implicitOption.eq.computeImplicitTermsSeparately )then
      ! implicit method -- compute implicit part 
      fi(i1,i2,i3,uc) = nuImp*(u0xx+u0yy+u0zz) + adCoeffl*uDiss23(u,uc)
      fi(i1,i2,i3,vc) = nuImp*(v0xx+v0yy+v0zz) + adCoeffl*uDiss23(u,vc) 
      fi(i1,i2,i3,wc) = nuImp*(w0xx+w0yy+w0zz) + adCoeffl*uDiss23(u,wc) 
      if( nonlinearTermsAreImplicit.eq.1 )then
        ! include linearized terms u0*ulx + ul*u0x 
        fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) - aImp*( ulterm )
        fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) - aImp*( vlterm )
        fi(i1,i2,i3,wc) = fi(i1,i2,i3,wc) - aImp*( wlterm )
      end if
    end if

    ! add Boussinesq terms 
    if( pdeModel.eq.BoussinesqModel )then   
     fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) -(1.-tImp)*thermalExpansivity*gravity(0)*qq 
     fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) -(1.-tImp)*thermalExpansivity*gravity(1)*qq 
     fe(i1,i2,i3,wc) = fe(i1,i2,i3,wc) -(1.-tImp)*thermalExpansivity*gravity(2)*qq 
     fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y -wgv*q0z 

     if( nonlinearTermsAreImplicit.eq.1 )then
       opEvalParametricDerivative(ul,qc,qql,1)
       getOp(x, ul,qc,qql,aj,qlx)       ! ql.x
       getOp(y, ul,qc,qql,aj,qly)       ! ql.y
       getOp(z, ul,qc,qql,aj,qlz)       ! ql.z
       qlterm = ugvl*q0x + vgvl*q0y + wgvl*q0z + bImp*(uu*qlx + vv*qly+ ww*qlz)
       fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + qlterm
     end if

     if( implicitOption.eq.computeImplicitTermsSeparately )then
      ! implicit method -- compute implicit part 
      fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) -tImp*thermalExpansivity*gravity(0)*qq 
      fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) -tImp*thermalExpansivity*gravity(1)*qq 
      fi(i1,i2,i3,wc) = fi(i1,i2,i3,wc) -tImp*thermalExpansivity*gravity(2)*qq 

      if( materialFormat.eq.constantMaterialProperties )then
        fi(i1,i2,i3,qc) = kThermal*(q0xx+q0yy+q0zz) + adcBoussinesq*uDiss23(u,qc)
      else
       ! -- variable material properties : 
       ! evaluate dsgT = div( K grad (T) )/(rho*Cp)
       getDivGradCoefficientsTemperature(1.)
       evalDivScalarGrad( dsgT , u, qc, au )
       fi(i1,i2,i3,qc) = dsgT + adcBoussinesq*uDiss23(u,qc)
      endif

      if( nonlinearTermsAreImplicit.eq.1 )then
        fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) - aImp*( qlterm )
      end if
     end if

    end if


  end if
 end if
 if( evalResidual.eq.1 )then
   ! residual in 3D: (NOTE: currently ul is not available when evaluating the residual)
   fe(i1,i2,i3,uc) = fi(i1,i2,i3,uc) -ugv*u0x -vgv*u0y -wgv*u0z - p0x + nu*(u0xx+u0yy+u0zz) + adCoeff*uDiss23(u,uc) 
   fe(i1,i2,i3,vc) = fi(i1,i2,i3,vc) -ugv*v0x -vgv*v0y -wgv*v0z - p0y + nu*(v0xx+v0yy+v0zz) + adCoeff*uDiss23(u,vc)
   fe(i1,i2,i3,wc) = fi(i1,i2,i3,wc) -ugv*w0x -vgv*w0y -wgv*w0z - p0z + nu*(w0xx+w0yy+w0zz) + adCoeff*uDiss23(u,wc)  
   if( pdeModel.eq.BoussinesqModel )then  
    fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) -thermalExpansivity*gravity(0)*qq
    fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) -thermalExpansivity*gravity(1)*qq 
    fe(i1,i2,i3,wc) = fe(i1,i2,i3,wc) -thermalExpansivity*gravity(2)*qq 
    fe(i1,i2,i3,qc) = fi(i1,i2,i3,qc) -ugv*q0x -vgv*q0y -wgv*q0z + adcBoussinesq*uDiss23(u,qc)
    if( materialFormat.eq.constantMaterialProperties )then
      fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) +kThermal*(q0xx+q0yy+q0zz)
    else
     ! -- variable material properties : 
     ! evaluate dsgT = div( K grad (T) )/(rho*Cp)
     getDivGradCoefficientsTemperature(1.)
     evalDivScalarGrad( dsgT , u, qc, au )
     fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + dsgT
    endif

   end if
 end if

 #End
endLoopsNoMask()
end if
#endMacro


! ===============================================================================================
!  Fill in the matrix BCs on a face for the INS equations 
! ===============================================================================================
#beginMacro fillMatrixBoundaryConditionsPDE()

if( fillCoefficients.eq.1 )then

if( fillCoefficientsScalarSystem.ne.fillCoeffT )then

  ! ---- fill BC coeffs for (u,v,w) ---

  bc0 = bc(side,axis)
  if( bc0.eq.slipWall .and. fillCoefficientsScalarSystem.gt.0 )then
    if( fillCoefficientsScalarSystem.eq.(axis+1) )then
      bc0=dirichletBoundaryCondition
    else
      bc0=neumannBoundaryCondition
    end if
  end if

if( bc0.eq.dirichletBoundaryCondition .or.\
    bc0.eq.noSlipWall .or.\
    bc0.eq.inflowWithVelocityGiven .or.\
    bc0.eq.interfaceBoundaryCondition )then
  
 ! Dirichlet BC
 beginLoopsMixedBoundary()

  ! zero out equations for u,v, [w]    ** but not the T eqn since these may be a Neumann BC ! **
  zeroMatrixCoefficients( coeff,eqnu,eqnu+ndu-1, i1,i2,i3 )

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
!   opEvalJacobianDerivatives(aj,1)

  ! evaluate the coeff operators 
  getCoeff(identity, iCoeff,aj)
!   getCoeff(x, xCoeff,aj)
!   getCoeff(y, yCoeff,aj)
  #If $DIM == 3
!    getCoeff(z, zCoeff,aj)
  #End

  do n=0,ndu-1
   setCoeff5(cmpu+n,eqnu+n,coeff,iCoeff,,,,)
  end do 

 endLoops()

else if( bc0.eq.outflow )then

 ! NOTE: outflowOption==0 is done below (extrapolation)
 if( outflowOption.eq.1 )then
  ! Neumann BC at outflow if outflowOption==1

   ! write(*,'("insImpINS: fill outflow BC into matrix: Neumann BC")')

  beginLoopsMixedBoundary()
   getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
   do n=0,ndu-1
     fillMatrixNeumann(coeff, cmpu+n,eqnu+n, i1,i2,i3, an,0.,1. )
   end do
  endLoops()
 else if( outflowOption.ne.0 )then
   write(*,'("insImpINS: fill outflow BC into matrix: ERROR: outflowOption=",i6)') outflowOption
 end if

else if( bc0.eq.neumannBoundaryCondition )then

  ! Neumann BC (used by slipWall and scalar systems)

  beginLoopsMixedBoundary()
   getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
   do n=0,ndu-1
     fillMatrixNeumann(coeff, cmpu+n,eqnu+n, i1,i2,i3, an,0.,1. )
   end do
  endLoops()

else if( bc0.eq.freeSurfaceBoundaryCondition )then

  ! **FREE SURFACE BCs *** 

  ! NEW way: proper free-surface conditions *wdh* Sept 30, 2017
  beginLoopsMixedBoundary()
    fillMatrixFreeSurface(coeff, cmpu,eqnu, i1,i2,i3, an )
  endLoops()

  ! Check for two adjacent free surface BCs -- not supported yet.
  ! Use extrapolation or compatibility at corner since equations are duplicate:
  !     u_x + v_y =0 
  !     u_y + v_x =0
  !Corner:
  !     u_xx = - v_xy = u_yy  -> u_xx - u_yy = 0 
  !     v_xx = - u_xy = v_yy  -> v_xx - v_yy = 0 
  axisp1 = mod(axis+1,nd)
  if( nd.eq.3 )then
    axisp2 = mod(axis+2,nd) 
  else
    axisp2=axisp1
  end if
  if( bc(0,axisp1).eq.freeSurfaceBoundaryCondition .or. bc(1,axisp1).eq.freeSurfaceBoundaryCondition .or. \
      bc(0,axisp2).eq.freeSurfaceBoundaryCondition .or. bc(1,axisp2).eq.freeSurfaceBoundaryCondition )then
    write(*,'("insImpINS: ERROR: two free surfaces meet at a corner -- not implemented -- fix me")') 
    stop 9099
  end if

else if( bc0.eq.slipWall )then

  ! SLIP-WALL

  ! NOTE: Here we assume the matrix already includes the interior equations on the boundary 

  ! NOTE: what about corners ???

  ! boundary values use:
  !    n.u = f
  !    tau.(Lu) = tau.g   (tangential component of the equations on the boundary
  ! To avoid a zero pivot we combine the above equations as
  !     (n.u) n + ( tau.(Lu) ) tau = n f + tau g 
  !
  ! OR:  ( tau.(Lu) ) tau = Lu - (n.(Lu)) n 
  !    (n.u) n + Lu - (n.(Lu)) n = n f +  g - (n.g) n 
  !       
 if( fillCoefficientsScalarSystem.ge.fillCoeffU .and. fillCoefficientsScalarSystem.le.fillCoeffW )then
   write(*,'(" insImpINS: slipWall BC not finished for scalar systems")')
   stop 8130
 end if

 getCoeff(identity, iCoeff,aj)

 beginLoopsMixedBoundary()

  getNormalForCurvilinearGrid(side,axis,i1,i2,i3)

  beginMatrixLoops(m1,m2,m3)

  ! Form the matrix for "n.u"  -->  nDot(mc3(m1,m2,m3,c)) = iCoeff(m1,m2,m3)*an(c-cmpu)
  !nDot=0
  !nDot(mc3(0,0,0,cmpu))=an(0)
  !nDot(mc3(0,0,0,cmpv))=an(1)
  !nDot(mc3(0,0,0,cmpw))=an(2)

  ! Form the matrix for "n.Lu"
#If $DIM == 2 
  nDotL(0)=an(0)*coeff(mce2(m1,m2,m3,cmpu,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce2(m1,m2,m3,cmpu,eqnv),i1,i2,i3)
  nDotL(1)=an(0)*coeff(mce2(m1,m2,m3,cmpv,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce2(m1,m2,m3,cmpv,eqnv),i1,i2,i3)
#Else
  nDotL(0)=an(0)*coeff(mce3(m1,m2,m3,cmpu,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce3(m1,m2,m3,cmpu,eqnv),i1,i2,i3)+\
           an(2)*coeff(mce3(m1,m2,m3,cmpu,eqnw),i1,i2,i3)
  nDotL(1)=an(0)*coeff(mce3(m1,m2,m3,cmpv,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce3(m1,m2,m3,cmpv,eqnv),i1,i2,i3)+\
           an(2)*coeff(mce3(m1,m2,m3,cmpv,eqnw),i1,i2,i3)
  nDotL(2)=an(0)*coeff(mce3(m1,m2,m3,cmpw,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce3(m1,m2,m3,cmpw,eqnv),i1,i2,i3)+\
           an(2)*coeff(mce3(m1,m2,m3,cmpw,eqnw),i1,i2,i3)
#End

  ! form the matrix for  Lu + [ (n.u) - (n.(Lu)) ] n 

  !  eqnu:  (n1*u1+n2*u2+n3*u3)*n1 + L1(u) - nDotL*n1    
  !  eqnv:  (n1*u1+n2*u2+n3*u3)*n2 + L2(u) - nDotL*n2
  !  eqnw:  (n1*u1+n2*u2+n3*u3)*n3 + L3(u) - nDotL*n3
  !  nDotL = L1(u)*n1 + L2(u)*n2 + L3(u)*n3 
  do e=eqnu,eqnu+nd-1
  do c=cmpu,cmpu+nd-1
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)= coeff(MCE(m1,m2,m3,c,e),i1,i2,i3) + \
       an(e-eqnu)*(iCoeff(MA(m1,m2,m3))*an(c-cmpu)-nDotL(c-cmpu))
  end do
  end do

 endMatrixLoops()

 ! fill ghost pt eqn's with a vector symmetry condition:
 fillMatrixVectorSymmetry(coeff, cmpu,eqnu, i1,i2,i3, an )

 endLoops()

else if( bc0.eq.inflowWithPandTV )then

  ! ------------------------------------------------
  ! ---- pressure and tangential velocity given ----
  ! ------------------------------------------------

 write(*,'("insImpINS: fill in BC pressure and tangential velocity ** check me** ")') 
 if( fillCoefficientsScalarSystem.ge.fillCoeffU .and. fillCoefficientsScalarSystem.le.fillCoeffW )then

  write(*,'(" fillCoefficientsScalarSystem=",i4)') fillCoefficientsScalarSystem

  ! --- fill coefficients for scalar systems ---
  ! This only works if the boundary face is on a plane x=constant, y=constant or z=constant 
  ! Added May 13, 2017 *wdh*
  beginLoopsMixedBoundary()
 
   opEvalJacobianDerivatives(aj,1)
   getCoeff(identity, iCoeff,aj)

   getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
   ! -- The grid face should be in a coordinate direction,
   !   normalAxis = 0,1, or 2 indicates this direction
   if( abs(abs(an(0))-1.) .lt. normalTol )then
    normalAxis=0
   else if( abs(abs(an(1))-1.) .lt. normalTol )then
    normalAxis=1
   else if(  abs(abs(an(2))-1.) .lt. normalTol )then
    normalAxis=2
   else
     write(*,'(" insImpINS: ERROR: inflowWithPandTV, scalar systems but normals funny")')
     write(*,'("  --> the normals should be in a coordinate direction")')
     stop 1287
   end if

   !  --- equations for u ---
   if( fillCoefficientsScalarSystem.eq.fillCoeffU )then
     if( normalAxis.eq.0 )then 
      ! boundary face is x=constant:
      !  Give u.n = 0 
      getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
      fillMatrixNeumann(coeff, cmpu,eqnu, i1,i2,i3, an,0.,1. )  ! Neuman: U_r = 
     else
      ! boundary face is y=constant, or z=constant : give u=0 
      zeroMatrixCoefficients( coeff,eqnu,eqnu, i1,i2,i3 )  ! set u eqn coeffs to zero
      setCoeff1(cmpu,eqnu,coeff,iCoeff)                    ! dirichlet: U= 
      fillMatrixExtrapolation(coeff,cmpu,eqnu,i1,i2,i3,orderOfExtrapolation,1)  ! extrap ghost for U 
     end if

   !  --- equations for v ---
   else if( fillCoefficientsScalarSystem.eq.fillCoeffV )then
     if( normalAxis.eq.1 )then
      ! boundary face is y=constant:
      !  Give v.n = 0 
      getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
      fillMatrixNeumann(coeff, cmpv,eqnv, i1,i2,i3, an,0.,1. )  ! Neuman: V_r = 
     else
      ! boundary face is x=constant, or z=constant : give v=0 
      zeroMatrixCoefficients( coeff,eqnv,eqnv, i1,i2,i3 )  ! set v eqn coeffs to zero
      setCoeff1(cmpv,eqnv,coeff,iCoeff)                    ! dirichlet: V= 
      fillMatrixExtrapolation(coeff,cmpv,eqnv,i1,i2,i3,orderOfExtrapolation,1)  ! extrap ghost for V 
     end if

   !  --- equations for w ---
   else if( fillCoefficientsScalarSystem.eq.fillCoeffW )then
     if( normalAxis.eq.2 )then
      ! boundary face is z=constant:
      !  Give w.n = 0 
      getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
      fillMatrixNeumann(coeff, cmpw,eqnw, i1,i2,i3, an,0.,1. )  ! Neuman: W_r = 
     else
      ! boundary face is x=constant, or y=constant : give w=0 
      zeroMatrixCoefficients( coeff,eqnw,eqnw, i1,i2,i3 )  ! set w eqn coeffs to zero
      setCoeff1(cmpw,eqnw,coeff,iCoeff)                    ! dirichlet: W= 
      fillMatrixExtrapolation(coeff,cmpw,eqnw,i1,i2,i3,orderOfExtrapolation,1)  ! extrap ghost for W 
     end if

   else
     write(*,'(" insImpINS: bc0.eq.inflowWithPandTV -- unknown option")')
     stop 8141
   end if
 
  endLoops()


else
  ! ****** inflowWithPandTV: vector system *********

 getCoeff(identity, iCoeff,aj)

 beginLoopsMixedBoundary()

  getNormalForCurvilinearGrid(side,axis,i1,i2,i3)

  beginMatrixLoops(m1,m2,m3)

  ! Form the matrix for "n.Lu"
#If $DIM == 2 
   nDotL(0)=an(0)*coeff(mce2(m1,m2,m3,cmpu,eqnu),i1,i2,i3)+\
            an(1)*coeff(mce2(m1,m2,m3,cmpu,eqnv),i1,i2,i3)
   nDotL(1)=an(0)*coeff(mce2(m1,m2,m3,cmpv,eqnu),i1,i2,i3)+\
            an(1)*coeff(mce2(m1,m2,m3,cmpv,eqnv),i1,i2,i3)
#Else
   nDotL(0)=an(0)*coeff(mce3(m1,m2,m3,cmpu,eqnu),i1,i2,i3)+\
            an(1)*coeff(mce3(m1,m2,m3,cmpu,eqnv),i1,i2,i3)+\
            an(2)*coeff(mce3(m1,m2,m3,cmpu,eqnw),i1,i2,i3)
   nDotL(1)=an(0)*coeff(mce3(m1,m2,m3,cmpv,eqnu),i1,i2,i3)+\
            an(1)*coeff(mce3(m1,m2,m3,cmpv,eqnv),i1,i2,i3)+\
            an(2)*coeff(mce3(m1,m2,m3,cmpv,eqnw),i1,i2,i3)
   nDotL(2)=an(0)*coeff(mce3(m1,m2,m3,cmpw,eqnu),i1,i2,i3)+\
            an(1)*coeff(mce3(m1,m2,m3,cmpw,eqnv),i1,i2,i3)+\
           an(2)*coeff(mce3(m1,m2,m3,cmpw,eqnw),i1,i2,i3)
#End

   ! form the matrix for  Iu + [ (n.(Lu)) - (n.u) ] n 

   !  eqnu:     u1 + nDotL*n1 - (n1*u1+n2*u2+n3*u3)*n1 
   !  eqnv:     u2 + nDotL*n2 - (n1*u1+n2*u2+n3*u3)*n2
   !  eqnw:     u3 + nDotL*n3 - (n1*u1+n2*u2+n3*u3)*n3
   !  nDotL = L1(u)*n1 + L2(u)*n2 + L3(u)*n3 
   do e=eqnu,eqnu+nd-1
   do c=cmpu,cmpu+nd-1
    ! *** check this ***
    coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)= iCoeff(MA(m1,m2,m3))*delta(c-cmpu,e-eqnu) + \
        an(e-eqnu)*( nDotL(c-cmpu) - iCoeff(MA(m1,m2,m3))*an(c-cmpu) )
   end do
   end do

  endMatrixLoops()

  ! Neumann condition for ghost: **** is this right? or extrapolate ?? ****
  do n=0,ndu-1
    fillMatrixNeumann(coeff, cmpu+n,eqnu+n, i1,i2,i3, an,0.,1. )
  end do

 endLoops()

end if  ! end vector system 

else if( bc0.eq.axisymmetric )then

 if( fillCoefficientsScalarSystem.eq.fillCoeffU )then

  ! BC on an axisymmetric side : scalar matrix for U 
  beginLoopsMixedBoundary()
 
   ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
   ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
   opEvalJacobianDerivatives(aj,1)

   ! getCoeff(identity, iCoeff,aj)
   !zeroMatrixCoefficients( coeff,eqnv,eqnv, i1,i2,i3 )  ! set v eqn coeffs to zero
   !setCoeff1(cmpv,eqnv,coeff,iCoeff)                ! dirichlet: V= 
   !fillMatrixExtrapolation(coe<ff,cmpv,eqnv,i1,i2,i3,orderOfExtrapolation,1)  ! extrap ghost for V 
 
   getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
   fillMatrixNeumann(coeff, cmpu,eqnu, i1,i2,i3, an,0.,1. )  ! Neuman: U_r = 
 
  endLoops()

 else if( fillCoefficientsScalarSystem.eq.fillCoeffV )then

  ! BC on an axisymmetric side : scalar matrix for V 
  beginLoopsMixedBoundary()
 
   ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
   ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
   opEvalJacobianDerivatives(aj,1)
   getCoeff(identity, iCoeff,aj)
 
   zeroMatrixCoefficients( coeff,eqnv,eqnv, i1,i2,i3 )  ! set v eqn coeffs to zero
   setCoeff1(cmpv,eqnv,coeff,iCoeff)                ! dirichlet: V= 
   fillMatrixExtrapolation(coeff,cmpv,eqnv,i1,i2,i3,orderOfExtrapolation,1)  ! extrap ghost for V 
 
   !getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
   !fillMatrixNeumann(coeff, cmpu,eqnu, i1,i2,i3, an,0.,1. )  ! Neuman: U_r = 
 
  endLoops()

 else

  ! BC on an axisymmetric side  
  beginLoopsMixedBoundary()
 
   ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
   ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
   opEvalJacobianDerivatives(aj,1)
   getCoeff(identity, iCoeff,aj)
 
   zeroMatrixCoefficients( coeff,eqnv,eqnv, i1,i2,i3 )  ! set v eqn coeffs to zero
   setCoeff1(cmpv,eqnv,coeff,iCoeff)                ! dirichlet: V= 
   fillMatrixExtrapolation(coeff,cmpv,eqnv,i1,i2,i3,orderOfExtrapolation,1)  ! extrap ghost for V 
 
   getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
   fillMatrixNeumann(coeff, cmpu,eqnu, i1,i2,i3, an,0.,1. )  ! Neuman: U_r = 
 
  endLoops()
 end if
else if( bc0.gt.0 )then
  write(*,'("insimp:BC: ERROR unknown bc=",i4)') bc0
  stop 5501
end if

if( bc0.eq.dirichletBoundaryCondition .or.\
    bc0.eq.noSlipWall.or.\
    bc0.eq.inflowWithVelocityGiven .or.\
    (bc0.eq.outflow .and. outflowOption.eq.0) .or.\
    bc0.eq.interfaceBoundaryCondition )then

 ! === extrapolation ===

 orderOfExtrap=orderOfExtrapolation
 if( bc0.eq.outflow .and. orderOfExtrapolationForOutflow.gt.0 )then
   orderOfExtrap=orderOfExtrapolationForOutflow
 end if
 if( orderOfExtrap.lt.1 .or. orderOfExtrap.gt.maxOrderOfExtrapolation )then
  write(*,'("insimp:BC:ERROR: requesting orderOfExtrap=",i6)') orderOfExtrap
  stop 5502
 end if

 if( .false. .and. bc0.eq.outflow )then
  write(*,'("insimp:BC: fill extrap outflow BC into matrix, orderOfExtrap,orderOfExtrapolationForOutflow=",2i4)') orderOfExtrap,orderOfExtrapolationForOutflow
 end if

 ! write(*,'("insimp:BC: orderOfExtrap,orderOfExtrapolationForOutflow=",2i4)') orderOfExtrap,orderOfExtrapolationForOutflow

 beginLoopsMixedBoundary()

  ! i1m=i1-is1  ! ghost point
  ! i2m=i2-is2
  ! i3m=i3-is3
  ! zeroMatrixCoefficients( coeff,eqnu,eqnu+ndu-1, i1,i2,i3 )

  do n=0,ndu-1
   c=cmpu+n
   e=eqnu+n
   fillMatrixExtrapolation(coeff,c,e,i1,i2,i3,orderOfExtrap,1)
  end do

 endLoops()
end if  
end if


if( pdeModel.eq.BoussinesqModel .and. (fillCoefficientsScalarSystem.eq.0 .or. fillCoefficientsScalarSystem.eq.fillCoeffT) )then  
 ! ----------------------------
 ! --- Assign the BCs for T --- (this is duplicated from insImpVP.bf : we could share 
 ! ----------------------------
 bc0 = bc(side,axis)

 a0 = mixedCoeff(tc,side,axis,grid)
 a1 = mixedNormalCoeff(tc,side,axis,grid)
 if( debug.gt.3 )then
   write(*,'(" insImpINS: T BC: bc=",i3," (a0,a1)=(",f5.1,",",f5.1,") for side,axis,grid=",3i3)') \
    bc0,a0,a1,side,axis,grid
   write(*,'(" cmpu,eqnu=",2i2," cmpq,eqnq=",2i2," orderOfExtrap=",i2)') cmpu,eqnu, cmpq,eqnq,orderOfExtrap
 endif
 ! '
 if( bc0.eq.dirichletBoundaryCondition .or.\
     bc0.eq.noSlipWall .or.\
     bc0.eq.slipWall .or.\
     bc0.eq.inflowWithVelocityGiven .or.\
     bc0.eq.outflow .or.\
     bc0.eq.axisymmetric .or.\
     bc0.eq.interfaceBoundaryCondition .or. \
     bc0.eq.freeSurfaceBoundaryCondition )then

   if( bc0.eq.outflow .or. bc0.eq.axisymmetric )then
    ! outflow is Neumann
    a0=0.
    a1=1.
   end if

  if( a1.ne.0. )then
   ! Mixed BC 
   beginLoopsMixedBoundary()
    getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
    fillMatrixNeumann(coeff, cmpq,eqnq, i1,i2,i3, an,a0,a1 )
   endLoops()
  else
   ! Dirichlet + extrap ghost line values


   getCoeff(identity, iCoeff,aj)
   beginLoopsMixedBoundary()
    zeroMatrixCoefficients( coeff,eqnq,eqnq,i1,i2,i3 )
    setCoeff5(cmpq,eqnq,coeff,a0*iCoeff,,,,)
    fillMatrixExtrapolation(coeff,cmpq,eqnq,i1,i2,i3,orderOfExtrapolation,1)
   endLoops()
  end if

!  else if( bc0.eq.outflow )then
!    ! === extrapolation ===
!   beginLoopsMixedBoundary()
!    fillMatrixExtrapolation(coeff,cmpq,eqnq,i1,i2,i3,orderOfExtrap,1)
!   endLoops()

 else if( bc0.gt.0 )then
  write(*,'("insImpINS:T: ERROR unknown bc=",i4)') bc0 
  stop 9167
 end if 
endif

end if ! end if( fillCoefficients.eq.1 )
#endMacro


! ===============================================================================================
!  Compute the residual for BCs
! ===============================================================================================
#beginMacro getBoundaryResidualPDE()
if( evalResidualForBoundaryConditions.eq.1 )then

if( bc(side,axis).eq.dirichletBoundaryCondition .or.\
    bc(side,axis).eq.noSlipWall.or.\
    bc(side,axis).eq.inflowWithVelocityGiven.or.\
    bc(side,axis).eq.interfaceBoundaryCondition )then
  
 ! Dirichlet BC
 beginLoops()

  ! do n=0,ntdc-1             ! ntdc = number of time depenedent components
  do n=0,nd-1  ! ntdc = number of time depenedent components
   ! fe(i1,i2,i3,uc+n)=0.  
   fe(i1,i2,i3,uc+n)=fi(i1,i2,i3,uc+n)-u(i1,i2,i3,uc+n)
  end do 

 endLoops()

else if( bc(side,axis).eq.outflow )then

else if( bc(side,axis).eq.axisymmetric )then

! do nothing for now

else if( bc(side,axis).eq.freeSurfaceBoundaryCondition )then

! do nothing for now


else if( bc(side,axis).eq.slipWall )then
  ! SLIP-WALL
 
  ! boundary values use:
  !    n.u = f
  !    tau.(Lu) = tau.g   (tangential component of the equations on the boundary
  ! To avoid a zero pivot we combine the above equations as
  !     (n.u) n + ( tau.(Lu) ) tau = n f + tau g 
  !
  ! OR:  ( tau.(Lu) ) tau = Lu - (n.(Lu)) n 
  !    (n.u) n + Lu - (n.(Lu)) n = n f +  g - (n.g) n 
  !       

 beginLoops()

  do n=0,nd-1
   fe(i1,i2,i3,uc+n)=0.  ! *** do this for now 
  end do 

 endLoops()

else if( bc(side,axis).eq.inflowWithPandTV )then
 ! pressure and tangential velocity given
 beginLoops()

  do n=0,nd-1
   fe(i1,i2,i3,uc+n)=0.  ! *** do this for now 
  end do 

 endLoops()


else if( bc(side,axis).gt.0 )then

  write(*,'("insimp:residual:BC: ERROR unknown bc=",i4)') bc(side,axis)
  stop 9099

end if

end if ! end if( evalResidualForBoundaryConditions.eq.1 )
#endMacro


! ********* Here we now define the suboutine insImpINS ******************
INSIMP(insImpINS,INS)
