!
! Compute the time step dt for the incompressible NS on rectangular AND curvilinear grids
!

! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


#beginMacro beginLoops(e1)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoops(e1)
  end if
 end do
 end do
 end do
#endMacro


! Define macros for the derivatives based on the dimension, order of accuracy and grid-type
#beginMacro defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

#defineMacro U(cc) u(i1,i2,i3,cc)
#defineMacro UU(cc) uu(i1,i2,i3,cc)

#If #DIM == "2"
 #If #ORDER == "2" 
   #If #GRIDTYPE == "rectangular" 
     #defineMacro UX(cc) ux22r(i1,i2,i3,cc)
     #defineMacro UY(cc) uy22r(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx22r(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy22r(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy22r(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian22r(i1,i2,i3,cc)
   #Elif #GRIDTYPE == "curvilinear"
     #defineMacro UX(cc) ux22(i1,i2,i3,cc)
     #defineMacro UY(cc) uy22(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx22(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy22(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy22(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian22(i1,i2,i3,cc)

     #defineMacro RXX() rxx22(i1,i2,i3)
     #defineMacro RXY() rxy22(i1,i2,i3)
     #defineMacro RYY() ryy22(i1,i2,i3)
     #defineMacro SXX() sxx22(i1,i2,i3)
     #defineMacro SXY() sxy22(i1,i2,i3)
     #defineMacro SYY() syy22(i1,i2,i3)
   #Else
     stop 888
   #End
 #Else
   #If #GRIDTYPE == "rectangular" 
     #defineMacro UX(cc) ux42r(i1,i2,i3,cc)
     #defineMacro UY(cc) uy42r(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx42r(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy42r(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy42r(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian42r(i1,i2,i3,cc)
   #Elif #GRIDTYPE == "curvilinear"
     #defineMacro UX(cc) ux42(i1,i2,i3,cc)
     #defineMacro UY(cc) uy42(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx42(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy42(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy42(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian42(i1,i2,i3,cc)

     #defineMacro RXX() rxx42(i1,i2,i3)
     #defineMacro RXY() rxy42(i1,i2,i3)
     #defineMacro RYY() ryy42(i1,i2,i3)
     #defineMacro SXX() sxx42(i1,i2,i3)
     #defineMacro SXY() sxy42(i1,i2,i3)
     #defineMacro SYY() syy42(i1,i2,i3)
   #Else
     stop 888
   #End
 #End
#Else
 #If #ORDER == "2" 
   #If #GRIDTYPE == "rectangular" 
     #defineMacro UX(cc) ux23r(i1,i2,i3,cc)
     #defineMacro UY(cc) uy23r(i1,i2,i3,cc)
     #defineMacro UZ(cc) uz23r(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx23r(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy23r(i1,i2,i3,cc)
     #defineMacro UXZ(cc) uxz23r(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy23r(i1,i2,i3,cc)
     #defineMacro UYZ(cc) uyz23r(i1,i2,i3,cc)
     #defineMacro UZZ(cc) uzz23r(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian23r(i1,i2,i3,cc)
   #Elif #GRIDTYPE == "curvilinear"
     #defineMacro UX(cc) ux23(i1,i2,i3,cc)
     #defineMacro UY(cc) uy23(i1,i2,i3,cc)
     #defineMacro UZ(cc) uz23(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx23(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy23(i1,i2,i3,cc)
     #defineMacro UXZ(cc) uxz23(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy23(i1,i2,i3,cc)
     #defineMacro UYZ(cc) uyz23(i1,i2,i3,cc)
     #defineMacro UZZ(cc) uzz23(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian23(i1,i2,i3,cc)

     #defineMacro RXX() rxx23(i1,i2,i3)
     #defineMacro RXY() rxy23(i1,i2,i3)
     #defineMacro RXZ() rxz23(i1,i2,i3)
     #defineMacro RYY() ryy23(i1,i2,i3)
     #defineMacro RYZ() ryz23(i1,i2,i3)
     #defineMacro RZZ() rzz23(i1,i2,i3)
                       
     #defineMacro SXX() sxx23(i1,i2,i3)
     #defineMacro SXY() sxy23(i1,i2,i3)
     #defineMacro SXZ() sxz23(i1,i2,i3)
     #defineMacro SYY() syy23(i1,i2,i3)
     #defineMacro SYZ() syz23(i1,i2,i3)
     #defineMacro SZZ() szz23(i1,i2,i3)
                       
     #defineMacro TXX() txx23(i1,i2,i3)
     #defineMacro TXY() txy23(i1,i2,i3)
     #defineMacro TXZ() txz23(i1,i2,i3)
     #defineMacro TYY() tyy23(i1,i2,i3)
     #defineMacro TYZ() tyz23(i1,i2,i3)
     #defineMacro TZZ() tzz23(i1,i2,i3)

   #Else
     stop 888
   #End

 #Else

   #If #GRIDTYPE == "rectangular" 
     #defineMacro UX(cc) ux43r(i1,i2,i3,cc)
     #defineMacro UY(cc) uy43r(i1,i2,i3,cc)
     #defineMacro UZ(cc) uz43r(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx43r(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy43r(i1,i2,i3,cc)
     #defineMacro UXZ(cc) uxz43r(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy43r(i1,i2,i3,cc)
     #defineMacro UYZ(cc) uyz43r(i1,i2,i3,cc)
     #defineMacro UZZ(cc) uzz43r(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian43r(i1,i2,i3,cc)
   #Elif #GRIDTYPE == "curvilinear"
     #defineMacro UX(cc) ux43(i1,i2,i3,cc)
     #defineMacro UY(cc) uy43(i1,i2,i3,cc)
     #defineMacro UZ(cc) uz43(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx43(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy43(i1,i2,i3,cc)
     #defineMacro UXZ(cc) uxz43(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy43(i1,i2,i3,cc)
     #defineMacro UYZ(cc) uyz43(i1,i2,i3,cc)
     #defineMacro UZZ(cc) uzz43(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian43(i1,i2,i3,cc)

     #defineMacro RXX() rxx43(i1,i2,i3)
     #defineMacro RXY() rxy43(i1,i2,i3)
     #defineMacro RXZ() rxz43(i1,i2,i3)
     #defineMacro RYY() ryy43(i1,i2,i3)
     #defineMacro RYZ() ryz43(i1,i2,i3)
     #defineMacro RZZ() rzz43(i1,i2,i3)
                       
     #defineMacro SXX() sxx43(i1,i2,i3)
     #defineMacro SXY() sxy43(i1,i2,i3)
     #defineMacro SXZ() sxz43(i1,i2,i3)
     #defineMacro SYY() syy43(i1,i2,i3)
     #defineMacro SYZ() syz43(i1,i2,i3)
     #defineMacro SZZ() szz43(i1,i2,i3)
                       
     #defineMacro TXX() txx43(i1,i2,i3)
     #defineMacro TXY() txy43(i1,i2,i3)
     #defineMacro TXZ() txz43(i1,i2,i3)
     #defineMacro TYY() tyy43(i1,i2,i3)
     #defineMacro TYZ() tyz43(i1,i2,i3)
     #defineMacro TZZ() tzz43(i1,i2,i3)
   #Else
     stop 888
   #End
 #End
#End
#endMacro 


! =============================================================
! Compute derivatives of u,v,w 
! =============================================================
#beginMacro setupDerivatives(DIM)
 u0x=UX(uc)
 u0y=UY(uc)
 v0x=UX(vc)
 v0y=UY(vc)
#If #DIM == "3"
 u0z=UZ(uc)
 v0z=UZ(vc)
 w0x=UX(wc)
 w0y=UY(wc)
 w0z=UZ(wc)
#End
#endMacro

! =============================================================
! Compute Spalart-Allmaras quantities
!   This macro assumes u0x,u0y, ... are defined
! =============================================================
#beginMacro setupSpalartAllmaras(DIM)

 setupDerivatives(DIM)

 n0=u(i1,i2,i3,nc)
 chi=n0/nu
 chi3=chi**3
 fnu1=chi3/( chi3+cv1e3)
 fnu2=1.-chi/(1.+chi*fnu1)
 dd = dw(i1,i2,i3)+cd0
 dKappaSq=(dd*kappa)**2
#If #DIM == "2"
  s=abs(u0y-v0x)+ n0*fnu2/dKappaSq ! turbulence source term 
#Else
  s=n0*fnu2/dKappaSq \
    +sqrt( (u0y-v0x)**2 + (v0z-w0y)**2 + (w0x-u0z)**2 )
#End
 r= min( n0/( s*dKappaSq ), cr0 )
 g=r+cw2*(r**6-r)
 fw=g*( (1.+cw3e6)/(g**6+cw3e6) )**(1./6.)
 nSqBydSq=cw1*fw*(n0/dd)**2
 nuT = nu+n0*chi3/(chi3+cv1e3)
 nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
 n0x=UX(nc)
 n0y=UY(nc)
 nuTx=n0x*nuTd
 nuTy=n0y*nuTd
 #If #DIM == "3" 
   n0z=UZ(nc)
   nuTz=n0z*nuTd
 #End
#endMacro



! ============== from inspf.bf ***
! Return nuT and it's first derivatives for SPAL
#beginMacro getSpalartAllmarasEddyViscosityAndFirstDerivatives(DIM)
  chi=u(i1,i2,i3,nc)/nu
  chi3= chi**3
  nuT = nu+u(i1,i2,i3,nc)*chi3/(chi3+cv1e3)
  nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2

  n0x=UX(nc)
  n0y=UY(nc)

  nuTx=n0x*nuTd
  nuTy=n0y*nuTd

  #If #DIM == "3" 
    n0z=UZ(nc)
    nuTz=n0z*nuTd
  #End
  #If #DIM == "2"
    s=abs(UY(uc)-UX(vc))
  #Else
    s=sqrt( (UY(uc)-UX(vc))**2 + (UZ(vc)-UY(wc))**2 + (UX(wc)-UZ(uc))**2 )
  #End

#endMacro

! ============== from inspf.bf ***
! Return nuT and it's first derivatives for BL
#beginMacro getBaldwinLomaxEddyViscosityAndFirstDerivatives(DIM)

  nuT = u(i1,i2,i3,nc)

  nuTx=UX(nc)
  nuTy=UY(nc)

  #If #DIM == "3" 
    nuTz=UZ(nc)
  #End
#endMacro

! ============== from inspf.bf ***
! Return nuT and it's first derivatives for KE
#beginMacro getKEpsilonViscosityAndFirstDerivatives(DIM)
 k0=u(i1,i2,i3,kc)
 e0=u(i1,i2,i3,ec)
 nuT = nu + cMu*k0**2/e0
 k0x=UX(kc)
 e0x=UX(ec)
 nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
 k0y=UY(kc)
 e0y=UY(ec)
 nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2
 #If #DIM == "3" 
  k0z=UZ(kc)
  e0z=UZ(ec)
  nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
 #End
#endMacro

! ============== from inspf.bf ***
! Return the visco-plastic viscosity and it's first derivatives for BL
#beginMacro getViscoPlasticViscosityAndFirstDerivatives(DIM)

  nuT = u(i1,i2,i3,vsc)
  nuTx=UX(vsc)
  nuTy=UY(vsc)

  #If #DIM == "3" 
    nuTz=UZ(vsc)
  #End

#endMacro

#beginMacro getEddyViscosityAndFirstDerivatives(SOLVER,DIM)

 #If #SOLVER == "INSSPAL"
   setupSpalartAllmaras(DIM)
 #Elif #SOLVER == "INSBL"
   getBaldwinLomaxEddyViscosityAndFirstDerivatives(DIM)
 #Elif #SOLVER == "INSKE"
   getKEpsilonViscosityAndFirstDerivatives(DIM)
 #Elif #SOLVER == "INSVP" || #SOLVER == "INSTP"
   getViscoPlasticViscosityAndFirstDerivatives(DIM)
 #Else
   write(*,'("insdts:ERROR: unknown solver= SOLVER ")')
   stop 987 
 #End

#endMacro

!====================================================================================
!
! SOLVER: INS, INSSPAL, INSBL, INSKE
! METHOD: GLOBAL, LOCAL  (GLOBAL=fixed time step, LOCAL=local-time stepping -> compute dtVar)
! OPTION: EXPLICIT, IMPLICIT
! ADTYPE: AD2, AD4, AD24 --- NOT USED NOW
! ORDER: 2,4
! DIM: 2,3
! GRIDTYPE: rectangular, curvilinear
! AXISYMMETRIC: notAxisymmetric, or axisymmetric
!====================================================================================
#beginMacro getTimeSteppingEigenvalues(SOLVER,METHOD,OPTION,ADTYPE,ORDER,DIM,GRIDTYPE,AXISYMMETRIC)

defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
! scale factors for 2nd and fourth order:
#If #ORDER == "2"
  cr = 1.      ! [-1  1 ]/ 2 
  crr = 4.     ! [1 -2 1  ]/ 1
#Elif #ORDER == "4"
  cr = 20./12. ! [1 8 8 1  ]/ 12 
  crr= 64./12. ! [1  16  30  16  1  ]/12
#Else
  stop 676
#End

#If #OPTION == "IMPLICIT"
if( useNewImplicitMethod.eq.1 )then
  if( implicitVariation.eq.implicitAdvectionAndViscous .or.implicitVariation.eq.implicitFullLinearized )then
    ! this is a guess: 
    cr=cr*max(.5-implicitFactor,0.)
    crr=crr*max(.5-implicitFactor,0.)
    ! write(*,'("insdts: implicit: get dt: scale advection terms by cr,crr=",2e10.2)') cr,crr
    ! '
  end if
end if
#End

imLambda=0.
reLambda=0.

dtVarMin=1.e22  ! we need a REAL_MAX for fortran
dtVarMax=0.

! ...............................................
beginLoops()

#If #SOLVER == "INSSPAL" || #SOLVER == "INSBL" || #SOLVER == "INSKE" || #SOLVER == "INSVP" || #SOLVER == "INSTP" 
  getEddyViscosityAndFirstDerivatives(SOLVER,DIM)
#End

#If #GRIDTYPE == "rectangular"

  #If #DIM == "2" 
    imPart= cr*( abs(UU(uc))/dx(0)+abs(UU(vc))/dx(1) )
  #Else
    imPart= cr*( abs(UU(uc))/dx(0) + abs(UU(vc))/dx(1) + abs(UU(wc))/dx(2) )
  #End

  #If #SOLVER == "INSSPAL" 
    #If #DIM == "2"
      imPart= imPart+ cr*(1.+cb2)*sigmai*(abs(nuTx)/dx(0)+abs(nuTy)/dx(1) )
    #Else
      imPart= imPart+ cr*(1.+cb2)*sigmai*( abs(nuTx)/dx(0)+abs(nuTy)/dx(1)+abs(nuTz)/dx(2) )
    #End
  #Elif #SOLVER == "INSBL" || #SOLVER == "INSKE" || #SOLVER == "INSVP" || #SOLVER == "INSTP"
    ! check this 
    #If #DIM == "2"
      imPart= imPart+ cr*( abs(nuTx)/dx(0)+abs(nuTy)/dx(1) )
    #Else
      imPart= imPart+ cr*( abs(nuTx)/dx(0)+abs(nuTy)/dx(1)+abs(nuTz)/dx(2) )
    #End
  #End

  #If #OPTION == "EXPLICIT"
   #If #SOLVER == "INS" 
    if( materialFormat.eq.constantMaterialProperties )then
      ! const material properties -- nuk has been set
    else if( materialFormat.eq.piecewiseConstantMaterialProperties )then
      ! piecewise constant material properties
      nuk=max( nu, thermalKpc(i1,i2,i3)/( rhopc(i1,i2,i3)*Cppc(i1,i2,i3) ) )
      ! write(*,'(" (i1,i2)=",i3,",",i3,") Kpc = ",e10.2)') i1,i2,thermalKpc(i1,i2,i3)/( rhopc(i1,i2,i3)*Cppc(i1,i2,i3) )
    else if( materialFormat.eq.variableMaterialProperties )then 
      ! variable material properties
      nuk=max( nu, thermalKv(i1,i2,i3)/( rhov(i1,i2,i3)*Cpv(i1,i2,i3) ) )
      ! write(*,'(" (i1,i2)=",i3,",",i3,") Kv = ",e10.2)') i1,i2,thermalKv(i1,i2,i3)/( rhov(i1,i2,i3)*Cpv(i1,i2,i3) )
    end if
    #If #DIM == "2"
      rePart= crr*nuk*( 1./dx(0)**2 + 1./dx(1)**2 )
    #Else
      rePart= crr*nuk*( 1./dx(0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
    #End
    #If #AXISYMMETRIC == "axisymmetric"
      yy=yc(i2)
      if( abs(yy).ge.dx(1) )then
        imPart=imPart + cr*nuk*( 1./(yy*dx(1)) )  ! u.y/y 
      else
        rePart=rePart + crr*nuk*( 1./dx(1)**2 )   ! u.yy
      end if
    #Elif #AXISYMMETRIC == "notAxisymmetric"
    #Else
      stop 77542
    #End

   #Elif #SOLVER == "INSSPAL" || #SOLVER == "INSBL" || #SOLVER == "INSKE" || #SOLVER == "INSVP" || #SOLVER == "INSTP"
     ! "
     #If #SOLVER == "INSSPAL"
       nuMax= max( nu+nuT, sigmai*(nu+n0) )
     #Elif #SOLVER == "INSKE" 
       nuMax= max( nu+nuT, nu+sigmaKI*nuT, nu+sigmaEpsI*nuT )
     #Elif #SOLVER == "INSTP"
       nuMax=max(adPsi,nuT/u(i1,i2,i3,rc))  
     #Else
       nuMax= nu+nuT
     #End

      ! **** finish this ****
     #If #DIM == "2"
      rePart= crr*nuMax*( 1./dx(0)**2 + 1./dx(1)**2 )
     #Else
      rePart= crr*nuMax*( 1./dx(0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
     #End

   #Else
     ! unknown solver
     stop 234
   #End

  #Elif  #OPTION == "IMPLICIT"
    rePart=0.
    if( pdeModel.eq.BoussinesqModel )then
     ! T is also done implicitly now: 070704
     ! If #DIM == "2"
     !   rePart= crr*kThermal*( 1./dx(0)**2 + 1./dx(1)**2 )
     ! Else
     !   rePart= crr*kThermal*( 1./dx(0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
     ! End
     ! #If #AXISYMMETRIC == "axisymmetric"
     !   rePart=rePart + crr*kThermal*( 1./dx(1)**2 )   ! u.yy
     ! #Elif #AXISYMMETRIC == "notAxisymmetric"
     ! #Else
     !   stop 77542
     ! #End
    end if
   
    #If #SOLVER == "INSSPAL" 
      ! we need to add this for the steady state solver
      ! rePart=rePart+ max(0., cb1*s - cw1*fw*n0/dd**2)
      ! rePart=rePart+ max(0., cb1*s - .5*cw1*fw*n0/dd**2)
      ! add a factor of 2
      ! rePart=rePart+ 2.*cb1*s
      rePart=rePart+ max(0., 2.*cb1*s - .5*cw1*fw*n0/dd**2)
    #End
  #Else
    ! unknown option
    stop 112233
  #End   

#Elif #GRIDTYPE == "curvilinear"

  #If #DIM == "2"
    a1   = UU(uc)*rx(i1,i2,i3)+UU(vc)*ry(i1,i2,i3)
    a2   = UU(uc)*sx(i1,i2,i3)+UU(vc)*sy(i1,i2,i3)
  #Else
    a1   = UU(uc)*rx(i1,i2,i3)+UU(vc)*ry(i1,i2,i3)+UU(wc)*rz(i1,i2,i3)
    a2   = UU(uc)*sx(i1,i2,i3)+UU(vc)*sy(i1,i2,i3)+UU(wc)*sz(i1,i2,i3)
    a3   = UU(uc)*tx(i1,i2,i3)+UU(vc)*ty(i1,i2,i3)+UU(wc)*tz(i1,i2,i3)
  #End

  #If #SOLVER == "INSSPAL" 
    #If #DIM == "2"
      a1=a1 - (1.+cb2)*sigmai*(nuTx*rx(i1,i2,i3)+nuTy*ry(i1,i2,i3))
      a2=a2 - (1.+cb2)*sigmai*(nuTx*sx(i1,i2,i3)+nuTy*sy(i1,i2,i3))
    #Else
      a1=a1 - (1.+cb2)*sigmai*(nuTx*rx(i1,i2,i3)+nuTy*ry(i1,i2,i3)+nuTz*rz(i1,i2,i3))
      a2=a2 - (1.+cb2)*sigmai*(nuTx*sx(i1,i2,i3)+nuTy*sy(i1,i2,i3)+nuTz*sz(i1,i2,i3))
      a3=a3 - (1.+cb2)*sigmai*(nuTx*tx(i1,i2,i3)+nuTy*ty(i1,i2,i3)+nuTz*tz(i1,i2,i3))
    #End
  #Elif #SOLVER == "INSBL" || #SOLVER == "INSKE" || #SOLVER == "INSVP" || #SOLVER == "INSTP"
    ! check this 
    #If #DIM == "2"
      a1=a1 - (nuTx*rx(i1,i2,i3)+nuTy*ry(i1,i2,i3))
      a2=a2 - (nuTx*sx(i1,i2,i3)+nuTy*sy(i1,i2,i3))
    #Else     
      a1=a1 - (nuTx*rx(i1,i2,i3)+nuTy*ry(i1,i2,i3)+nuTz*rz(i1,i2,i3))
      a2=a2 - (nuTx*sx(i1,i2,i3)+nuTy*sy(i1,i2,i3)+nuTz*sz(i1,i2,i3))
      a3=a3 - (nuTx*tx(i1,i2,i3)+nuTy*ty(i1,i2,i3)+nuTz*tz(i1,i2,i3))
    #End
  #End

  #If #OPTION == "EXPLICIT"
   #If #SOLVER == "INS" 
    if( materialFormat.eq.constantMaterialProperties )then
      ! const material properties -- nuk has been set
    else if( materialFormat.eq.piecewiseConstantMaterialProperties )then
      ! piecewise constant material properties
      nuk=max( nu, thermalKpc(i1,i2,i3)/( rhopc(i1,i2,i3)*Cppc(i1,i2,i3) ) )
    else if( materialFormat.eq.variableMaterialProperties )then 
      ! variable material properties
      nuk=max( nu, thermalKv(i1,i2,i3)/( rhov(i1,i2,i3)*Cpv(i1,i2,i3) ) )
    end if
    if( pdeModel.eq.twoPhaseFlowModel )then
      nuk=max(adPsi,u(i1,i2,i3,vsc)/u(i1,i2,i3,rc))  ! do this for now 
    end if
    #If #DIM == "2"
      a1=a1 -nuk*( RXX() + RYY() )
      a2=a2 -nuk*( SXX() + SYY() )
      rePart = nuk*( \
                 ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(0))) \
             +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3) )*( cr/(dr(0)*dr(1))) \
                +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )

     #If #AXISYMMETRIC == "axisymmetric"
      yy=xy(i1,i2,i3,1)
      if( abs(yy).gt.yEps )then
        ! u.y/y 
        a1 = a1 -nuk*( ry(i1,i2,i3)/yy ) 
        a2 = a2 -nuk*( sy(i1,i2,i3)/yy ) 
      else
        ! u.yy 
        rePart= rePart+nuk*(  \
                 (                            ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(0))) \
             +abs(                           ry(i1,i2,i3)*sy(i1,i2,i3) )*( cr/(dr(0)*dr(1))) \
                +(                            sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )
      end if
     #Elif #AXISYMMETRIC == "notAxisymmetric"
     #Else
       stop 77542
     #End
    #Else
      a1=a1 -nuk*( RXX() + RYY() + RZZ())
      a2=a2 -nuk*( SXX() + SYY() + SZZ() )
      a3=a3 -nuk*( TXX() + TYY() + TZZ() )

      rePart = nuk*( \
                 ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*dr(0))) \
                +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         + sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  \
                +( tx(i1,i2,i3)**2          + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*dr(2)))  \
        +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) \
        +abs( rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(0)*dr(2))) \
        +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(1)*dr(2))) \
                )

    #End
   #Elif #SOLVER == "INSSPAL" || #SOLVER == "INSBL" || #SOLVER == "INSKE" || #SOLVER == "INSVP" || #SOLVER == "INSTP" 
     ! "
     #If #SOLVER == "INSSPAL"
       nuMax= max( nu+nuT, sigmai*(nu+n0) )
     #Elif #SOLVER == "INSKE" 
       nuMax= max( nu+nuT, nu+sigmaKI*nuT, nu+sigmaEpsI*nuT )
     #Elif #SOLVER == "INSTP"
       nuMax=max(adPsi,nuT/u(i1,i2,i3,rc))  
     #Else
       nuMax= nu+nuT
     #End

      ! **** finish this ****
     #If #DIM == "2"

      a1=a1 -nuMax*( RXX() + RYY() ) 
      a2=a2 -nuMax*( SXX() + SYY() )
      rePart = nuMax*( \
                 ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(0))) \
             +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3) )*( cr/(dr(0)*dr(1))) \
                +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )

     #Else
      a1=a1 -nuMax*( RXX()+RYY()+RZZ() ) 
      a2=a2 -nuMax*( SXX()+SYY()+SZZ() )
      a3=a3 -nuMax*( TXX()+TYY()+TZZ() )

      rePart = nuMax*( \
                 ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*dr(0))) \
                +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         + sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  \
                +( tx(i1,i2,i3)**2          + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*dr(2)))  \
        +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) \
        +abs( rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(0)*dr(2))) \
        +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(1)*dr(2))) \
                )

     #End

   #Else
     ! known solver
     stop 234
   #End

  #Elif  #OPTION == "IMPLICIT"

    rePart=0.
    if( pdeModel.eq.BoussinesqModel )then
      ! T is also done implicitly now: 070704
      ! kThermal*Delta T terms
      #If #DIM == "2"
      !   a1=a1 -kThermal*( RXX() + RYY() )
      !   a2=a2 -kThermal*( SXX() + SYY() )
      !   rePart = kThermal*( \
      !              ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(0))) \
      !          +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3) )*( cr/(dr(0)*dr(1))) \
      !             +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )
      ! 
      ! *wdh* 080718 -- axisym case for T is also symmetric
      !  #If #AXISYMMETRIC == "axisymmetric"
      !   yy=xy(i1,i2,i3,1)
      !   if( abs(yy).gt.yEps )then
      !   else
      !     ! u.yy 
      !     rePart= rePart+kThermal*(  \
      !              (                            ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(0))) \
      !          +abs(                           ry(i1,i2,i3)*sy(i1,i2,i3) )*( cr/(dr(0)*dr(1))) \
      !             +(                            sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )
      !   end if
      !  #Elif #AXISYMMETRIC == "notAxisymmetric"
      !  #Else
      !    stop 77542
      !  #End
      #Else
      !   a1=a1 -kThermal*( RXX() + RYY() + RZZ())
      !   a2=a2 -kThermal*( SXX() + SYY() + SZZ() )
      !   a3=a3 -kThermal*( TXX() + TYY() + TZZ() )
      ! 
      !   rePart = kThermal*( \
      !              ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*dr(0))) \
      !             +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         + sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  \
      !             +( tx(i1,i2,i3)**2          + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*dr(2)))  \
      !     +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) \
      !     +abs( rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(0)*dr(2))) \
      !     +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(1)*dr(2))) \
      !             )
  
      #End
    end if
    #If #SOLVER == "INSSPAL" 
      ! rePart=rePart+ max(0., cb1*s - .5*cw1*fw*n0/dd**2)
      ! add a factor of 2
      ! rePart=rePart+ 2.*cb1*s
      rePart=rePart+ max(0., 2.*cb1*s - .5*cw1*fw*n0/dd**2)
    #End

  #End

  #If #DIM == "2"
    imPart=cr*(abs(a1)/dr(0)+abs(a2)/dr(1)) 
  #Else
    imPart = cr*( abs(a1)/dr(0)+abs(a2)/dr(1)+abs(a3)/dr(2) )
  #End
#End

#If #OPTION == "EXPLICIT"
  if( use2ndOrderAD.eq.1 )then 
    #If #DIM == "2"
      #If #SOLVER == "INSSPAL"
        adINS = 8.*( ad21 + cd22*( abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)) ) )
        adSPAL= 8.*( ad21n + cd22n*( abs(UX(nc))+abs(UY(nc)) ) )
        rePart=rePart + max( adINS, adSPAL )
      #Else
        rePart=rePart + 8.*( ad21 + cd22*( abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)) ) )
      #End
    #Else
      #If #SOLVER == "INSSPAL"
        adINS = 12.*( ad21 + cd22*( \
         abs(UX(uc))+abs(UX(vc))+abs(UX(wc))+abs(UY(uc))+abs(UY(vc))+abs(UY(wc))+abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc))) )
        adSPAL = 12.*( ad21n + cd22n*( abs(UX(nc))+abs(UY(nc))+abs(UZ(nc)) ) )
        rePart=rePart + max( adINS, adSPAL )
      #Else
        rePart=rePart + 12.*( ad21 + cd22*( \
         abs(UX(uc))+abs(UX(vc))+abs(UX(wc))+abs(UY(uc))+abs(UY(vc))+abs(UY(wc))+abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc))) )
      #End
    #End
  end if
  if(  use4thOrderAD.eq.1 )then 
    #If #DIM == "2"
      rePart=rePart + 32.*( ad41 + cd42*(abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc))) )
    #Else
      rePart=rePart + 48.*( ad21 + cd22*( \
        abs(UX(uc))+abs(UX(vc))+abs(UX(wc))+abs(UY(uc))+abs(UY(vc))+abs(UY(wc))+abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc))) )
    #End
  end if
#End

#If #METHOD == "GLOBAL" 
! write(*,'("BEFORE: i1,i2,i3,imPart,rePart=",3i3,f6.2,2(e11.3,1x))') i1,i2,i3,imPart,rePart

!  correct for divergence damping term
 #If #OPTION == "EXPLICIT"
   rePart = rePart + scaleFactor*divDamping(i1,i2,i3)
 !  write(*,'(" i1,i2,i3,cdv,divDamping,rePart=",3i3,f6.2,2(e11.3,1x))') i1,i2,i3,cdv,divDamping(i1,i2,i3),rePart
 #Elif #OPTION == "IMPLICIT"
!   In the implicit case we limit the size of the divergence damping by cdt/dt
!   *** this may be different from the C++ version ** 
   rePart = rePart + imPart*cDt*factor
   imPart = imPart*(1.+cDt*factor)
 #End

! write(*,'("AFTER: i1,i2,i3,imPart,rePart=",3i3,f6.2,2(e11.3,1x))') i1,i2,i3,imPart,rePart
#End

#If #METHOD == "LOCAL"
  if( implicitMethod.eq.lineImplicit )then               ! line-implicit
    dtVar(i1,i2,i3)=1./max( dtMaxInverse, sqrt( imPart**2 + rePart**2 ) )
  else
    dtVar(i1,i2,i3)=1./sqrt( imPart**2 + rePart**2 )      ! explicit local time stepping
  end if
  dtVarMin=min(dtVarMin,dtVar(i1,i2,i3))
  dtVarMax=max(dtVarMax,dtVar(i1,i2,i3))
#End

 reLambda=max(reLambda,rePart)
 imLambda=max(imLambda,imPart)


endLoops()

#endMacro

! ====================================================================================
! ====================================================================================
#beginMacro getTimeSteppingEigenvaluesByGridType(SOLVER,METHOD,OPTION,ADTYPE,ORDER,DIM)

 if( isAxisymmetric.eq.0 )then
  if( gridType.eq.rectangular )then
    getTimeSteppingEigenvalues(SOLVER,METHOD,OPTION,ADTYPE,ORDER,DIM,rectangular,notAxisymmetric)
  else if(  gridType.eq.curvilinear )then
    getTimeSteppingEigenvalues(SOLVER,METHOD,OPTION,ADTYPE,ORDER,DIM,curvilinear,notAxisymmetric)
  else
    stop 123
  end if
 else
  ! axisymmetric is only for INS for now
  #If #SOLVER == "INS"
   if( gridType.eq.rectangular )then
     getTimeSteppingEigenvalues(SOLVER,METHOD,OPTION,ADTYPE,ORDER,DIM,rectangular,axisymmetric)
   else if(  gridType.eq.curvilinear )then
     getTimeSteppingEigenvalues(SOLVER,METHOD,OPTION,ADTYPE,ORDER,DIM,curvilinear,axisymmetric)
   else
     stop 123
   end if

  #Else
    stop 321
  #End
 end if
#endMacro

#beginMacro getTimeSteppingEigenvaluesByDimension(SOLVER,METHOD,OPTION,ADTYPE,ORDER)
 if( nd.eq.2 )then
   getTimeSteppingEigenvaluesByGridType(SOLVER,METHOD,OPTION,ADTYPE,ORDER,2)
 else if( nd.eq.3 )then
   getTimeSteppingEigenvaluesByGridType(SOLVER,METHOD,OPTION,ADTYPE,ORDER,3)
 else
   stop 123
 end if
#endMacro

#beginMacro getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,ADTYPE)
 if( orderOfAccuracy.eq.2 )then
  getTimeSteppingEigenvaluesByDimension(SOLVER,METHOD,OPTION,ADTYPE,2)
 else if( orderOfAccuracy.eq.4 )then
  ! only INS and INSVP are 4th order for now  
  #If #SOLVER == "INS" || #SOLVER == "INSVP"
    getTimeSteppingEigenvaluesByDimension(SOLVER,METHOD,OPTION,ADTYPE,4)
  #Else
    stop 321
  #End
 else
   stop 123
 end if
#endMacro


#beginMacro getTimeSteppingEigenvaluesByArtificialDissipation(SOLVER,METHOD,OPTION)
! Don't split by ADTYPE since this makes the file too long for no big benefit.
! if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then 
!  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,NONE)
! else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then 
!  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD2)
! else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 )then 
!  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD4)
! else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 )then 
!  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD24)
! else
!   stop 123
! end if
 getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD24) 
#endMacro

#beginMacro getTimeSteppingEigenvaluesByImplicit(SOLVER,METHOD) 
 if( gridIsImplicit.eq.0 )then
  ! -- This grid is treated implicitly --
  getTimeSteppingEigenvaluesByArtificialDissipation(SOLVER,METHOD,EXPLICIT)
 else
  ! -- This grid is treated explicitly --
  getTimeSteppingEigenvaluesByArtificialDissipation(SOLVER,METHOD,IMPLICIT)
 end if
#endMacro

#beginMacro getTimeSteppingEigenvaluesByMethod(SOLVER) 
 if( useLocalTimeStepping.eq.0 )then
  ! -- Use a single global dt --
  getTimeSteppingEigenvaluesByImplicit(SOLVER,GLOBAL)
 else
  ! -- Use a local dt --
  getTimeSteppingEigenvaluesByImplicit(SOLVER,LOCAL)
 end if
#endMacro

! This is not finished yet..
#beginMacro computeAxisymmetricCorrection(DIM,ORDER,GRIDTYPE)
defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
beginLoops()
! y corresponds to the radial direction
! y=0 is the axis of symmetry
!   nu*(  u.xx + u.yy + (1/y) u.y )
!   nu*(  v.xx + v.yy + (1/y) v.y - v/y^2 ) 
 #If #GRIDTYPE == "rectangular"
   radiusInverse=1./max(REAL_MIN,YY(i2))
 #Else
   radiusInverse=1./max(REAL_MIN,vertex(i1,i2,i3,1))
 #End 
 endLoops()
 urOverR(i1,i2,i3)=UY(uc)*radiusInverse
 vrOverR(i1,i2,i3)=(UY(vc)-U(vc)*radiusInverse)*radiusInverse
 do axis=0,nd-1
 do side=0,1
  if( boundaryCondition(side,axis).eq.axisymmetric )then
    getBoundaryIndex(mg.gridIndexRange(),side,axis)
    beginLoops()
      urOverR(i1,i2,i3)=UYY(uc)
      vrOverR(i1,i2,i3)=.5*UYY(vc)
    endLoops()
  end if
 end do
 end do
#endMacro


! ============================================================================================================
!  Define the subroutine that compute the eigenvalues for a given solver
! ============================================================================================================
#beginMacro INSDTS(SOLVER,NAME)
 subroutine NAME(nd, n1a,n1b,n2a,n2b,n3a,n3b, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
     mask,xy, rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal,\
     bc, ipar, rpar, pdb, ierr )
!======================================================================
!
!    Determine the time step for the INS equations.
!    ---------------------------------------------
!
! nd : number of space dimensions
!
! gv : gridVelocity for moving grids
! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
! dw : distance to the wall for some turbulence models
!======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

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

 !   ---- local variables -----
 integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
 integer gridIsImplicit,implicitOption,implicitMethod,isAxisymmetric
 integer use2ndOrderAD,use4thOrderAD,useLocalTimeStepping
 integer rc,pc,uc,vc,wc,sc,nc,kc,ec,grid,m,advectPassiveScalar,vsc,debug
 real nu,dt,nuPassiveScalar,adcPassiveScalar,kThermal,nuk
 real dtVarMin,dtVarMax,dtMax,dtMaxInverse
 real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
 real ad21,ad22,ad41,ad42,cd22,cd42,adc
 real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
 real adINS,adSPAL
 real scaleFactor,factor,cDt,cdv,cr,crr
 integer i1a,i2a,i3a
 real yy,yEps,xa,ya,za

 integer ok,getInt,getReal

 integer gridType
 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

 integer computeAllTerms,\
     doNotComputeImplicitTerms,\
     computeImplicitTermsSeparately,\
     computeAllWithWeightedImplicit

 parameter( computeAllTerms=0,\
           doNotComputeImplicitTerms=1,\
           computeImplicitTermsSeparately=2,\
           computeAllWithWeightedImplicit=3 )

 integer implicitVariation
 integer implicitViscous, implicitAdvectionAndViscous, implicitFullLinearized
 parameter( implicitViscous=0, \
            implicitAdvectionAndViscous=1, \
            implicitFullLinearized=2 )

 ! These should match those in OB_Parameters.h
 integer notImplicit,backwardEuler,secondOrderBDF,crankNicolson,lineImplicit
 parameter( notImplicit=0,backwardEuler=1,secondOrderBDF=2,crankNicolson=3,lineImplicit=4 )

 real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
 real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
 real udmzC,udzmC,udmzzC,udzmzC,udzzmC
 real admzR,adzmR,admzzR,adzmzR,adzzmR
 real admzC,adzmC,admzzC,adzmzC,adzzmC
 real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
 real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
 real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f

 real delta22,delta23,delta42,delta43

 real adCoeff2,adCoeff4

 real ad2,ad23,ad4,ad43
 real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
 real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

 real rx,ry,rz,sx,sy,sz,tx,ty,tz
 real dr(0:2), dx(0:2)

 ! for SPAL TM
 real n0,n0x,n0y,n0z
 real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
 real chi,chi3,fnu1,fnu2,s,r,g,fw,dKappaSq,nSqBydSq,dd
 real nuT,nuTx,nuTy,nuTz,nuTd

 real u0,u0x,u0y,u0z
 real v0,v0x,v0y,v0z
 real w0,w0x,w0y,w0z
 ! for k-epsilon
 real k0,k0x,k0y,k0z, e0,e0x,e0y,e0z
 real nuP,prod
 real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI

 ! two-phase flow
 real adPsi,adPhi
 ! visco-plastic
 ! real nuVP,etaVP,yieldStressVP,exponentVP,epsVP
 ! real eDotNorm,exp0
 ! real u0xx,u0xy,u0yy
 ! real v0xx,v0xy,v0yy

 real rePart,imPart,imLambda,reLambda,a1,a2,a3,nuMax
 real implicitFactor
 integer useNewImplicitMethod

 real rhopc,rhov,   Cppc, Cpv, thermalKpc, thermalKv, Kx, Ky, Kz, Kr, Ks, Kt

 ! include 'declareDiffOrder2f.h'
 ! include 'declareDiffOrder4f.h'
 declareDifferenceOrder2(u,RX)
 declareDifferenceOrder4(u,RX)

 !     --- begin statement functions
 real xc,yc,zc

 rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
 ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
 rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
 sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
 sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
 sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
 tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
 ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
 tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

 ! for cartesian coordinates
 xc(i1) = xa + dx(0)*(i1-i1a)
 yc(i2) = ya + dx(1)*(i2-i2a)
 zc(i3) = za + dx(2)*(i3-i3a)

 !   The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)


 !    --- 2nd order 2D artificial diffusion ---
 ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
           +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

 !    --- 2nd order 3D artificial diffusion ---
 ad23(c)=adc\
    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) \
     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                  
 !     ---fourth-order artificial diffusion in 2D
 ad4(c)=adc\
    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    \
         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    \
     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    \
         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   \
      -12.*u(i1,i2,i3,c) ) 
 !     ---fourth-order artificial diffusion in 3D
 ad43(c)=adc\
    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   \
     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   \
         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  \
      -18.*u(i1,i2,i3,c) )

 !    --- For 2nd order 2D artificial diffusion ---
 delta22(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
              +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
 !    --- For 2nd order 3D artificial diffusion ---
 delta23(c)= \
   (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   \
   +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  \
   +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c)) 
 !     ---For fourth-order artificial diffusion in 2D
 delta42(c)= \
   (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
       -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
       +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  \
    -12.*u(i1,i2,i3,c) ) 
 !     ---For fourth-order artificial diffusion in 3D
 delta43(c)= \
   (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  \
       -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)  \
       -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  \
   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  \
       +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)  \
       +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
    -18.*u(i1,i2,i3,c) )


 #Include "selfAdjointArtificialDiffusion.h"

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

 ierr=0

 pc                 =ipar(0)
 uc                 =ipar(1)
 vc                 =ipar(2)
 wc                 =ipar(3)
 nc                 =ipar(4)
 sc                 =ipar(5)
 grid               =ipar(6)
 orderOfAccuracy    =ipar(7)
 gridIsMoving       =ipar(8)
 useWhereMask       =ipar(9)
 gridIsImplicit     =ipar(10)
 implicitMethod     =ipar(11)
 implicitOption     =ipar(12)
 isAxisymmetric     =ipar(13)
 use2ndOrderAD      =ipar(14)
 use4thOrderAD      =ipar(15)
 advectPassiveScalar=ipar(16)
 gridType           =ipar(17)
 turbulenceModel    =ipar(18)
 useLocalTimeStepping=ipar(19)
 i1a                =ipar(20)
 i2a                =ipar(21)
 i3a                =ipar(22)
 pdeModel           =ipar(23)
 implicitVariation  =ipar(24)
 rc                 =ipar(25) ! for variable density INS
 materialFormat     =ipar(26)

 dr(0)             =rpar(0)
 dr(1)             =rpar(1)
 dr(2)             =rpar(2)
 dx(0)             =rpar(3)
 dx(1)             =rpar(4)
 dx(2)             =rpar(5)
 nu                =rpar(6)
 ad21              =rpar(7)
 ad22              =rpar(8)
 ad41              =rpar(9)
 ad42              =rpar(10)
 nuPassiveScalar   =rpar(11)
 adcPassiveScalar  =rpar(12)
 !     reLambda          =rpar(13) ! returned here
 !     imLambda          =rpar(14) ! returned here
 cDt               =rpar(15)
 cdv               =rpar(16)
 dtMax             =rpar(17)
 ad21n             =rpar(18)
 ad22n             =rpar(19)
 ad41n             =rpar(20)
 ad42n             =rpar(21)
 xa                =rpar(22)
 ya                =rpar(23)
 za                =rpar(24)
 yEps              =rpar(25) ! for axisymmetric y<yEps => y is on the axis
 
! nuVP              =rpar(26)
! etaVP             =rpar(27)
! yieldStressVP     =rpar(28)
! exponentVP        =rpar(29)
! epsVP             =rpar(30)

 kc=nc
 ec=kc+1

 #If #SOLVER eq "INSKE" || #SOLVER eq "INSBL"
   write(*,'("*** insdts SOLVER: Entering.. gridIsImplicit=",i2,"  kc,ec=",2i3)') gridIsImplicit,kc,ec
 #End

 ok = getReal(pdb,'kThermal',kThermal)  
 if( (pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel) .and. ok.eq.0 )then
   write(*,'("*** insdts: ERROR: kThermal NOT FOUND for BoussinesqModel or viscoPlasticModel")') 
   ! '
 else if( pdeModel.eq.BoussinesqModel )then
   ! write(*,'("*** insdts: pdeModel=",i2," kThermal=",e10.2)') pdeModel,kThermal
 end if

 if( pdeModel.eq.BoussinesqModel )then
   ! for Boussinesq base viscous dt on the max of nu and kThermal
   nuk=max(nu,kThermal)
 else
   nuk=nu
 end if


 ok = getInt(pdb,'debug',debug) 
 if( ok.eq.0 )then
  write(*,'("*** insdts: ERROR: debug NOT FOUND")') 
 else
  ! write(*,'("*** insdts: debug=",i6)') debug
 end if

 ok = getReal(pdb,'implicitFactor',implicitFactor) 
 if( ok.eq.0 )then
  write(*,'("*** insdts: ERROR: implicitFactor NOT FOUND")') 
 end if

 ok = getInt(pdb,'useNewImplicitMethod',useNewImplicitMethod) 
 if( ok.eq.0 )then
  write(*,'("*** insdts: ERROR: useNewImplicitMethod NOT FOUND")') 
 else
  ! write(*,'("*** insdts: useNewImplicitMethod=",i6)') useNewImplicitMethod
 end if

 ok = getInt(pdb,'vsc',vsc) 
 if( ok.eq.0 )then
  write(*,'("*** insdts: ERROR: vsc NOT FOUND")') 
 else
  ! write(*,'("*** insdts: vsc=",i6)') vsc
 end if

 if( pdeModel.eq.twoPhaseFlowModel .and. (rc.lt.0 .or. vsc.lt.0) )then
   write(*,'("*** insdts: ERROR: twoPhaseFlowModel but rc<0 or vsc<0 ")') 
   stop 9921
 end if
 if( pdeModel.eq.twoPhaseFlowModel )then 
   ok = getReal(pdb,'PdeParameters/twoPhaseArtDisPsi',adPsi) 
   if( ok.eq.0 )then
     adPsi=.1 ! default value if not supplied 
     write(*,'("*** insdt:ERROR: PdeParameters/twoPhaseArtDisPsi NOT FOUND")') 
     ! ' 
   else
     if( debug.gt.4 )then
      write(*,'("*** insdt:twoPhaseArtDisPsi=",e8.2)') adPsi
     end if
   end if
   ok = getReal(pdb,'PdeParameters/twoPhaseArtDisPhi',adPhi) 
   if( ok.eq.0 )then
     adPsi=.1 ! default value if not supplied 
     write(*,'("*** insdt:ERROR: PdeParameters/twoPhaseArtDisPhi NOT FOUND")') 
     ! ' 
   else
     if( debug.gt.4 )then
      write(*,'("*** insdt:twoPhaseArtDisPhi=",e8.2)') adPhi
     end if
   end if
 end if


! write(*,'("insdts: gridType,gridIsImplicit,implicitMethod,implicitOption,useLocalTimeStepping=",10i3)') gridType,gridIsImplicit,implicitMethod,implicitOption,useLocalTimeStepping


 if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
   write(*,'("insdts:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
   stop 1
 end if
 if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
   write(*,'("insdts:ERROR gridType=",i6)') gridType
   stop 2
 end if
 if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
   write(*,'("insdts:ERROR uc,vc,ws=",3i6)') uc,vc,wc
   stop 4
 end if
 if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. kc.gt.1000) )then
   write(*,'("insdts:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc
   stop 5
 end if
 if( nd.ne.2 .and. nd.ne.3 )then
   write(*,'("insdts:ERROR nd=",i6)') nd
   stop 1
 end if

 if( materialFormat.ne.constantMaterialProperties .and. \
     materialFormat.ne.piecewiseConstantMaterialProperties .and. \
     materialFormat.ne.variableMaterialProperties )then
  write(*,'("insdts:ERROR: Invalid materialFormat=",i6)') materialFormat
  stop 5522
 end if

 ! --- Output rho, Cp and kThermal t=0 for testing ---
 if( .false. .and. materialFormat.ne.0 .and. (nd1b-nd1a)*(nd2b-nd2a).lt. 1000 )then

  write(*,'("insdts: variable material properties rho,Cp,kThermal for T")')
  write(*,'("insdts: rho:")')
  i3=nd3a
  do i2=nd2b,nd2a,-1
    if( materialFormat.eq.piecewiseConstantMaterialProperties )then
     write(*,'(100(f5.1))') (rhopc(i1,i2,i3),i1=nd1a,nd1b)
    else
     write(*,'(100(f5.1))') (rhov(i1,i2,i3),i1=nd1a,nd1b)
    end if
  end do 
  write(*,'("insdts: Cp:")')
  do i2=nd2b,nd2a,-1
    if( materialFormat.eq.piecewiseConstantMaterialProperties )then
     write(*,'(100(f5.1))') (Cppc(i1,i2,i3),i1=nd1a,nd1b)
    else
     write(*,'(100(f5.1))') (Cpv(i1,i2,i3),i1=nd1a,nd1b)
    end if
  end do 
  write(*,'("insdts: thermalConductivity:")')
  do i2=nd2b,nd2a,-1
    if( materialFormat.eq.piecewiseConstantMaterialProperties )then
     write(*,'(100(f5.1))') (thermalKpc(i1,i2,i3),i1=nd1a,nd1b)
    else
     write(*,'(100(f5.1))') (thermalKv(i1,i2,i3),i1=nd1a,nd1b)
    end if
  end do 

 end if

 ! ** these are needed by self-adjoint terms **fix**
 dxi=1./dx(0)
 dyi=1./dx(1)
 dzi=1./dx(2)
 !     dx2i=1./(2.*dx(0))
 !     dy2i=1./(2.*dx(1))
 !     dz2i=1./(2.*dx(2))

 dri=1./dr(0)
 dsi=1./dr(1)
 dti=1./dr(2)
 dr2i=1./(2.*dr(0))
 ds2i=1./(2.*dr(1))
 dt2i=1./(2.*dr(2))

 if( turbulenceModel.eq.spalartAllmaras )then
   call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, \
      cv1e3, cd0, cr0)
 else if( turbulenceModel.eq.kEpsilon )then

  ! write(*,'(" insdts: k-epsilon: nc,kc,ec=",3i3)') nc,kc,ec

   call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
   !  write(*,'(" insdts: cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=",5f8.3)') cMu,cEps1,cEps2,sigmaEpsI,sigmaKI

 else if( turbulenceModel.eq.baldwinLomax .or. \
          turbulenceModel.eq.largeEddySimulation )then

 else if( turbulenceModel.ne.noTurbulenceModel )then
   write(*,'("insdts: unknown turbulenceModel=",i6)') turbulenceModel
   stop 8080
 end if

 adc=adcPassiveScalar ! coefficient of linear artificial diffusion
 cd22=ad22/(nd**2)
 cd42=ad42/(nd**2)
 cd22n=ad22/nd
 cd42n=ad42/nd
 dtMaxInverse=1./dtMax

 !     correction factors for divergence damping term
 !     this is an over estimate ****
 if( cdv.eq.0. )then
   scaleFactor=0.
 else
   scaleFactor = 1.
   if( isAxisymmetric.eq.1 )then
     scaleFactor=2.
   end if
 end if
 factor=1.5*scaleFactor


 if( gridIsMoving.ne.0 )then
   ! compute uu = u -gv
   if( nd.eq.2 )then
     beginLoops()
       uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
       uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
     endLoops()
   else if( nd.eq.3 )then
     beginLoops()
      uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
      uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
      uu(i1,i2,i3,wc)=u(i1,i2,i3,wc)-gv(i1,i2,i3,2)
    endLoops()
   else
     stop 11
   end if
 end if

 !      if( isAxisymmetric.eq.1 )then
 !        computeAxisymmetricCorrection()
 !      end if

 !     *****************************************************
 !     ********DETERMINE THE TIME STEPPING EIGENVALUES *****
 !     *****************************************************      

 getTimeSteppingEigenvaluesByMethod(SOLVER) 

! if( .false. .and. useLocalTimeStepping.eq.1 )then
!   write(*,'(" insdts: local dt, grid=",i3," dtVar (min,max)=(",e10.2,",",e10.2,")")') grid,dtVarMin,dtVarMax
!   ! '
! end if

 rpar(13)=reLambda
 rpar(14)=imLambda

 return
 end

#endMacro


      subroutine insdts(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
        mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal, \
        bc, ipar, rpar, pdb, ierr )
!======================================================================
!
!    Determine the time step for the INS equations.
!    ---------------------------------------------
!
! nd : number of space dimensions
!
! gv : gridVelocity for moving grids
! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
! dw : distance to the wall for some turbulence models
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)
      
      double precision pdb  ! pointer to data base

      ! -- arrays for variable material properties --
      integer materialFormat,ndMatProp
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

!     --- end statement functions

      ierr=0
      ! write(*,'("Inside insdts: gridType=",i2)') gridType

!$$$      pc                 =ipar(0)
!$$$      uc                 =ipar(1)
!$$$      vc                 =ipar(2)
!$$$      wc                 =ipar(3)
!$$$      nc                 =ipar(4)
!$$$      sc                 =ipar(5)
!$$$      grid               =ipar(6)
!$$$      orderOfAccuracy    =ipar(7)
!$$$      gridIsMoving       =ipar(8)
!$$$      useWhereMask       =ipar(9)
!$$$      gridIsImplicit     =ipar(10)
!$$$      implicitMethod     =ipar(11)
!$$$      implicitOption     =ipar(12)
!$$$      isAxisymmetric     =ipar(13)
!$$$      use2ndOrderAD      =ipar(14)
!$$$      use4thOrderAD      =ipar(15)
!$$$      advectPassiveScalar=ipar(16)
!$$$      gridType           =ipar(17)
      turbulenceModel    =ipar(18)
!$$$      useLocalTimeStepping=ipar(19)

      pdeModel           =ipar(23)

!     *****************************************************
!     ********DETERMINE THE TIME STEPPING EIGENVALUES *****
!     *****************************************************      

      if( turbulenceModel.eq.noTurbulenceModel )then

        if( pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel )then
          call insdtsINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
            mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal, \
            bc, ipar, rpar, pdb, ierr )
        else if( pdeModel.eq.viscoPlasticModel )then
          call insdtsVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
            mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal,\
             bc, ipar, rpar, pdb, ierr )
        else if( pdeModel.eq.twoPhaseFlowModel )then
          call insdtsTP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
            mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal,\
            bc, ipar, rpar, pdb, ierr )
        else
          write(*,'("insdts:ERROR::pdeModel=",i2)') pdeModel
          stop 45
        end if

      else if( turbulenceModel.eq.spalartAllmaras )then

        call insdtsSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal,\
          bc, ipar, rpar, pdb, ierr )

      else if( turbulenceModel.eq.baldwinLomax )then

        call insdtsBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal,\
          bc, ipar, rpar, pdb, ierr )

      else if( turbulenceModel.eq.kEpsilon )then

        call insdtsKE(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal,\
          bc, ipar, rpar, pdb, ierr )

      else if( turbulenceModel.eq.largeEddySimulation )then
        ! use the VP model to get the time step here
        call insdtsVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
            mask,xy,rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal,\
            bc, ipar, rpar, pdb, ierr )
      else 
        stop 33
      end if


      return
      end

! 
! : empty version for linking when we don't want an option
!
#beginMacro INSDTS_NULL(SOLVER,NAME)
 subroutine NAME(nd, n1a,n1b,n2a,n2b,n3a,n3b, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
     mask,xy, rsxy,  u,uu, gv,dw, divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal, \
     bc, ipar, rpar, pdb, ierr )
!======================================================================
!       EMPTY VERSION for Linking without this Capability
!
!    Determine the time step for the INS equations.
!    ---------------------------------------------
!
! nd : number of space dimensions
!
! gv : gridVelocity for moving grids
! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
! dw : distance to the wall for some turbulence models
!======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

 integer ipar(0:*)
 real rpar(0:*)

 double precision pdb  ! pointer to data base

 ! -- arrays for variable material properties --
 integer materialFormat,ndMatProp
 integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real matValpc(0:ndMatProp-1,0:*)
 real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

 write(*,'("ERROR: NULL version of subroutine NAME called")')
 write(*,'(" You may have to turn on an option in the Makefile.")')
 ! ' 
 stop 1060

 return
 end
#endMacro


#beginMacro buildFile(SOLVER,NAME)
#beginFile src/NAME.f
 INSDTS(SOLVER,NAME)
#endFile
#beginFile src/NAME ## Null.f
 INSDTS_NULL(SOLVER,NAME)
#endFile
#endMacro


      buildFile(INS,insdtsINS)
      buildFile(INSSPAL,insdtsSPAL)
      buildFile(INSBL,insdtsBL)
      buildFile(INSKE,insdtsKE)
      buildFile(INSVP,insdtsVP)
      buildFile(INSTP,insdtsTP)
      
