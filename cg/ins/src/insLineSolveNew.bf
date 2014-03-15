c***********************************************************************************************
c 
c   *NEW* Steady-state line-solver routines for the incompressible NS plus some turbulence models
c
c***********************************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

#Include "commonMacros.h"

! define the INITIALZE macro: 
#Include initLineSolveParameters.h 


c ** Include "defineSelfAdjointMacros.h"

c ************************ NEW WAY ************************

c These next include file will define the macros that will define the difference approximations (in op/src)
c Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
c #Include "derivMacroDefinitions.h"


c Define 
c    defineParametricDerivativeMacros(u,dr,DIM,ORDER,COMPONENTS,MAXDERIV)
c       defines -> ur2, us2, ux2, uy2, ...            (2D)
c                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
!* #Include "defineParametricDerivMacros.h"

! defineParametricDerivativeMacros(u,dr,DIM,ORDER,COMPONENTS,MAXDERIV)

!*  defineParametricDerivativeMacros(u,dr,3,2,1,2)
!*  defineParametricDerivativeMacros(rsxy,dr,3,2,2,1)

! Example to define orders 2,4,6: 
! defineParametricDerivativeMacros(u1,dr1,2,2,1,6)
! defineParametricDerivativeMacros(u1,dr1,2,4,1,4)
! defineParametricDerivativeMacros(u1,dr1,2,6,1,2)

! construct an include file that declares temporary variables:
!**#beginFile insLSdeclareTemporaryVariablesOrder2.h
!**      !  declareTemporaryVariables(DIM,MAXDERIV)   ! I think DIM and MAXDERIV are ignored for now
!**      declareTemporaryVariables(2,2)
!**      declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)
!**#endFile


! --- Macros to define INS and Temperature line-solve functions:---
!          fillEquationsRectangularGridINS
!          fillEquationsCurvilinearGridINS
#Include "lineSolveINS.h"

! --- Macros to define Visco Plastic line-solve functions:---
!     fillEquationsRectangularGridVP, computeResidualVP
#Include "lineSolveVP.h"

! --- Macros for the Spalart-Almaras turbulence model
#Include lineSolveSA.h

! --- Macros for the Baldwin-Lomax approx. 
! define computeBLNuT() :   Macro to compute Baldwin-Lomax Turbulent viscosity
#Include lineSolveBL.h


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

c ===========================================================================================
c dim : number of dimensions 2,3
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c equations e1,e2,...,e10 are for the matrix
c equations e11,e12,... are for the RHS
c ===========================================================================================
#beginMacro triLoops(dim,EQN, e1,e2,e3,e4,e5,e6,e7,e8,e9,e10, e11,e12,e13)
if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
   e1
   e2
   e3
   e4
   e5
   e6
   e7
   e8
   e9
   e10
c rhs equations:
   e11
   e12
   e13
  else 
c for interpolation points or unused:
   am(i1,i2,i3)=0.
   bm(i1,i2,i3)=1.
   cm(i1,i2,i3)=0.
#If #EQN == "TEMPERATURE"
   f(i1,i2,i3,fct)=uu(tc)
#Else
   f(i1,i2,i3,fcu)=uu(uc)
   f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
   f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
   f(i1,i2,i3,nc)=uu(nc)
#End
  end if
 end do
 end do
 end do

else if( computeMatrix.eq.1 )then

 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
   e1
   e2
   e3
   e4
   e5
   e6
   e7
   e8
   e9
   e10
  else 
c for interpolation points or unused:
   am(i1,i2,i3)=0.
   bm(i1,i2,i3)=1.
   cm(i1,i2,i3)=0.
  end if
 end do
 end do
 end do

else if( computeRHS.eq.1 )then

 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
   e11
   e12
   e13
  else 
c for interpolation points or unused:
#If #EQN == "TEMPERATURE"
   f(i1,i2,i3,fct)=uu(tc)
#Else
   f(i1,i2,i3,fcu)=uu(uc)
   f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
   f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
   f(i1,i2,i3,nc)=uu(nc)
#End

  end if
 end do
 end do
 end do
end if
#endMacro



c ===========================================================================================
c Loops: 4th-order version
c
c dim : number of dimensions 2,3
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c equations e1,e2,...,e10 are for the matrix
c equations e11,e12,... are for the RHS
c ===========================================================================================
#beginMacro triLoops4(dim,EQN, e1,e2,e3,e4,e5,e6,e7,e8,e9,e10, e11,e12,e13)
if( computeMatrix.eq.1 .and. computeRHS.eq.1 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
      e1
      e2
      e3
      e4
      e5
      e6
      e7
      e8
      e9
      e10
c rhs equations:
      e11
      e12
      e13
    else 
c for interpolation points or unused:
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=0.
      cm(i1,i2,i3)=1.
      dm(i1,i2,i3)=0.
      em(i1,i2,i3)=0.

#If #EQN == "TEMPERATURE"
      f(i1,i2,i3,fct)=uu(tc)
#Else
      f(i1,i2,i3,fcu)=uu(uc)
      f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
      f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
      f(i1,i2,i3,nc)=uu(nc)
#End
    end if
  end do
  end do
  end do

else if( computeMatrix.eq.1 )then

  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
c matrix equations:
      e1
      e2
      e3
      e4
      e5
      e6
      e7
      e8
      e9
      e10
    else 
c for interpolation points or unused:
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=0.
      cm(i1,i2,i3)=1.
      dm(i1,i2,i3)=0.
      em(i1,i2,i3)=0.
    end if
  end do
  end do
  end do

else if( computeRHS.eq.1 )then

  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
c rhs equations:
      e11
      e12
      e13
    else 
c for interpolation points or unused:
#If #EQN == "TEMPERATURE"
      f(i1,i2,i3,fct)=uu(tc)
#Else
      f(i1,i2,i3,fcu)=uu(uc)
      f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
      f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
      f(i1,i2,i3,nc)=uu(nc)
#End
    end if
  end do
  end do
  end do
end if
#endMacro





#beginMacro loops(e1,e2,e3, e4,e5,e6)
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
      e5
      e6
    end if
  end do
  end do
  end do
#endMacro

c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary
c* c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c* c ***********************************************************************
c* #beginMacro loopsBC(dim,EQN, e1,e2,e3,e4,e5,e6)
c*  do i3=n3a,n3b
c*  do i2=n2a,n2b
c*  do i1=n1a,n1b
c*   if( mask(i1,i2,i3).gt.0 )then
c*    e1
c*    e2
c*    e3
c*    e4
c*    e5
c*    e6
c*   else
c* c for interpolation points or unused:
c*    am(i1,i2,i3)=0.
c*    bm(i1,i2,i3)=1.
c*    cm(i1,i2,i3)=0.
c* #If #EQN == "TEMPERATURE"
c*    f(i1,i2,i3,fct)=uu(tc)
c* #Else
c*    f(i1,i2,i3,fcu)=uu(uc)
c*    f(i1,i2,i3,fcv)=uu(vc)
c* #If #dim == "3"
c*    f(i1,i2,i3,fcw)=uu(wc)
c* #End
c* #End
c* #If #EQN == "SA"
c*    f(i1,i2,i3,nc)=uu(nc)
c* #End      
c*   end if
c*  end do
c*  end do
c*  end do
c* #endMacro
c* 
c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary
c* c  SOLVER: INS, SPAL
c* c ***********************************************************************
c* #beginMacro loopsMatrixBC(SOLVER,e1,e2,e3,e4,e5,e6)
c* #If #SOLVER == "INS" || #SOLVER == "INSVP"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignINS )then
c*    loopsBC(2,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignINS )then
c*    loopsBC(3,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Elif #SOLVER == "SPAL"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC(2,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC(3,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Else
c*   stop 8862
c* #End
c* #endMacro
c* 
c* 
c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary, fourth-order version
c* c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c* c ***********************************************************************
c* #beginMacro loopsBC4(dim,EQN, e1,e2,e3,e4,e5,e6)
c*   do i3=n3a,n3b
c*   do i2=n2a,n2b
c*   do i1=n1a,n1b
c*     if( mask(i1,i2,i3).gt.0 )then
c*       e1
c*       e2
c*       e3
c*       e4
c*       e5
c*       e6
c*     else
c* c for interpolation points or unused:
c*       am(i1,i2,i3)=0.
c*       bm(i1,i2,i3)=0.
c*       cm(i1,i2,i3)=1.
c*       dm(i1,i2,i3)=0.
c*       em(i1,i2,i3)=0.
c*       am(i1-is1,i2-is2,i3-is3)=0.
c*       bm(i1-is1,i2-is2,i3-is3)=0.
c*       cm(i1-is1,i2-is2,i3-is3)=1.
c*       dm(i1-is1,i2-is2,i3-is3)=0.
c*       em(i1-is1,i2-is2,i3-is3)=0.
c* #If #EQN == "TEMPERATURE"
c*       f(i1,i2,i3,fct)=uu(tc)
c*       f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,i3-is3,tc)
c* #Else
c*       f(i1,i2,i3,fcu)=uu(uc)
c*       f(i1,i2,i3,fcv)=uu(vc)
c*       f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,i3-is3,uc)
c*       f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,i3-is3,vc)
c* #If #dim == "3"
c*       f(i1,i2,i3,fcw)=uu(wc)
c*       f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,i3-is3,wc)
c* #End
c* #End
c* #If #EQN == "SA"
c*       f(i1,i2,i3,nc)=uu(nc)
c*       f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,i3-is3,nc)
c* #End    
c*     end if
c*   end do
c*   end do
c*   end do
c* #endMacro
c* 
c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary -- fourth-order version
c* c  SOLVER: INS, SPAL
c* c ***********************************************************************
c* #beginMacro loopsMatrixBC4(SOLVER,e1,e2,e3,e4,e5,e6)
c* #If #SOLVER == "INS"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignINS )then
c*    loopsBC4(2,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC4(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignINS )then
c*    loopsBC4(3,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC4(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Elif #SOLVER == "SPAL"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC4(2,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC4(3,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Else
c*   stop 7715
c* #End
c* !  if( nd.eq.2 )then
c* !    if( option.eq.assignINS )then
c* !      loopsBC4(2,INS,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignTemperature )then
c* !      loopsBC4(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignSpalartAllmaras )then
c* !      loopsBC4(2,SA,e1,e2,e3, e4,e5,e6)
c* !    end if
c* !  else
c* !    if( option.eq.assignINS )then
c* !      loopsBC4(3,INS,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignTemperature )then
c* !      loopsBC4(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignSpalartAllmaras )then
c* !      loopsBC4(3,SA,e1,e2,e3, e4,e5,e6)
c* !    end if
c* !  end if
c* #endMacro


c Define the artificial diffusion coefficients
c gt should be R or C
c tb should be blank or SA (for Splarat-Allmaras)
#beginMacro defineArtificialDiffusionCoefficients(dim,dir,gt,tb)
  #If #dim == "2" 
    cdmz=admz ## gt ## tb(i1  ,i2  ,i3)
    cdpz=admz ## gt ## tb(i1+1,i2  ,i3)
    cdzm=adzm ## gt ## tb(i1  ,i2  ,i3)
    cdzp=adzm ## gt ## tb(i1  ,i2+1,i3)
    ! write(*,'(1x,''insLS:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
    ! cdmz=0.
    ! cdpz=0.
    ! cdzm=0.
    ! cdzp=0.
    cdDiag=cdmz+cdpz+cdzm+cdzp
    #If #dir == "0" 
      cdm=cdmz
      cdp=cdpz
    #Elif #dir == "1"
      cdm=cdzm
      cdp=cdzp
    #Else
      stop 1234
    #End
  #Elif #dim == "3"
    cdmzz=admzz ## gt ## tb(i1  ,i2  ,i3  )
    cdpzz=admzz ## gt ## tb(i1+1,i2  ,i3  )
    cdzmz=adzmz ## gt ## tb(i1  ,i2  ,i3  )
    cdzpz=adzmz ## gt ## tb(i1  ,i2+1,i3  )
    cdzzm=adzzm ## gt ## tb(i1  ,i2  ,i3  )
    cdzzp=adzzm ## gt ## tb(i1  ,i2  ,i3+1)
    cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
    #If #dir == "0" 
      cdm=cdmzz
      cdp=cdpzz
    #Elif #dir == "1"
      cdm=cdzmz
      cdp=cdzpz
    #Elif #dir == "2"
      cdm=cdzzm
      cdp=cdzzp
    #Else 
      stop 9876
    #End
  #Else
    stop 888
  #End
#endMacro



c=======================================================================
c Define the stuff needed for 2nd-order + 4th-order artificial dissipation
c define: adCoeff2, adCoeff4 and the inline macro ade(cc) (for the rhs)
c=======================================================================
#beginMacro defineAD24(ADTYPE,DIM,DIR)
 #If #DIM == "2"
  #If #DIR == "0"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)) \
              + adCoeff4*(-u(i1,i2-2,i3,cc)-u(i1,i2+2,i3,cc)+4.*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)))
  #Elif #DIR == "1"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)) \
              + adCoeff4*(-u(i1-2,i2,i3,cc)-u(i1+2,i2,i3,cc)+4.*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)))
  #Else
    stop 676
  #End

 #Elif #DIM == "3"

  #If #DIR == "0"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)) \
              + adCoeff4*(-u(i1,i2-2,i3,cc)-u(i1,i2+2,i3,cc)-u(i1,i2,i3-2,cc)-u(i1,i2,i3+2,cc)\
                      +4.*(u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)))
  #Elif #DIR == "1"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)) \
              + adCoeff4*(-u(i1-2,i2,i3,cc)-u(i1+2,i2,i3,cc)-u(i1,i2,i3-2,cc)-u(i1,i2,i3+2,cc)\
                      +4.*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2,i3-1,cc)+u(i1,i2,i3+1,cc)))
  #Elif #DIR == "2"
    #defineMacro ade(cc)  \
                adCoeff2*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)) \
              + adCoeff4*(-u(i1-2,i2,i3,cc)-u(i1+2,i2,i3,cc)-u(i1,i2-2,i3,cc)-u(i1,i2+2,i3,cc)\
                      +4.*(u(i1-1,i2,i3,cc)+u(i1+1,i2,i3,cc)+u(i1,i2-1,i3,cc)+u(i1,i2+1,i3,cc)))
  #Else
    stop 676
  #End

 #Else
   stop 677
 #End

#endMacro

c =======================================================================================
c =======================================================================================
#beginMacro defineSelfAdjointDiffusionCoefficients(DIM,DIR,UV,nuT)

#If #DIM == "2"

 #If #UV == "U"
  getSelfAdjointDiffusionCoefficients(UV,i1,i2,i3,nuT,cu,cv)
  #If #DIR == "0"
   dsam    = cu(-1,0)
   dsaDiag = cu( 0,0)
   dsap    = cu( 1,0)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Elif #DIR == "1"
   dsam    = cu( 0,-1)
   dsaDiag = cu( 0, 0)
   dsap    = cu( 0, 1)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Else
    stop 101
  #End
 #Elif #UV == "V"
   ! note reverse order of cu and cv :
   getSelfAdjointDiffusionCoefficients(UV,i1,i2,i3,nuT,cv,cu)
   dsam    = cv(-1,0)
   dsaDiag = cv( 0,0)
   dsap    = cv( 1,0)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Elif #DIR == "1"
   dsam    = cv( 0,-1)
   dsaDiag = cv( 0, 0)
   dsap    = cv( 0, 1)

   #defineMacro dsau() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
   #defineMacro dsav() \
        cu(-1,-1)*u(i1-1,i2-1,i3,uc)+cu(0,-1)*u(i1  ,i2-1,i3,uc)+cu(1,-1)*u(i1+1,i2-1,i3,uc)\
       +cu(-1, 0)*u(i1-1,i2  ,i3,uc)+cu(0, 0)*u(i1  ,i2  ,i3,uc)+cu(1, 0)*u(i1+1,i2  ,i3,uc)\
       +cu(-1, 1)*u(i1-1,i2+1,i3,uc)+cu(0, 1)*u(i1  ,i2+1,i3,uc)+cu(1, 1)*u(i1+1,i2+1,i3,uc)\
       +cv(-1,-1)*v(i1-1,i2-1,i3,vc)+cv(0,-1)*v(i1  ,i2-1,i3,vc)+cv(1,-1)*v(i1+1,i2-1,i3,vc)\
       +cv(-1, 0)*v(i1-1,i2  ,i3,vc)+cv(0, 0)*v(i1  ,i2  ,i3,vc)+cv(1, 0)*v(i1+1,i2  ,i3,vc)\
       +cv(-1, 1)*v(i1-1,i2+1,i3,vc)+cv(0, 1)*v(i1  ,i2+1,i3,vc)+cv(1, 1)*v(i1+1,i2+1,i3,vc)           
  #Else
    stop 101
  #End
 #Else
   stop 111
 #End

#Elif #DIM == "3"

  stop 777

#Else
  stop 123
#End

#endMacro





c ===================================================================================
c   ******* fillEquationsRectangularGrid ***********
c
c  SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE, INSVP
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsRectangularGrid(SOLVER,dir)

#If #SOLVER == "INS"
 fillEquationsRectangularGridINS(dir)
#Elif #SOLVER == "INS_TEMPERATURE"
 fillEquationsRectangularGridTemperature(dir)
#Elif #SOLVER == "INSVP"
 fillEquationsRectangularGridVP(dir)
#Else
 stop 7721
#End

#endMacro

c ===================================================================================
c  ******* fillEquationsCurvilinearGrid *********
c
c  SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE 
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsCurvilinearGrid(SOLVER,dir)

#If #SOLVER == "INS"
 fillEquationsCurvilinearGridINS(dir)
#Elif #SOLVER == "INS_TEMPERATURE"
 fillEquationsCurvilinearGridTemperature(dir)
#Elif #SOLVER == "INSVP"
 fillEquationsCurvilinearGridVP(dir)
#Elif
 stop 1843
#End

#endMacro







c ===========================================================
c SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE
c GRIDTYPE: rectangular, curvilinear
c ===========================================================
#beginMacro fillEquations(SOLVER)
 if( gridType.eq.rectangular )then
   ! *******************************************
   ! ************** rectangular  ***************
   ! *******************************************
   if( orderOfAccuracy.eq.2 )then
     if( dir.eq.0 )then
      fillEquationsRectangularGrid(SOLVER,0)
     else if( dir.eq.1 )then
      fillEquationsRectangularGrid(SOLVER,1)
    else ! dir.eq.2
      fillEquationsRectangularGrid(SOLVER,2)
    end if ! end dir
   else ! order==4
   end if
 else if( gridType.eq.curvilinear )then
   ! *******************************************
   ! ************** curvilinear  ***************
   ! *******************************************
   if( orderOfAccuracy.eq.2 )then
     if( dir.eq.0 )then
      fillEquationsCurvilinearGrid(SOLVER,0)
     else if( dir.eq.1 )then
      fillEquationsCurvilinearGrid(SOLVER,1)
    else ! dir.eq.2
      fillEquationsCurvilinearGrid(SOLVER,2)
    end if ! end dir
   else ! order==4
   end if
 else
   stop 111
 end if

#endMacro

#beginMacro assignDirichletFourthOrder()
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
 am(i1,i2,i3)=0.
 bm(i1,i2,i3)=0.
 cm(i1,i2,i3)=1.
 dm(i1,i2,i3)=0.
 em(i1,i2,i3)=0.
 am(i1-is1,i2-is2,i3-is3)=0.
 bm(i1-is1,i2-is2,i3-is3)=0.
 cm(i1-is1,i2-is2,i3-is3)=1.
 dm(i1-is1,i2-is2,i3-is3)=0.
 em(i1-is1,i2-is2,i3-is3)=0.
#endMacro

#beginMacro assignFourthOrder()
 am(i1,i2,i3)=cexa
 bm(i1,i2,i3)=cexb
 cm(i1,i2,i3)=cexc
 dm(i1,i2,i3)=cexd
 em(i1,i2,i3)=cexe
 am(i1-is1,i2-is2,i3-is3)=c4exa
 bm(i1-is1,i2-is2,i3-is3)=c4exb
 cm(i1-is1,i2-is2,i3-is3)=c4exc
 dm(i1-is1,i2-is2,i3-is3)=c4exd
 em(i1-is1,i2-is2,i3-is3)=c4exe
#endMacro



c ================================================================================
c Define the subroutine that builds the tridiagonal matrxix for a  given solver
c
c SOLVER: INS, INSSPAL, INSBL, INSVP
c=================================================================================
#beginMacro INS_LINE_SETUP(SOLVER,NAME)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
      md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw,\
      dir,am,bm,cm,dm,em,  bc, boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c 
c dw: distance to wall for SA TM
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
  md1a,md1b,md2a,md2b,md3a,md3b,dir

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

 real am(md1a:md1b,md2a:md2b,md3a:md3b)
 real bm(md1a:md1b,md2a:md2b,md3a:md3b)
 real cm(md1a:md1b,md2a:md2b,md3a:md3b)
 real dm(md1a:md1b,md2a:md2b,md3a:md3b)
 real em(md1a:md1b,md2a:md2b,md3a:md3b)

 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

 real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr
 real dtScale,cfl

 ! bcData(component+numberOfComponents*(0),side,axis,grid)
 integer numberOfComponents,systemComponent
 integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
 real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

 integer ipar(0:*)
 real rpar(0:*)
 
 !     ---- local variables -----
 integer m,n,c,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask
 integer gridIsImplicit,implicitOption,implicitMethod,ibc,\
 isAxisymmetric,use2ndOrderAD,use4thOrderAD,useSelfAdjointDiffusion,\
 orderOfExtrapolation,fourthOrder,dirp1,dirp2
 integer pc,uc,vc,wc,tc,vsc,fc,fcu,fcv,fcw,fcn,fct,grid,side,gridType
 integer computeMatrix,computeRHS,computeMatrixBC
 integer twilightZoneFlow,computeTemperature
 integer indexRange(0:1,0:2),gid(0:1,0:2),is1,is2,is3
 real nu,kThermal,thermalExpansivity,gravity(0:2)
 real dx(0:2),dx0,dy,dz,dxi,dyi,dzi,dri,dsi,dti
 real dxv2i(0:2),dx2i,dy2i,dz2i
 real dxvsqi(0:2),dxsqi,dysqi,dzsqi
 real drv2i(0:2),dr2i,ds2i,dt2i
 real drvsqi(0:2),drsqi,dssqi,dtsqi
 real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,dyz4i
 real ad21,ad22,ad41,ad42,cd22,cd42,adc,sn
 real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
 real dr(0:2)

 real adCoeff2,adCoeff4
 real cexa,cexb,cexc,cexd,cexe
 real c4exa,c4exb,c4exc,c4exd,c4exe

 integer option
 integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
 parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

 real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
 real dd,dndx(0:2)

 integer axis,kd
 real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
 real magu,magumax,ymax,ulmax,lmixw,lmixmax,lmix2max,vto,vort,fdotn,tawu ! baldwin-lomax tmp variables
 real yscale,yplus,nmag,ftan(3),norm(3),tauw,maxumag,maxvt,ctrans,ditrip ! more baldwin-lomax tmp variables
 integer iswitch, ibb, ibe, i, ii1,ii2,ii3,io(3) ! baldwin-lomax loop variables
 integer itrip,jtrip,ktrip !baldwin-lomax trip location
 real chi,fnu1,fnu2,s,r,g,fw,dKappaSq,nBydSqLhs,nSqBydSq,nutb
 real nuTilde,nuT,nuTx(0:2),fv1,fv1x,fv1y,fv1z
 real nuTSA,chi3,nuTd

 real urr0,uss0,utt0

#If #SOLVER == "INSVP"
 ! --- declare variables for a nonlinear viscosity (from consCoeff.h) ---
 declareNonLinearViscosityVariables()

#End

 double precision pdb
 character *50 name
 integer ok,getInt,getReal

 integer nc
#If #SOLVER == "INSSPAL"
 real fsa2d0,fsa2d1,fsa2d2
 real fsa3d0,fsa3d1,fsa3d2
 real fsac2d0,fsac2d1,fsac2d2
 real fsac3d0,fsac3d1,fsac3d2

 real fusar2d0,fusar2d1,fusar2d2
 real fusar3d0,fusar3d1,fusar3d2
 real fusac2d0,fusac2d1,fusac2d2
 real fusac3d0,fusac3d1,fusac3d2
#End

 integer \
     noSlipWall,\
     outflow,\
     convectiveOutflow,\
     tractionFree,\
     inflowWithPandTV,\
     dirichletBoundaryCondition,\
     symmetry,\
     axisymmetric
 parameter( noSlipWall=1,outflow=5,convectiveOutflow=14,tractionFree=15,\
  inflowWithPandTV=3,dirichletBoundaryCondition=12,symmetry=11,axisymmetric=13 )

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer interpolate,dirichlet,neumann,extrapolate
 parameter( interpolate=0, dirichlet=1, neumann=2, extrapolate=3 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

 !     --- begin statement functions
 real t1,t2,dr0i,dr1i
 ! real fr2d0,fr2d1,fr2d2,fc2d0,fc2d1,fc2d2
 ! real resr2d0,resr2d1
 ! real fr3d0,fr3d1,fr3d2,fc3d0,fc3d1,fc3d2
 real ftr2d0,ftr2d1,ftr2d2,ftc2d0,ftc2d1,ftc2d2
 real ftr3d0,ftr3d1,ftr3d2,ftc3d0,ftc3d1,ftc3d2
 real uAve0,uAve1,uAve2,uAve3d0,uAve3d1,uAve3d2
 real ad2Coeff,ad2,ad23Coeff,ad23,ad4Coeff,ad4,ad43Coeff,ad43
 real ad2cCoeff,ad23cCoeff
 real ad2nCoeff,ad23nCoeff,ad2cnCoeff,ad23cnCoeff

 real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
 real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
 real udmzC,udzmC,udmzzC,udzmzC,udzzmC
 real admzR,adzmR,admzzR,adzmzR,adzzmR
 real admzC,adzmC,admzzC,adzmzC,adzzmC
 real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
 real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
 real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f
 real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
 real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

 real rxi,rxr,rxs,rxt,rxx,rxy,ryy,rxx3,rxy3,rxz3
 real ur,us,ut,urs,urt,ust,urr,uss,utt
 real uxx0,uyy0,uzz0,ux2c,uy2c,ux3c,uy3c,uz3c
 real lap2d2c,lap3d2c

 real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
 real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,uyz4,uyy4,uzz4

 real mixedRHS,mixedCoeff,mixedNormalCoeff,a0,a1

 real rx,ry,rz,sx,sy,sz,tx,ty,tz

! include 'declareDiffOrder2f.h'
! include 'declareDiffOrder4f.h'
 declareDifferenceOrder2(u,RX)
! declareDifferenceOrder4(u,RX)

 ! This include file (created above) declares variables needed by the getDuDx() macros. (
!** include 'insLSdeclareTemporaryVariablesOrder2.h'

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

 !     The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 ! defineDifferenceOrder4Components1(u,RX)

 !*      include 'insDeriv.h'
 !*      include 'insDerivc.h'


 uu(c)    = u(i1,i2,i3,c)

 ux2(c)   = ux22r(i1,i2,i3,c)
 uy2(c)   = uy22r(i1,i2,i3,c)
 uz2(c)   = uz23r(i1,i2,i3,c)
 uxy2(c)  = uxy22r(i1,i2,i3,c)
 uxz2(c)  = uxz23r(i1,i2,i3,c) 
 uyz2(c)  = uyz23r(i1,i2,i3,c) 
 uxx2(c)  = uxx22r(i1,i2,i3,c) 
 uyy2(c)  = uyy22r(i1,i2,i3,c) 
 uzz2(c)  = uzz23r(i1,i2,i3,c) 
 lap2d2(c)= ulaplacian22r(i1,i2,i3,c)
 lap3d2(c)= ulaplacian23r(i1,i2,i3,c)

!* ux4(c)   = ux42r(i1,i2,i3,c)
!* uy4(c)   = uy42r(i1,i2,i3,c)
!* uz4(c)   = uz43r(i1,i2,i3,c)
!* uxy4(c)  = uxy42r(i1,i2,i3,c)
!* uxz4(c)  = uxz43r(i1,i2,i3,c) 
!* uyz4(c)  = uyz43r(i1,i2,i3,c) 
!* uxx4(c)  = uxx42r(i1,i2,i3,c) 
!* uyy4(c)  = uyy42r(i1,i2,i3,c) 
!* uzz4(c)  = uzz43r(i1,i2,i3,c) 
!* lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
!* lap3d4(c)= ulaplacian43r(i1,i2,i3,c)

 rxi(m,n) = rsxy(i1,i2,i3,m,n)
 rxr(m,n) = (rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*dr2i
 rxs(m,n) = (rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*ds2i
 rxt(m,n) = (rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))*dt2i
 rxx(m,n) = rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)
 rxy(m,n) = rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)
 ryy(m,n) = rxy(m,n)

 rxx3(m,n)= rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)+rxi(2,0)*rxt(m,n)
 rxy3(m,n)= rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)+rxi(2,1)*rxt(m,n)
 rxz3(m,n)= rxi(0,2)*rxr(m,n)+rxi(1,2)*rxs(m,n)+rxi(2,2)*rxt(m,n)

 ur(m) = ur2(i1,i2,i3,m)
 us(m) = us2(i1,i2,i3,m) 
 ut(m) = ut2(i1,i2,i3,m) 
 urs(m)= urs2(i1,i2,i3,m) 
 urt(m)= urt2(i1,i2,i3,m) 
 ust(m)= ust2(i1,i2,i3,m) 
 urr(m)= urr2(i1,i2,i3,m)
 uss(m)= uss2(i1,i2,i3,m)
 utt(m)= utt2(i1,i2,i3,m)

 ux2c(m) = ux22(i1,i2,i3,m)
 uy2c(m) = uy22(i1,i2,i3,m)

 ux3c(m) = ux23(i1,i2,i3,m)
 uy3c(m) = uy23(i1,i2,i3,m)
 uz3c(m) = uz23(i1,i2,i3,m)

 lap2d2c(m) = ulaplacian22(i1,i2,i3,m)
 lap3d2c(m) = ulaplacian23(i1,i2,i3,m)                            



 !      ux(c) = (u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c))*dx2i
 !      uy(c) = (u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c))*dy2i
 !      uxx(c) = (u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi
 !      uyy(c) = (u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))*dysqi
 uxx0(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi  ! without diagonal term
 uyy0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))*dysqi  ! without diagonal term
 uzz0(c) = (u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))*dzsqi  ! without diagonal term

 urr0(m)  = (u(i1+1,i2,i3,m)+u(i1-1,i2,i3,m))*drsqi  ! without diagonal term
 uss0(m)  = (u(i1,i2+1,i3,m)+u(i1,i2-1,i3,m))*dssqi  ! without diagonal term
 utt0(m)  = (u(i1,i2,i3+1,m)+u(i1,i2,i3-1,m))*dtsqi  ! without diagonal term

 uAve0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
 uAve1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))
 uAve2(c) = 0.

 uAve3d0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
 uAve3d1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
 uAve3d2(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))

! resr2d0(m) = f(i1,i2,i3,0) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*lap2d2(m)
! resr2d1(m) = f(i1,i2,i3,1) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*lap2d2(m)

 ! INS - RHS forcing for rectangular grids, directions=0,1,2 (do NOT include grad(p) terms, since then macro is valid for m=uc,vc,wc)
 !  fr2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+nu*uyy0(m) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 !  fr2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+nu*uxx0(m) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 !  fr2d2(m) = 0.

 !  fr3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+nu*(uyy0(m)+uzz0(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 !  fr3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+nu*(uxx0(m)+uzz0(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
 !  fr3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*(uxx0(m)+uyy0(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)

 ! Temperature - RHS, forcing for rectangular grids, directions=0,1,2
 ftr2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+kThermal*uyy0(m)
 ftr2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+kThermal*uxx0(m)
 ftr2d2(m) = 0.

 ftr3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+kThermal*(uyy0(m)+uzz0(m))
 ftr3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+kThermal*(uxx0(m)+uzz0(m))
 ftr3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+kThermal*(uxx0(m)+uyy0(m))

 ! INS - RHS forcing for curvilinear grids, directions=0,1,2  (do NOT include grad(p) terms)
!*  fc2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+nu*(rxx(1,0)+ryy(1,1)))*us(m)+\
!*   nu*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
!* 
!*  fc2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +\
!*   (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+nu*(rxx(0,0)+ryy(0,1)))*ur(m)+\
!*   nu*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
!*  fc2d2(m) = 0.
!* 
!*  fc3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
!*   (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
!*   nu*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
!*        (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
!*        2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
!*        2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
!*      ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
!* 
!*  fc3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
!*   (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
!*   nu*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
!*        (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
!*        2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
!*        2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
!*      ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)
!* 
!*  fc3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
!*   (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
!*   nu*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
!*        (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
!*        2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
!*        2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
!*      ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc)

 ! Temperature - RHS forcing for curvilinear grids, directions=0,1,2
!*  ftc2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+kThermal*(rxx(1,0)+ryy(1,1)))*us(m)+\
!*   kThermal*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))
!* 
!*  ftc2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +\
!*   (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+kThermal*(rxx(0,0)+ryy(0,1)))*ur(m)+\
!*   kThermal*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))
!*  ftc2d2(m) = 0.
!* 
!*  ftc3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
!*   (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
!*   kThermal*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
!*        (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
!*        2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
!*        2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
!*      )
!* 
!*  ftc3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
!*   (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
!*   kThermal*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
!*        (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
!*        2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
!*        2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
!*      )
!* 
!*  ftc3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
!*   (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
!*   (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
!*   kThermal*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
!*        (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
!*        2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
!*        2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
!*        2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
!*      )

#If #SOLVER == "INSSPAL"
 !     --- for SA TM ---
 nuTSA(i1,i2,i3)=u(i1,i2,i3,nc)*(u(i1,i2,i3,nc)/nu)**3/((u(i1,i2,i3,nc)/nu)**3+cv1e3)

 fusar2d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+nuT*uyy0(m)
 fusar2d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+nuT*uxx0(m) 
 fusar2d2(m) = 0.

 fusar3d0(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+nuT*(uyy0(m)+uzz0(m)) 
 fusar3d1(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+nuT*(uxx0(m)+uzz0(m))
 fusar3d2(m) = uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nuT*(uxx0(m)+uyy0(m)) 

 fsa2d0(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*dndx(1)+nutb*uyy0(nc)+(1.+cb2)*sigmai*dndx(1)**2
 fsa2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)+nutb*uxx0(nc)+(1.+cb2)*sigmai*dndx(0)**2
 fsa2d2(m)=0.

 fsa3d0(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*dndx(1)-uu(wc)*dndx(2)+nutb*(uyy0(nc)+uzz0(nc))\
          +(1.+cb2)*sigmai*(dndx(1)**2+dndx(2)**2)
 fsa3d1(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)-uu(wc)*dndx(2)+nutb*(uxx0(nc)+uzz0(nc))\
          +(1.+cb2)*sigmai*(dndx(0)**2+dndx(2)**2)
 fsa3d2(m)=uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*dndx(0)-uu(vc)*dndx(1)+nutb*(uxx0(nc)+uyy0(nc))\
          +(1.+cb2)*sigmai*(dndx(0)**2+dndx(1)**2)

 !     --- SA TM curvilinear grid case ----    

 ! The momentum equations are the same as above but with nu replaced by nuT
 fusac2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+nuT*(rxx(1,0)+ryy(1,1)))*us(m)+\
  nuT*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))

 fusac2d1(m)=uu(m)*dtScale/dt(i1,i2,i3) +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+nuT*(rxx(0,0)+ryy(0,1)))*ur(m)+\
  nuT*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m))
 fusac2d2(m) = 0.

 fusac3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nuT*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nuT*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nuT*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 fusac3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nuT*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nuT*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nuT*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 fusac3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nuT*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nuT*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  nuT*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     )

 fsac2d0(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(1,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(1,1)\
        +   nutb*(rxx(1,0)+ryy(1,1)) )*us(m)\
        +  nutb*( (rxi(1,0)**2+rxi(1,1)**2)*uss0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m) )
 fsac2d1(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(0,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(0,1)\
        +   nutb*(rxx(0,0)+ryy(0,1)) )*ur(m)\
        +  nutb*( (rxi(0,0)**2+rxi(0,1)**2)*urr0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m) )
 fsac2d2(m)=0.

 fsac3d0(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(1,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(1,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(1,2)\
        +     nutb*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)) )*us(m)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(2,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(2,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(2,2)\
        +     nutb*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)) )*ut(m) \
        +  nutb*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)\
        +         (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)\
        +         2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)\
        +         2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m) )
 fsac3d1(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(0,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(0,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(0,2)\
        +     nutb*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)) )*ur(m)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(2,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(2,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(2,2)\
        +     nutb*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)) )*ut(m) \
        +  nutb*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)\
        +         (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)\
        +         2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)\
        +         2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m) )
 fsac3d2(m)=uu(m)*dtScale/dt(i1,i2,i3)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(0,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(0,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(0,2)\
        +     nutb*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)) )*ur(m)\
        + ( (-uu(uc)+(1.+cb2)*sigmai*dndx(0))*rxi(1,0)\
        +   (-uu(vc)+(1.+cb2)*sigmai*dndx(1))*rxi(1,1)\
        +   (-uu(wc)+(1.+cb2)*sigmai*dndx(2))*rxi(1,2)\
        +     nutb*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)) )*us(m) \
        +  nutb*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)\
        +         (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)\
        +         2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)\
        +         2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)\
        +         2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m) )

#End

 !    --- 2nd order 2D artificial diffusion ---
 ad2Coeff()=(ad21 + cd22* \
     ( abs(ux2(uc))+abs(uy2(uc))  \
      +abs(ux2(vc))+abs(uy2(vc)) ) )
 ad2cCoeff()=(ad21 + cd22* \
     ( abs(ux2c(uc))+abs(uy2c(uc))  \
      +abs(ux2c(vc))+abs(uy2c(vc)) ) )
 ad2nCoeff() =(ad21 + cd22*( abs(ux2(nc)) +abs(uy2(nc)) ) ) ! for eddy viscosity
 ad2cnCoeff()=(ad21 + cd22*( abs(ux2c(nc))+abs(uy2c(nc)) ) )
 ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
            +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

 !    --- 2nd order 3D artificial diffusion ---
 ad23Coeff()=(ad21 + cd22*   \
     ( abs(ux2(uc))+abs(uy2(uc))+abs(uz2(uc)) \
      +abs(ux2(vc))+abs(uy2(vc))+abs(uz2(vc))  \
      +abs(ux2(wc))+abs(uy2(wc))+abs(uz2(wc)) ) )
 ad23cCoeff()=(ad21 + cd22*   \
     ( abs(ux3c(uc))+abs(uy3c(uc))+abs(uz3c(uc)) \
      +abs(ux3c(vc))+abs(uy3c(vc))+abs(uz3c(vc))  \
      +abs(ux3c(wc))+abs(uy3c(wc))+abs(uz3c(wc)) ) )
 ad23nCoeff() =(ad21 + cd22*( abs(ux2(nc)) +abs(uy2(nc)) +abs(uz2(nc)) ) ) ! for eddy viscosity
 ad23cnCoeff()=(ad21 + cd22*( abs(ux3c(nc))+abs(uy3c(nc))+abs(uz3c(nc)) ) )
 ad23(c)=adc\
     *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
      +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) \
      +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                  
 !     ---fourth-order artficial diffusion in 2D
 ad4Coeff()=(ad41 + cd42*    \
     ( abs(ux2(uc))+abs(uy2(uc))    \
      +abs(ux2(vc))+abs(uy2(vc)) ) )
 ad4(c)=adc\
     *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    \
          -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    \
      +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    \
          +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   \
       -12.*u(i1,i2,i3,c) ) 
 !     ---fourth-order artficial diffusion in 3D
 ad43Coeff()=\
    (ad41 + cd42*    \
     ( abs(ux2(uc))+abs(uy2(uc))+abs(uz2(uc))    \
      +abs(ux2(vc))+abs(uy2(vc))+abs(uz2(vc))    \
      +abs(ux2(wc))+abs(uy2(wc))+abs(uz2(wc)) ) )
 ad43(c)=adc\
     *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
          -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
          -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   \
      +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
          +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   \
          +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  \
       -18.*u(i1,i2,i3,c) )


 #Include "selfAdjointArtificialDiffusion.h"

! statement functions to access coefficients of mixed-boundary conditions
 mixedRHS(c,side,axis,grid)         =bcData(c+numberOfComponents*(0),side,axis,grid)
 mixedCoeff(c,side,axis,grid)       =bcData(c+numberOfComponents*(1),side,axis,grid)
 mixedNormalCoeff(c,side,axis,grid) =bcData(c+numberOfComponents*(2),side,axis,grid)
 !     --- end statement functions

 ierr=0
 ! write(*,*) 'Inside insLineSolve'


 INITIALIZE(SOLVER)


 if ( option.eq.setupSweep ) then
  #If #SOLVER == "INSBL"
   if ( turbulenceModel.eq.baldwinLomax ) then
     computeBLNuT()
   end if
  #Else
    stop 825
  #End 

 else if( option.eq.assignINS )then

  ! **************************************************************************
  ! Fill in the tridiagonal matrix for the momentum equations for the INS plus
  ! artificial dissipation and/or turbulence model
  ! ***************************************************************************

  #If #SOLVER == "INS"
    fillEquations(INS)
  #Elif #SOLVER == "INSSPAL"
    fillEquations(INSSPAL)
  #Elif #SOLVER == "INSBL"
    fillEquations(INSBL)
  #Elif #SOLVER == "INSVP"
    fillEquations(INSVP)
  #Else
    stop 555
  #End

 else if( option.eq.assignTemperature )then

  ! **************************************************************************
  ! Fill in the tridiagonal matrix for the INS Temperature equation 
  ! ***************************************************************************
  #If #SOLVER == "INS"
   fillEquations(INS_TEMPERATURE)
  #Else
    stop 556 
  #End

 else if( option.eq.assignSpalartAllmaras )then

  ! **************************************************************************
  ! Fill in the tridiagonal matrix for the turbulent eddy viscosity eqution for 
  ! the Spalart Almaras TM
  ! ***************************************************************************

  #If #SOLVER == "INSSPAL"
   if( gridType.eq.rectangular )then
     if( dir.eq.0 )then
       fillSAEquationsRectangularGrid(0)
     else if( dir.eq.1 )then
       fillSAEquationsRectangularGrid(1)
     else ! dir.eq.2
       fillSAEquationsRectangularGrid(2)
     end if ! end dir
   else
     if( dir.eq.0 )then
       fillSAEquationsCurvilinearGrid(0)
     else if( dir.eq.1 )then
       fillSAEquationsCurvilinearGrid(1)
     else ! dir.eq.2
       fillSAEquationsCurvilinearGrid(2)
     end if ! end dir
   end if
  #Else
   stop 777
  #End
 else
   write(*,*) 'Unknown option=',option
   stop 8
 end if ! option

 

!* if( .false. ) then ! done elsewhere
!*  ! ****** Boundary Conditions ******
!*  indexRange(0,0)=n1a
!*  indexRange(1,0)=n1b
!*  indexRange(0,1)=n2a
!*  indexRange(1,1)=n2b
!*  indexRange(0,2)=n3a
!*  indexRange(1,2)=n3b
!*  ! assign loop variables to correspond to the boundary
!*  
!*  do side=0,1
!*    is1=0
!*    is2=0
!*    is3=0
!*    if( dir.eq.0 )then
!*      is1=1-2*side
!*      n1a=indexRange(side,dir)-is1    ! boundary is 1 pt outside
!*      n1b=n1a
!*    else if( dir.eq.1 )then
!*      is2=1-2*side
!*      n2a=indexRange(side,dir)-is2
!*      n2b=n2a
!*    else
!*      is3=1-2*side
!*      n3a=indexRange(side,dir)-is3
!*      n3b=n3a
!*    end if
!*     
!* 
!*   sn=2*side-1 ! sign for normal
!*   ! write(*,*) '$$$$$ side,bc = ',side,bc(side,ibc)
!*   if( bc(side,ibc).eq.dirichlet )then
!*     if( computeMatrixBC.eq.1 )then
!*       if( fourthOrder.eq.0 )then
!*         loopsMatrixBC( SOLVER,\
!*                        am(i1,i2,i3)=0.,\
!*                        bm(i1,i2,i3)=1.,\
!*                        cm(i1,i2,i3)=0.,,,)
!*       else
!*         loopsMatrixBC4(SOLVER,$$assignDirichletFourthOrder(),,,,,)
!*       end if
!*     end if   
!* 
!*   else if( bc(side,ibc).eq.neumann )then 
!* 
!*     ! apply a neumann BC on this side.
!*     !             | b[0] c[0] a[0]                |
!*     !             | a[1] b[1] c[1]                |
!*     !         A = |      a[2] b[2] c[2]           |
!*     !             |            .    .    .        |
!*     !             |                a[.] b[.] c[.] |
!*     !             |                c[n] a[n] b[n] |
!*     if( computeMatrixBC.eq.1 )then
!* 
!*       if( computeTemperature.ne.0 )then
!*         a0 = mixedCoeff(tc,side,dir,grid)
!*         a1 = mixedNormalCoeff(tc,side,dir,grid)
!*         write(*,'(" insLineSolve: T BC: (a0,a1)=(",f3.1,",",f3.1,") for side,dir,grid=",3i3)') a0,a1,side,dir,grid
!*         ! '
!*       end if
!*       if( fourthOrder.eq.0 )then
!*         if( side.eq.0 )then
!*           loopsMatrixBC(SOLVER,\
!*                         bm(i1,i2,i3)= 1.,\
!*                         cm(i1,i2,i3)=0.,\
!*                         am(i1,i2,i3)=-1.,,,)
!*         else
!*           loopsMatrixBC(SOLVER,\
!*                         cm(i1,i2,i3)=-1.,\
!*                         am(i1,i2,i3)=0.,\
!*                         bm(i1,i2,i3)= 1.,,,)
!*         end if
!*       else
!*         ! use +-D0 and D+D-D0
!*         if( side.eq.0 )then
!*           cexa= 0.
!*           cexb= 0.
!*           cexc= 1.
!*           cexd= 0.
!*           cexe=-1.
!* 
!*           c4exa= 2.
!*           c4exb=-1.
!*           c4exc= 1.
!*           c4exd=-2.
!*           c4exe= 0.
!*         else
!*           cexa=-1.
!*           cexb= 0.
!*           cexc= 1.
!*           cexd= 0.
!*           cexe= 0.
!*           c4exa= 0.
!*           c4exb=-2.
!*           c4exc= 1.
!*           c4exd=-1.
!*           c4exe= 2.
!*         end if
!*         loopsMatrixBC4(SOLVER,$$assignFourthOrder(),,,,,)
!*       end if
!*     end if
!* 
!*   else if( bc(side,ibc).eq.extrapolate )then 
!* 
!*     if( computeMatrixBC.eq.1 )then
!* 
!*       if( fourthOrder.eq.0 )then
!*         ! **** second order ****
!*         if( orderOfExtrapolation.eq.2 )then
!*           if( side.eq.0 )then
!*             cexa= 1.
!*             cexb= 1.
!*             cexc=-2.
!*           else
!*             cexa=-2.
!*             cexb= 1.
!*             cexc= 1.
!*           end if
!*         else if( orderOfExtrapolation.eq.3 )then 
!*           if( side.eq.0 )then
!*             cexa= 3.
!*             cexb= 1.
!*             cexc=-3.
!*           else
!*             cexa=-3.
!*             cexb= 1.
!*             cexc= 3.
!*           end if
!*         else
!*           write(*,*) 'ERROR: not implemeted: orderOfExtrapolation=',orderOfExtrapolation
!*           stop 1111
!*         end if
!*         loopsMatrixBC( SOLVER,\
!*                        am(i1,i2,i3)=cexa,\
!*                        bm(i1,i2,i3)=cexb,\
!*                        cm(i1,i2,i3)=cexc,,,)
!* 
!*       else 
!*         ! **** fourth order ****
!*         if( orderOfExtrapolation.eq.2 )then
!*           if( side.eq.0 )then
!*             cexa= 0.
!*             cexb= 0.
!*             cexc= 1.
!*             cexd=-2.
!*             cexe= 1.
!*           else
!*             cexa= 1.
!*             cexb=-2.
!*             cexc= 1.
!*             cexd= 0.
!*             cexe= 0.
!*           end if
!*         else if( orderOfExtrapolation.eq.3 )then 
!*           if( side.eq.0 )then
!*             cexa=-1.
!*             cexb= 0.
!*             cexc= 1.
!*             cexd=-3.
!*             cexe= 3.
!*           else
!*             cexa= 3.
!*             cexb=-3.
!*             cexc= 1.
!*             cexd= 0.
!*             cexe=-1.
!*           end if
!*         else
!*           write(*,*) 'ERROR: not implemeted: orderOfExtrapolation=',orderOfExtrapolation
!*           stop 1111
!*         end if
!*         if( side.eq.0 )then
!*           c4exa=-4.
!*           c4exb= 1.
!*           c4exc= 1.
!*           c4exd=-4.
!*           c4exe= 6.
!*         else
!*           c4exa=+6.
!*           c4exb=-4.
!*           c4exc= 1.
!*           c4exd= 1.
!*           c4exe=-4.
!*         end if
!*         loopsMatrixBC4(SOLVER,$$assignFourthOrder(),,,,,)
!* 
!*       end if
!*     end if
!* 
!*   end if
!* 
!*   ! reset values
!*   if( dir.eq.0 )then
!*     n1a=indexRange(0,dir)
!*     n1b=indexRange(1,dir)
!*   else if( dir.eq.1 )then
!*     n2a=indexRange(0,dir)
!*     n2b=indexRange(1,dir)
!*   else
!*     n3a=indexRange(0,dir)
!*     n3b=indexRange(1,dir)
!*   end if
!*  end do ! do side
!* 
!* end if

 return
 end
#endMacro

#beginMacro INS_LINE_SETUP_NULL(SOLVER,NAME)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
      md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw,\
      dir,am,bm,cm,dm,em,  bc, boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c        ****** NULL version **********
c 
c Used if we don't want to compile the real file for a given case
c======================================================================

 write(*,'("ERROR: NULL version of subroutine NAME called")')
 write(*,'(" You may have to turn on an option in the Makefile.")')
 ! ' 
 stop 1050

 return
 end
#endMacro


#beginMacro buildFile(SOLVER,NAME)
#beginFile src/NAME.f
 INS_LINE_SETUP(SOLVER,NAME)
#endFile
#beginFile src/NAME ## Null.f
 INS_LINE_SETUP_NULL(SOLVER,NAME)
#endFile
#endMacro


      buildFile(INS,lineSolveNewINS)
c      buildFile(INSSPAL,lineSolveINSSPAL)
c      buildFile(INSBL,lineSolveINSBL)
      buildFile(INSVP,lineSolveNewINSVP)



      subroutine insLineSetupNew(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
       md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
       ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c         ************* INS Line Solver Function ***************
c
c This function can:
c  (1) Fill in the matrix coefficents for line solvers
c  (2) Assign the right-hand-side values in f
c  (3) Compute the residual  
c
c NOTES:
c   Fill in the interior equation for points (n1a:n1b,n2a:n2b,n3a:n3b)
c   Fill in the BC equations for points outside this (along the line solver direction)
c   
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & md1a,md1b,md2a,md2b,md3a,md3b,
     & dir

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real am(md1a:md1b,md2a:md2b,md3a:md3b)
      real bm(md1a:md1b,md2a:md2b,md3a:md3b)
      real cm(md1a:md1b,md2a:md2b,md3a:md3b)
      real dm(md1a:md1b,md2a:md2b,md3a:md3b)
      real em(md1a:md1b,md2a:md2b,md3a:md3b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr

      ! bcData(component+numberOfComponents*(0),side,axis,grid)  
      integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
      real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

      integer ipar(0:*)
      real rpar(0:*)
      
c     ---- local variables -----
      integer option
      integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
      parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

c     --- end statement functions

      ierr=0
      ! write(*,*) 'Inside insLineSolve'

      option            = ipar(21)
      turbulenceModel   = ipar(23)
      pdeModel          = ipar(27)

      if( turbulenceModel.eq.noTurbulenceModel )then
        if( pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel .or. option.eq.assignTemperature )then
          call lineSolveNewINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
           ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
        else if( pdeModel.eq.viscoPlasticModel )then
         if( .false. .or. option.eq.assignTemperature )then
          call lineSolveINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
           ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
         else
          call lineSolveNewINSVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
           ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
         end if
        else
          stop 5533
        end if
        
c      else if( turbulenceModel.eq.spalartAllmaras )then
c        call lineSolveNewINSSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
c          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c      else if( turbulenceModel.eq.baldwinLomax )then
c        call lineSolveNewINSBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
c          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
      else
        write(*,*) 'insLineSetup:Unknown turbulenceModel=',turbulenceModel
        stop 444
      end if      

      return
      end





#beginMacro resLoops(e1,e2,e3,e4,e5,e6)
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
      e5
      e6
    else 
    end if
  end do
  end do
  end do
#endMacro


c====================================================================
c Define first derivatives and the coeffciients adc2 and adc4 for the 
c artficial dissipation
c====================================================================
#beginMacro getDerivativesAndDissipation(DIM,ORDER,GRIDTYPE)

 defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

 u0x=UX(uc)
 u0y=UY(uc)
 v0x=UX(vc)
 v0y=UY(vc)

#If #DIM == "2"
 adc = abs(u0x)+abs(u0y)+abs(v0x)+abs(v0y)
#Elif #DIM == "3"
 u0z=UZ(uc)
 v0z=UZ(vc)
 w0x=UX(wc)
 w0y=UY(wc)
 w0z=UZ(wc)
 adc = abs(u0x)+abs(u0y)+abs(u0z)+abs(v0x)+abs(v0y)+abs(v0z)+abs(w0x)+abs(w0y)+abs(w0z)
#Else
  stop 7654
#End

 adc2= ad21 + cd22*adc
 adc4= ad41 + cd42*adc

#endMacro



#beginMacro COMPUTE_RESIDUAL(SOLVER,NAME)
      subroutine NAME(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,rsxy,  u,gv,dt,f,dw, residual,
     & bc, ipar, rpar, ierr )
c======================================================================
c
c  *********** Compute the residual *****************
c
c nd : number of space dimensions
c
c u : input - current solution
c f : input rhs forcing
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real residual(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),ierr
      real dtScale,cfl

      integer ipar(0:*)
      real rpar(0:*)

c     ---- local variables -----
      integer m,n,c,kd,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask
      integer gridIsImplicit,implicitOption,implicitMethod,ibc,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD,useSelfAdjointDiffusion,
     & orderOfExtrapolation,fourthOrder,dirp1,dirp2
      integer pc,uc,vc,wc,tc, vsc,fc,fcu,fcv,fcw,fcn,fct,grid,side,gridType
      integer computeMatrix,computeRHS,computeMatrixBC,computeTemperature
      integer twilightZoneFlow
      integer indexRange(0:1,0:2),gid(0:1,0:2),is1,is2,is3
      real nu,kThermal,thermalExpansivity,gravity(0:2)
      real dx(0:2),dx0,dy,dz,dxi,dyi,dzi,dri,dsi,dti
      real dxv2i(0:2),dx2i,dy2i,dz2i
      real dxvsqi(0:2),dxsqi,dysqi,dzsqi
      real drv2i(0:2),dr2i,ds2i,dt2i
      real drvsqi(0:2),drsqi,dssqi,dtsqi
      real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,
     & dxy4i,dxz4i,dyz4i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc,sn,adc2,adc4
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real dr(0:2)

      integer option
      integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
      parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

      real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
      real dd,dndx(0:2)

      real chi,fnu1,fnu2,s,r,g,fw,dKappaSq,nBydSqLhs,nSqBydSq,nutb
      real nuTilde,nuT,nuTx(0:2),fv1,fv1x,fv1y,fv1z
      real nuTSA,chi3,nuTd
      real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
      integer itrip,jtrip,ktrip !baldwin-lomax trip location

      integer numberOfComponents,systemComponent
      integer nc

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

      real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
      real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
      real udmzC,udzmC,udmzzC,udzmzC,udzzmC
      real admzR,adzmR,admzzR,adzmzR,adzzmR
      real admzC,adzmC,admzzC,adzmzC,adzzmC
      real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
      real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
      real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f
      real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
      real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

      real rxi,rxr,rxs,rxt,rxx,rxy,ryy,rxx3,rxy3,rxz3
      real ur,us,ut,urs,urt,ust,urr,uss,utt
      real uxx0,uyy0,uzz0,ux2c,uy2c,ux3c,uy3c,uz3c
      real lap2d2c,lap3d2c

      real u0,u0x,u0y,u0z
      real v0,v0x,v0y,v0z
      real w0,w0x,w0y,w0z

      declareNonLinearViscosityVariables()

c     ------------ start statement functions -------------------
      real rx,ry,rz,sx,sy,sz,tx,ty,tz

      real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
      real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,uyz4,uyy4,uzz4

      real  ad2Coeff,ad2rCoeff,ad2,ad23Coeff,ad23rCoeff,ad23,ad4Coeff,ad4rCoeff,ad4,ad43Coeff,ad43rCoeff,ad43

      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'
      declareDifferenceOrder2(u,RX)
!      declareDifferenceOrder4(u,RX)

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

      !     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
!      defineDifferenceOrder4Components1(u,RX)

      !*      include 'insDeriv.h'
      !*      include 'insDerivc.h'

      uu(c)    = u(i1,i2,i3,c)

      ux2(c)   = ux22r(i1,i2,i3,c)
      uy2(c)   = uy22r(i1,i2,i3,c)
      uz2(c)   = uz23r(i1,i2,i3,c)
      uxy2(c)  = uxy22r(i1,i2,i3,c)
      uxz2(c)  = uxz23r(i1,i2,i3,c) 
      uyz2(c)  = uyz23r(i1,i2,i3,c) 
      uxx2(c)  = uxx22r(i1,i2,i3,c) 
      uyy2(c)  = uyy22r(i1,i2,i3,c) 
      uzz2(c)  = uzz23r(i1,i2,i3,c) 
      lap2d2(c)= ulaplacian22r(i1,i2,i3,c)
      lap3d2(c)= ulaplacian23r(i1,i2,i3,c)

!*       ux4(c)   = ux42r(i1,i2,i3,c)
!*       uy4(c)   = uy42r(i1,i2,i3,c)
!*       uz4(c)   = uz43r(i1,i2,i3,c)
!*       uxy4(c)  = uxy42r(i1,i2,i3,c)
!*       uxz4(c)  = uxz43r(i1,i2,i3,c) 
!*       uyz4(c)  = uyz43r(i1,i2,i3,c) 
!*       uxx4(c)  = uxx42r(i1,i2,i3,c) 
!*       uyy4(c)  = uyy42r(i1,i2,i3,c) 
!*       uzz4(c)  = uzz43r(i1,i2,i3,c) 
!*       lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
!*       lap3d4(c)= ulaplacian43r(i1,i2,i3,c)

      ux2c(m) = ux22(i1,i2,i3,m)
      uy2c(m) = uy22(i1,i2,i3,m)

      ux3c(m) = ux23(i1,i2,i3,m)
      uy3c(m) = uy23(i1,i2,i3,m)
      uz3c(m) = uz23(i1,i2,i3,m)

      lap2d2c(m) = ulaplacian22(i1,i2,i3,m)
      lap3d2c(m) = ulaplacian23(i1,i2,i3,m)  

c    --- 2nd order 2D artificial diffusion ---
      ad2Coeff()=(ad21 + cd22* 
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))  
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad2rCoeff()=(ad21 + cd22* 
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))  
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad2(adc,c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &               +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
c    --- 2nd order 3D artificial diffusion ---
      ad23Coeff()=(ad21 + cd22*   
     &    ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc)) 
     &     +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))  
     &     +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,i3,wc)) ) )
      ad23rCoeff()=(ad21 + cd22*   
     &    ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc)) 
     &     +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,vc))  
     &     +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,wc)) ) )
      ad23(adc,c)=adc
     &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) 
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                       
c     ---fourth-order artificial diffusion in 2D
      ad4Coeff()=(ad41 + cd42*    
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))    
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad4rCoeff()=(ad41 + cd42*    
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))    
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad4(adc,c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   
     &      -12.*u(i1,i2,i3,c) ) 
c     ---fourth-order artificial diffusion in 3D
      ad43Coeff()=
     &   (ad41 + cd42*    
     &    ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))    
     &     +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))    
     &     +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,i3,wc)) ) )
      ad43rCoeff()=
     &   (ad41 + cd42*    
     &    ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))    
     &     +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,vc))    
     &     +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,wc)) ) )
      ad43(adc,c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  
     &      -18.*u(i1,i2,i3,c) )


#Include "selfAdjointArtificialDiffusion.h"

c ------------ end statement functions -------------------

INITIALIZE(SOLVER)

      if( orderOfAccuracy.eq.2 )then
       #If #SOLVER == "INS"
        if( gridType.eq.rectangular )then
         computeResidualINS(rectangular)
        else
         computeResidualINS(curvilinear)
        end if
       #Elif #SOLVER == "INSVP"
        if( gridType.eq.rectangular )then
         computeResidualVP(rectangular)
        else
         computeResidualVP(curvilinear)
        end if
       #Else
         stop 1188
       #End

      else ! order==4
      end if ! end order 


      return
      end

#endMacro


#beginMacro buildFile(SOLVER,NAME)
#beginFile src/NAME.f
COMPUTE_RESIDUAL(SOLVER,NAME)
#endFile
#beginFile src/NAME ## Null.f
COMPUTE_RESIDUAL(SOLVER,NAME)
#endFile
#endMacro


! ins residual function: 
buildFile(INS,lineSolveResidualINS)
! visco-plastic residual function: 
buildFile(INSVP,lineSolveResidualVP)



      subroutine computeResidualNew(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,rsxy,  u,gv,dt,f,dw, residual,
     & bc, ipar, rpar, ierr )
c======================================================================
c
c  *********** Compute the residual *****************
c
c nd : number of space dimensions
c
c u : input - current solution
c f : input rhs forcing
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real residual(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),ierr
      real dtScale,cfl

      integer ipar(0:*)
      real rpar(0:*)

c     ---- local variables -----
      integer option
      integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
      parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )


      ierr=0
      option            = ipar(21)
      turbulenceModel   = ipar(23)
      pdeModel          = ipar(27)


      if( (pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel ) \
          .and. turbulenceModel.eq.noTurbulenceModel )then

        call lineSolveResidualINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
               mask,rsxy,  u,gv,dt,f,dw, residual, bc, ipar, rpar, ierr )


      else if( pdeModel.eq.viscoPlasticModel )then

        call lineSolveResidualVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
               mask,rsxy,  u,gv,dt,f,dw, residual, bc, ipar, rpar, ierr )

      else
        ! for now use old way for BL, SA
        call computeResidual(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
               mask,rsxy,  u,gv,dt,f,dw, residual, bc, ipar, rpar, ierr )
      end if


      return
      end
