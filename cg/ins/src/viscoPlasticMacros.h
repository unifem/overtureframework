c       *****************************************
c       ** Macros for the Visco-plastic Model ***
c       *****************************************

c  **   NOTE: you should also change viscoPlasticMacrosCpp.h if you change this file ***

c ===============================================================
c Here is effective strain rate (plus a small value)
c         sqrt( (2/3)*eDot_ij eDot_ij ) + epsVP
c Also define the derivatives of the square of the effective strain rate
c  
c ===============================================================
#defineMacro strainRate2d() (sqrt( (2./3.)*( u0x**2 + v0y**2 + .5*( u0y + v0x )**2 ) )+epsVP)

#defineMacro strainRate2dSqx() \
        ( (2./3.)*( 2.*u0x*u0xx + 2.*v0y*v0xy + ( u0y + v0x )*( u0xy+v0xx ) ) )
#defineMacro strainRate2dSqy() \
        ( (2./3.)*( 2.*u0x*u0xy + 2.*v0y*v0yy + ( u0y + v0x )*( u0yy+v0xy ) ) )

#defineMacro strainRate2dSqxx() \
        ( (2./3.)*( 2.*(u0xx*u0xx+u0x*u0xxx) + 2.*(v0xy*v0xy+v0y*v0xxy) \
             + ( u0xy + v0xx )*( u0xy+v0xx ) + ( u0y + v0x )*( u0xxy+v0xxx ) ) )

#defineMacro strainRate2dSqxy() \
        ( (2./3.)*( 2.*(u0xy*u0xx+u0x*u0xxy) + 2.*(v0yy*v0xy+v0y*v0xyy) \
             + ( u0yy + v0xy )*( u0xy+v0xx ) + ( u0y + v0x )*( u0xyy+v0xxy ) ) )

#defineMacro strainRate2dSqyy() \
        ( (2./3.)*( 2.*(u0xy*u0xy+u0x*u0xyy) + 2.*(v0yy*v0yy+v0y*v0yyy) \
             + ( u0yy + v0xy )*( u0yy+v0xy ) + ( u0y + v0x )*( u0yyy+v0xyy ) ) )

c =====================================================================================
c Here is the coefficient of viscosity for the visco plastic model 
c 
c   esr = effective strain rate = || (2/3)*eDot_ij ||
c
c   nuT = etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr))
c 
c =====================================================================================
#beginMacro getViscoPlasticViscosity(nuT,esr)
  
  exp0 = exp(-exponentVP*esr)
  nuT = (etaVP + (yieldStressVP/esr)*(1.-exp0))

#endMacro

c =====================================================================================
c Here is the coefficient of viscosity for the visco plastic model and its first derivative
c   esr = effective strain rate = || (2/3)*eDot_ij ||
c
c   nuT = etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr))
c 
c   nuTd = D( nuT )/D( esr**2)  = D( nuT )/D( esr ) * ( 1 / (2*esr ) )
c =====================================================================================
#beginMacro getViscoPlasticViscosityAndFirstDerivative(esr)
  
  getViscoPlasticViscosity(nuT,esr)
  nuTd = .5*( (-1./esr)*(1.-exp0) + exponentVP*exp0  )*(yieldStressVP/esr**2)

#endMacro

#beginMacro getViscoPlasticViscosityAndTwoDerivative(esr)

  getViscoPlasticViscosityAndFirstDerivative(esr)
!  nuTdd=  .25*( 3./(esr**2)*(1.-exp0) -2./(esr)*exponentVP*exp0 + exponentVP**2*exp0 )*(yieldStressVP/esr**2)
  nuTdd=  .25*( 3./(esr**2)*(1.-exp0) -3./(esr)*exponentVP*exp0 - exponentVP**2*exp0 )*(yieldStressVP/esr**3)

#endMacro


c =============================================================================
c Evaluate the coefficients of the visco-plastic model
c   This macro assumes u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z are defined
c   Used in insdt.bf
c ============================================================================
#beginMacro getViscoPlasticCoefficients(DIM)
  u0xx = UXX(uc)
  u0xy = UXY(uc)
  u0yy = UYY(uc)
  v0xx = UXX(vc)
  v0xy = UXY(vc)
  v0yy = UYY(vc)
#If #DIM == "2"

  eDotNorm = strainRate2d()
  getViscoPlasticViscosityAndFirstDerivative(eDotNorm)

  nuTx = nuTd*strainRate2dSqx()
  nuTy = nuTd*strainRate2dSqy()

#Else

  ! this needs to be finished
 nuT=nu
 nuTx=0. 
 nuTy=0. 
 nuTz=0. 

 stop 7744

#End

!      write(*,9000) i1,i2,nuT,nuTx,nuTy,nuTxx,nuTxy,nuTyy
! 9000  format("insp: i1,i2=",2i3," nuT,nuTx,nuTy,nuTxx,nuTxy,nuTyy=",
!     & 6e10.2)

#endMacro


c ============================================================================
c Define the visco-plastic viscosity and first derivatives
c   Used in inspf.bf
c ============================================================================
#beginMacro getViscoPlasticViscosityAndFirstDerivatives(DIM)
  u0x  = UX(uc)
  u0y  = UY(uc)
  v0x  = UX(vc)
  v0y  = UY(vc)

  u0xx = UXX(uc)
  u0xy = UXY(uc)
  u0yy = UYY(uc)
  v0xx = UXX(vc)
  v0xy = UXY(vc)
  v0yy = UYY(vc)
#If #DIM == "2"

  eDotNorm = strainRate2d()

  getViscoPlasticViscosityAndFirstDerivative(eDotNorm)

  nuTx = nuTd*strainRate2dSqx()
  nuTy = nuTd*strainRate2dSqy()

#Else

 ! FINISH THIS
  
 nuT=nu
 nuTx=0. 
 nuTy=0. 
 nuTz=0. 

 stop 7755

#End

#endMacro

c ============================================================================
c Define the visco-plastic viscosity and first two derivatives 
c   Used in inspf.bf
c ============================================================================
#beginMacro defineVP_Derivatives(DIM)
  u0x  = UX(uc)
  u0y  = UY(uc)
  v0x  = UX(vc)
  v0y  = UY(vc)

  u0xx = UXX(uc)
  u0xy = UXY(uc)
  u0yy = UYY(uc)
  v0xx = UXX(vc)
  v0xy = UXY(vc)
  v0yy = UYY(vc)


#If #DIM == "2"

  u0Lap=u0xx+u0yy
  v0Lap=v0xx+v0yy

  ! still need to define 3d 3rd derivatives   
  u0xxx=UXXX(uc)
  u0xxy=UXXY(uc)
  u0xyy=UXYY(uc)
  u0yyy=UYYY(uc)

  v0xxx=UXXX(vc)
  v0xxy=UXXY(vc)
  v0xyy=UXYY(vc)
  v0yyy=UYYY(vc)


  eDotNorm = strainRate2d()
  getViscoPlasticViscosityAndTwoDerivative(eDotNorm)

  eDotNormSqx =strainRate2dSqx()
  eDotNormSqy =strainRate2dSqy()
  eDotNormSqxx=strainRate2dSqxx()
  eDotNormSqxy=strainRate2dSqxy()
  eDotNormSqyy=strainRate2dSqyy()

  nuTx=nuTd*eDotNormSqx
  nuTy=nuTd*eDotNormSqy
  nuTxx=nuTd*eDotNormSqxx + nuTdd*eDotNormSqx**2
  nuTxy=nuTd*eDotNormSqxy + nuTdd*eDotNormSqx*eDotNormSqy
  nuTyy=nuTd*eDotNormSqyy + nuTdd*eDotNormSqy**2

#Else

  u0zz = UZZ(uc)
  v0zz = UZZ(vc)

  u0Lap=u0xx+u0yy+u0zz
  v0Lap=v0xx+v0yy+v0zz

 nuT=nu
 nuTx=0. 
 nuTy=0. 
 nuTz=0. 
 nuTxx=0. 
 nuTxy=0. 
 nuTyy=0. 
 nuTxz=0.
 nuTyz=0.
 nuTzz=0.

 ! FINISH ME
 stop 7766

#End

#endMacro



