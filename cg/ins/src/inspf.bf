! ========================================================================================================
! Assign the RHS for the pressure equation 
! ========================================================================================================


! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
! Include "defineDiffOrder2f.h"
!** #Include "defineDiffNewerOrder2f.h"
!** #Include "defineDiffOrder4f.h"
#Include "defineDiffNewerOrder2f.h"
#Include "defineDiffNewerOrder4f.h"

#beginMacro loopse1(e1)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
      e1
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
  end do
  end do
  end do
end if
#endMacro
#beginMacro loopse2(e1,e2)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
      e1
      e2
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
  end do
  end do
  end do
end if
#endMacro

#beginMacro loopse3(e1,e2,e3)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
      e1
      e2
      e3
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
  end do
  end do
  end do
end if
#endMacro

#beginMacro loopse4(e1,e2,e3,e4)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
      e1
      e2
      e3
      e4
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
  end do
  end do
  end do
end if
#endMacro

#beginMacro loopse6(e1,e2,e3,e4,e5,e6)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
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
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
      e5
      e6
  end do
  end do
  end do
end if
#endMacro



#beginMacro beginLoops()
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
#endMacro
#beginMacro endLoops()
    end if
  end do
  end do
  end do
#endMacro



! Define macros for the derivatives based on the dimension, order of accuracy and grid-type
#beginMacro defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
#If #DIM == "2"
 #If #ORDER == "2" 
   #If #GRIDTYPE == "rectangular" 
     #defineMacro UX(cc) ux22r(i1,i2,i3,cc)
     #defineMacro UY(cc) uy22r(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx22r(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy22r(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy22r(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian22r(i1,i2,i3,cc)
     #defineMacro UXXX(cc) uxxx22r(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy22r(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy22r(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy22r(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx22r(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy22r(i1,i2,i3,cc)

   #Elif #GRIDTYPE == "curvilinear"
     #defineMacro UX(cc) ux22(i1,i2,i3,cc)
     #defineMacro UY(cc) uy22(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx22(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy22(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy22(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian22(i1,i2,i3,cc)
     #defineMacro UXXX(cc) uxxx22(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy22(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy22(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy22(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx22(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy22(i1,i2,i3,cc)
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
     #defineMacro UXXX(cc) uxxx42r(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy42r(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy42r(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy42r(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx42r(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy42r(i1,i2,i3,cc)

   #Elif #GRIDTYPE == "curvilinear"
     #defineMacro UX(cc) ux42(i1,i2,i3,cc)
     #defineMacro UY(cc) uy42(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx42(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy42(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy42(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian42(i1,i2,i3,cc)
     #defineMacro UXXX(cc) uxxx42(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy42(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy42(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy42(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx42(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy42(i1,i2,i3,cc)
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

     #defineMacro UXXX(cc) uxxx23r(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy23r(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy23r(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy23r(i1,i2,i3,cc)
     #defineMacro UXXZ(cc) uxxz23r(i1,i2,i3,cc)
     #defineMacro UXZZ(cc) uxzz23r(i1,i2,i3,cc)
     #defineMacro UZZZ(cc) uzzz23r(i1,i2,i3,cc)
     #defineMacro UYYZ(cc) uyyz23r(i1,i2,i3,cc)
     #defineMacro UYZZ(cc) uyzz23r(i1,i2,i3,cc)
     #defineMacro UXYZ(cc) uxyz23r(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx23r(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy23r(i1,i2,i3,cc)
     #defineMacro UDFZ(cc) udfz23r(i1,i2,i3,cc)

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
     #defineMacro UXXX(cc) uxxx23(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy23(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy23(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy23(i1,i2,i3,cc)
     #defineMacro UXXZ(cc) uxxz23(i1,i2,i3,cc)
     #defineMacro UXZZ(cc) uxzz23(i1,i2,i3,cc)
     #defineMacro UZZZ(cc) uzzz23(i1,i2,i3,cc)
     #defineMacro UYYZ(cc) uyyz23(i1,i2,i3,cc)
     #defineMacro UYZZ(cc) uyzz23(i1,i2,i3,cc)
     #defineMacro UXYZ(cc) uxyz23(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx23(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy23(i1,i2,i3,cc)
     #defineMacro UDFZ(cc) udfz23(i1,i2,i3,cc)

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
     #defineMacro UXXX(cc) uxxx43r(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy43r(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy43r(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy43r(i1,i2,i3,cc)
     #defineMacro UXXZ(cc) uxxz43r(i1,i2,i3,cc)
     #defineMacro UXZZ(cc) uxzz43r(i1,i2,i3,cc)
     #defineMacro UZZZ(cc) uzzz43r(i1,i2,i3,cc)
     #defineMacro UYYZ(cc) uyyz43r(i1,i2,i3,cc)
     #defineMacro UYZZ(cc) uyzz43r(i1,i2,i3,cc)
     #defineMacro UXYZ(cc) uxyz43r(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx43r(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy43r(i1,i2,i3,cc)
     #defineMacro UDFZ(cc) udfz43r(i1,i2,i3,cc)

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
     #defineMacro UXXX(cc) uxxx43(i1,i2,i3,cc)
     #defineMacro UXXY(cc) uxxy43(i1,i2,i3,cc)
     #defineMacro UXYY(cc) uxyy43(i1,i2,i3,cc)
     #defineMacro UYYY(cc) uyyy43(i1,i2,i3,cc)
     #defineMacro UXXZ(cc) uxxz43(i1,i2,i3,cc)
     #defineMacro UXZZ(cc) uxzz43(i1,i2,i3,cc)
     #defineMacro UZZZ(cc) uzzz43(i1,i2,i3,cc)
     #defineMacro UYYZ(cc) uyyz43(i1,i2,i3,cc)
     #defineMacro UYZZ(cc) uyzz43(i1,i2,i3,cc)
     #defineMacro UXYZ(cc) uxyz43(i1,i2,i3,cc)

     ! user defined force:
     #defineMacro UDFX(cc) udfx43(i1,i2,i3,cc)
     #defineMacro UDFY(cc) udfy43(i1,i2,i3,cc)
     #defineMacro UDFZ(cc) udfz43(i1,i2,i3,cc)
   #Else
     stop 888
   #End
 #End
#End
#endMacro 


! Return nuT and its first derivatives for SPAL
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
#endMacro

!$$$c Return nuT and its first derivatives for BL
!$$$#beginMacro getBaldwinLomaxEddyViscosityAndFirstDerivatives(DIM)
!$$$
!$$$  nuT = u(i1,i2,i3,nc)
!$$$
!$$$  nuTx=UX(nc)
!$$$  nuTy=UY(nc)
!$$$
!$$$  #If #DIM == "3" 
!$$$    nuTz=UZ(nc)
!$$$  #End
!$$$#endMacro


!$$$c Return nuT and its first derivatives for KE
!$$$#beginMacro getKEpsilonViscosityAndFirstDerivatives(DIM)
!$$$ k0=u(i1,i2,i3,kc)
!$$$ e0=u(i1,i2,i3,ec)
!$$$ nuT = nu + cMu*k0**2/e0
!$$$ k0x=UX(kc)
!$$$ e0x=UX(ec)
!$$$ nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
!$$$ k0y=UY(kc)
!$$$ e0y=UY(ec)
!$$$ nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2
!$$$ #If #DIM == "3" 
!$$$  k0z=UZ(kc)
!$$$  e0z=UZ(ec)
!$$$  nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
!$$$ #End
!$$$#endMacro

!$$$c Return the visco-plastic viscosity and its first derivatives for BL
!$$$#beginMacro getViscoPlasticViscosityAndFirstDerivatives(DIM)
!$$$
!$$$  nuT = u(i1,i2,i3,vsc)
!$$$  nuTx=UX(vsc)
!$$$  nuTy=UY(vsc)
!$$$
!$$$  #If #DIM == "3" 
!$$$    nuTz=UZ(vsc)
!$$$  #End
!$$$
!$$$#endMacro


! ========================================================================
!   Return the generic nonlinear viscosity and its first derivatives 
! =======================================================================
#beginMacro getNonlinearViscosityAndFirstDerivatives(DIM)

  nuT = u(i1,i2,i3,vsc)
  nuTx=UX(vsc)
  nuTy=UY(vsc)

  #If #DIM == "3" 
    nuTz=UZ(vsc)
  #End

#endMacro


#beginMacro getEddyViscosityAndFirstDerivatives(TYPE,DIM)

 #If #TYPE == "INSSPAL"
   getSpalartAllmarasEddyViscosityAndFirstDerivatives(DIM)
 #Elif #TYPE == "INSBL" || #TYPE == "INSKE" || #TYPE == "INSVP" || #TYPE == "INSTP"
   getNonlinearViscosityAndFirstDerivatives(DIM)
 #Else
   stop 987 
 #End

#endMacro

! ***********************************************************************************
!   This macro defines the statements that sets the pressure BC 
!   as the normal component of the momentum equation.
!   These statement are placed in a loop in the applyWallBC macro.
!
! OPTION: boundary : apply pressure BC on boundary in the "curl-curl" form
!         edge     : apply pressure BC on boundary in the normal form
! TYPE: INS, INSSPAL, INSBL, INSKE
! ADTYPE: noAD, AD2, AD4, AD24
! GRIDTYPE: rectangular, curvilinear
! DIM: 2,3
! AXIS: R,S,T
!
! ***********************************************************************************
#beginMacro setPressureBoundaryCondition(OPTION,TYPE,ADTYPE,GRIDTYPE,DIM,AXIS,normal)
 ! Define derivative macros before calling this macro

 ! By default there is no AD:
 #defineMacro artificialDissipation(cc) 

 #If #DIM == "2"
   #defineMacro advection(cc) (advectCoeff*( uu(i1,i2,i3,uc)*UX(cc) + uu(i1,i2,i3,vc)*UY(cc) ))
 #Else
   #defineMacro advection(cc) (advectCoeff*( uu(i1,i2,i3,uc)*UX(cc) + uu(i1,i2,i3,vc)*UY(cc) + uu(i1,i2,i3,wc)*UZ(cc) ))
 #End
 #If #TYPE == "INSTP" 
   ! advection term is rho*( u.grad u ) for two-phase flow 
   #If #DIM == "2"
     #defineMacro advection(cc) (advectCoeff*( uu(i1,i2,i3,uc)*UX(cc)+uu(i1,i2,i3,vc)*UY(cc) )*u(i1,i2,i3,rc))
   #Else
     #defineMacro advection(cc) (advectCoeff*( uu(i1,i2,i3,uc)*UX(cc) + uu(i1,i2,i3,vc)*UY(cc) + uu(i1,i2,i3,wc)*UZ(cc) )*u(i1,i2,i3,rc) )
   #End
 #End

 #If #TYPE == "INS" 
   #If #DIM == "2"
     #If #OPTION == "boundary"
       ! curl-curl form of the diffusion operator in 2D
       #defineMacro uDiffusion(i1,i2,i3) (nu*(-UXY(vc)+UYY(uc)))
       #defineMacro vDiffusion(i1,i2,i3) (nu*( UXX(vc)-UXY(uc)))
     #Else
       ! normal-form of the diffusion operator in 2D
       #defineMacro uDiffusion(i1,i2,i3) (nu*(ULAP(uc)))
       #defineMacro vDiffusion(i1,i2,i3) (nu*(ULAP(vc)))
     #End
   #Else
       ! curl-curl form of the diffusion operator in 3D
     #If #OPTION == "boundary"
       #defineMacro uDiffusion(i1,i2,i3) (nu*(-UXY(vc)-UXZ(wc)+UYY(uc)+UZZ(uc)))
       #defineMacro vDiffusion(i1,i2,i3) (nu*( UXX(vc)-UXY(uc)-UYZ(wc)+UZZ(vc)))
       #defineMacro wDiffusion(i1,i2,i3) (nu*( UXX(wc)+UYY(wc)-UXZ(uc)-UYZ(vc)))
     #Else
       ! normal-form of the diffusion operator in 3D
       #defineMacro uDiffusion(i1,i2,i3) (nu*(ULAP(uc)))
       #defineMacro vDiffusion(i1,i2,i3) (nu*(ULAP(vc)))
       #defineMacro wDiffusion(i1,i2,i3) (nu*(ULAP(wc)))
     #End
   #End

 #Elif #TYPE == "INSSPAL" || #TYPE == "INSBL" || #TYPE == "INSKE" || #TYPE == "INSVP" || #TYPE == "INSTP" 
   ! get nuT,nuTx,nuTy,nuTz
   getEddyViscosityAndFirstDerivatives(TYPE,DIM)
   #If #DIM == "2"
     #If #OPTION == "boundary"
       ! curl-curl form of the diffusion operator in 2D
       #defineMacro uDiffusion(i1,i2,i3) (nuT*(-UXY(vc)+UYY(uc))-2.*nuTx*(UY(vc))+nuTy*(UY(uc)+UX(vc)))
       #defineMacro vDiffusion(i1,i2,i3) (nuT*( UXX(vc)-UXY(uc))-2.*nuTy*(UX(uc))+nuTx*(UX(vc)+UY(uc)))
     #Else
       ! normal-form of the diffusion operator in 2D
       #defineMacro uDiffusion(i1,i2,i3) (nuT*(ULAP(uc))+2.*nuTx*(UX(uc))+nuTy*(UY(uc)+UX(vc)))
       #defineMacro vDiffusion(i1,i2,i3) (nuT*(ULAP(vc))+2.*nuTy*(UY(vc))+nuTx*(UX(vc)+UY(uc)))
     #End
   #Else
     #If #OPTION == "boundary"
       ! curl-curl form of the diffusion operator in 3D
       #defineMacro uDiffusion(i1,i2,i3) (nuT*(-UXY(vc)-UXZ(wc)+UYY(uc)+UZZ(uc)) \
                         -2.*nuTx*(UY(vc)+UZ(wc))+nuTy*(UY(uc)+UX(vc))+nuTz*(UZ(uc)+UX(wc)))            
       #defineMacro vDiffusion(i1,i2,i3) (nuT*(UXX(vc)-UXY(uc)-UYZ(wc)+UZZ(vc)) \
                         -2.*nuTy*(UZ(wc)+UX(uc))+nuTz*(UZ(vc)+UY(wc))+nuTx*(UX(vc)+UY(uc)))
       #defineMacro wDiffusion(i1,i2,i3) (nuT*(UXX(wc)+UYY(wc)-UXZ(uc)-UYZ(vc)) \
                         -2.*nuTz*(UX(uc)+UY(vc))+nuTx*(UX(wc)+UZ(uc))+nuTy*(UY(wc)+UZ(vc)))
     #Else
       ! normal-form of the diffusion operator in 3D
       #defineMacro uDiffusion(i1,i2,i3) (nuT*(ULAP(uc)) \
                         +2.*nuTx*(UX(uc))+nuTy*(UY(uc)+UX(vc))+nuTz*(UZ(uc)+UX(wc)))
       #defineMacro vDiffusion(i1,i2,i3) (nuT*(ULAP(vc)) \
                         +2.*nuTy*(UY(vc))+nuTz*(UZ(vc)+UY(wc))+nuTx*(UX(vc)+UY(uc)))
       #defineMacro wDiffusion(i1,i2,i3) (nuT*(ULAP(wc))\
                         +2.*nuTz*(UZ(wc))+nuTx*(UX(wc)+UZ(uc))+nuTy*(UY(wc)+UZ(vc)))
     #End
   #End
 #Else
   stop 123
 #End

 #If #ADTYPE == "AD2" || #ADTYPE == "AD24"
   #If #DIM == "2"
     adCoeff2 = ad21+cd22*( abs(UX(uc))+abs(UY(uc))+abs(UX(vc))+abs(UY(vc)) )
     #defineMacro artificialDissipation(cc)  +adCoeff2*ad2(cc)
   #Else
     adCoeff2 = ad21+cd22*( abs(UX(uc))+abs(UY(uc))+abs(UZ(uc))+\
                            abs(UX(vc))+abs(UY(vc))+abs(UZ(vc))+\
                            abs(UX(wc))+abs(UY(wc))+abs(UZ(wc)))
     #defineMacro artificialDissipation(cc)  +adCoeff2*ad23(cc)
   #End

 #End

 ! Artificial Dissipation  **todo** implicit-line, self-adjoint versions
 #If #ADTYPE == "AD4" || #ADTYPE == "AD24"
   #If #DIM == "2"
     adCoeff4 = ad41+cd42*( abs(UX(uc))+abs(UY(uc))+abs(UX(vc))+abs(UY(vc)) )
     #defineMacro artificialDissipation(cc)  +adCoeff4*ad4(cc)
   #Else
     adCoeff4 = ad41+cd42*( abs(UX(uc))+abs(UY(uc))+abs(UZ(uc))+\
                            abs(UX(vc))+abs(UY(vc))+abs(UZ(vc))+\
                            abs(UX(wc))+abs(UY(wc))+abs(UZ(wc)))
     #defineMacro artificialDissipation(cc)  +adCoeff4*ad43(cc)
   #End
 #End

 #If #ADTYPE == "AD24"
  #If #DIM == "2"
    #defineMacro artificialDissipation(cc)  +adCoeff2*ad2(cc) + adCoeff4*ad4(cc)
  #Else
    #defineMacro artificialDissipation(cc)  +adCoeff2*ad23(cc) + adCoeff4*ad43(cc)
  #End
 #End


 ! Here now is the statement where the ghost line value in the RHS is assigned.
 #If #GRIDTYPE == "rectangular"
   #If #AXIS == "R"
     f(i1+is1,i2,i3)=(2*side-1)*( uDiffusion(i1,i2,i3)-advection(uc) artificialDissipation(uc) )
   #Elif #AXIS == "S"
     f(i1,i2+is2,i3)=(2*side-1)*( vDiffusion(i1,i2,i3)-advection(vc) artificialDissipation(vc) )
   #Else
     f(i1,i2,i3+is3)=(2*side-1)*( wDiffusion(i1,i2,i3)-advection(wc) artificialDissipation(wc) )
   #End
 #Else
   #If #DIM == "2"
     f(i1+is1,i2+is2,i3)=normal(i1,i2,i3,0)*( uDiffusion(i1,i2,i3)-advection(uc) artificialDissipation(uc) )\
                        +normal(i1,i2,i3,1)*( vDiffusion(i1,i2,i3)-advection(vc) artificialDissipation(vc) )
   #Else
     f(i1+is1,i2+is2,i3+is3)=normal(i1,i2,i3,0)*( uDiffusion(i1,i2,i3)-advection(uc) artificialDissipation(uc) )\
                            +normal(i1,i2,i3,1)*( vDiffusion(i1,i2,i3)-advection(vc) artificialDissipation(vc) )\
                            +normal(i1,i2,i3,2)*( wDiffusion(i1,i2,i3)-advection(wc) artificialDissipation(wc) )
   #End
 #End
  
#endMacro

! *******************************************************************************
!
! TYPE: INS, INSSPAL, INSBL, INSKE
! ADTYPE: noAD, AD2, AD4, AD24
! GRIDTYPE: rectangular, curvilinear
! ORDER: 2,4
! DIM: 2,3
! AXIS: R,S,T
! 
! *******************************************************************************
#beginMacro applyWallBC(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM,AXIS,normal)

  ! Use the curl-curl form of the equations
  beginLoops()
    setPressureBoundaryCondition(boundary,TYPE,ADTYPE,GRIDTYPE,DIM,AXIS,normal)
  endLoops()

  ! For the Boussinesq approximation:  -alpha*T* normal.gravity
  if( pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel )then
    beginLoops()
     #If #GRIDTYPE == "rectangular"
       f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
     #Else
       #If #DIM == "2"
        f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-thermalExpansivity*u(i1,i2,i3,tc)*(\
              gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1))
       #Else
        f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(\
              gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1)+gravity(2)*normal(i1,i2,i3,2))
       #End
     #End
    endLoops()
  else if( pdeModel.eq.twoPhaseFlowModel )then
    beginLoops()
     #If #GRIDTYPE == "rectangular"
       f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
     #Else
       #If #DIM == "2"
        f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+u(i1,i2,i3,rc)*(\
              gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1))
       #Else
        f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+u(i1,i2,i3,rc)*(\
              gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1)+gravity(2)*normal(i1,i2,i3,2))
       #End
     #End
    endLoops()
  end if
  ! -- include contribution to BC from the body force --- *wdh* 2012/07/06
  if( turnOnBodyForcing.eq.1 )then
    ! write(*,'(" *** inspf: add body force to pressure BC ***")') 
    beginLoops()
      #If #GRIDTYPE == "rectangular" 
        f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
      #Else
       #If #DIM == "2"
        f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+normal(i1,i2,i3,0)*udf(i1,i2,i3,uc)\
                                               +normal(i1,i2,i3,1)*udf(i1,i2,i3,vc)
       #Else
        f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+normal(i1,i2,i3,0)*udf(i1,i2,i3,uc)\
                                                       +normal(i1,i2,i3,1)*udf(i1,i2,i3,vc)\
                                                       +normal(i1,i2,i3,2)*udf(i1,i2,i3,wc)
       #End
      #End
    endLoops()
  end if

  #If #TYPE == "INS" 
    #If #DIM == "2"
      if( isAxisymmetric.eq.1 )then
        ! add on the curl-curl form of the axisymmetric correction -- 
        ! See the CginsRef documentation for a description of where this comes from.
        beginLoops()
          if( radiusInverse(i1,i2,i3).ne.0. )then
            #If #GRIDTYPE == "rectangular"
              f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*an(0)*(UY(uc)-UX(vc))*radiusInverse(i1,i2,i3)
            #Else
              f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*normal(i1,i2,i3,0)*(UY(uc)-UX(vc))*radiusInverse(i1,i2,i3)
            #End
          else
            #If #GRIDTYPE == "rectangular"
             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*an(0)*(UYY(uc)-UXY(vc))
            #Else
             f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*normal(i1,i2,i3,0)*(UYY(uc)-UXY(vc))
            #End
         end if
        endLoops()
      end if
    #End
  #End

  ! end points that meet a wall are treated differently to make sure p.x=0 where it should be
  ! --> do not use the curl-curl boundary condition
  do kd=1,nd-1 ! loop over the two-tangential directions
   axisp=mod(axis+kd,nd)
   do sidep1=0,1
    ! For now: do not apply the corner correction for TZ flow (we need to fix TZ forcing for this case)
    if( bc(sidep1,axisp).eq.inflowWithVelocityGiven .or. bc(sidep1,axisp).eq.noSlipWall \
        .and. twilightZoneFlow.eq.0 )then  
      if( axisp.eq.0 )then
        n1a=indexRange(sidep1,0)
        n1b=indexRange(sidep1,0)
      else if( axisp.eq.1 )then
        n2a=indexRange(sidep1,1)
        n2b=indexRange(sidep1,1)
      else 
        n3a=indexRange(sidep1,2)
        n3b=indexRange(sidep1,2)
      end if
      beginLoops()
        setPressureBoundaryCondition(edge,TYPE,ADTYPE,GRIDTYPE,DIM,AXIS,normal)
      endLoops()

      ! For the Boussinesq approximation:  -alpha*T* normal.gravity
      if( pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel )then
        beginLoops()
         #If #GRIDTYPE == "rectangular"
           f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)-(2*side-1)*thermalExpansivity*u(i1,i2,i3,tc)*gravity(axis)
         #Else
           #If #DIM == "2"
            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)-thermalExpansivity*u(i1,i2,i3,tc)*(\
                  gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1))
           #Else
            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)-thermalExpansivity*u(i1,i2,i3,tc)*(\
                  gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1)+gravity(2)*normal(i1,i2,i3,2))
           #End
         #End
        endLoops()
      else if( pdeModel.eq.twoPhaseFlowModel )then
        beginLoops()
         #If #GRIDTYPE == "rectangular"
           f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+(2*side-1)*u(i1,i2,i3,rc)*gravity(axis)
         #Else
           #If #DIM == "2"
            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+u(i1,i2,i3,rc)*(\
                  gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1))
           #Else
            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+u(i1,i2,i3,rc)*(\
                  gravity(0)*normal(i1,i2,i3,0)+gravity(1)*normal(i1,i2,i3,1)+gravity(2)*normal(i1,i2,i3,2))
           #End
         #End
        endLoops()
      end if
      ! -- include contribution to BC from the body force ---
      if( turnOnBodyForcing.eq.1 )then
        beginLoops()
          #If #GRIDTYPE == "rectangular" 
            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+(2*side-1)*udf(i1,i2,i3,uc+axis)
          #Else
           #If #DIM == "2"
            f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+normal(i1,i2,i3,0)*udf(i1,i2,i3,uc)\
                                                   +normal(i1,i2,i3,1)*udf(i1,i2,i3,vc)
           #Else
            f(i1+is1,i2+is2,i3+is3)=f(i1+is1,i2+is2,i3+is3)+normal(i1,i2,i3,0)*udf(i1,i2,i3,uc)\
                                                           +normal(i1,i2,i3,1)*udf(i1,i2,i3,vc)\
                                                           +normal(i1,i2,i3,2)*udf(i1,i2,i3,wc)
           #End
          #End
        endLoops()
      end if
      #If #TYPE == "INS" 
        #If #DIM == "2"
          if( isAxisymmetric.eq.1 )then
            beginLoops()
              if( radiusInverse(i1,i2,i3).ne.0. )then
                #If #GRIDTYPE == "rectangular"
                  f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*(an(0)*(UY(uc)) +\
                       an(1)*( UY(vc)-u(i1,i2,i3,vc)*radiusInverse(i1,i2,i3) ))*radiusInverse(i1,i2,i3)
                #Else
                  f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*(normal(i1,i2,i3,0)*(UY(uc)) +\
                     normal(i1,i2,i3,1)*( UY(vc)-u(i1,i2,i3,vc)*radiusInverse(i1,i2,i3)) )*radiusInverse(i1,i2,i3)
                #End
              else
                #If #GRIDTYPE == "rectangular"
                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*(an(0)*(UYY(uc))\
                                                            +an(1)*(.5*UYY(vc)))
                #Else
                 f(i1+is1,i2+is2,i3)=f(i1+is1,i2+is2,i3)+nu*(normal(i1,i2,i3,0)*(UYY(uc))\
                                                            +normal(i1,i2,i3,1)*(.5*UYY(vc)) )
                #End
             end if
            endLoops()
          end if
        #End
      #End

      ! reset loop bounds
      if( axisp.eq.0 )then
        n1a=indexRange(0,0)
        n1b=indexRange(1,0)
      else if( axisp.eq.1 )then
        n2a=indexRange(0,1)
        n2b=indexRange(1,1)
      else 
        n3a=indexRange(0,2)
        n3b=indexRange(1,2)
      end if
     end if
   end do
  end do

#endMacro



! *******************************************************************************
!  
! *******************************************************************************
#beginMacro getDerivativesForArtificalDissipation(DIM,ORDER)
  u0x=UX(uc)
  u0y=UY(uc)
  u0xx=UXX(uc)
  u0xy=UXY(uc)
  u0yy=UYY(uc)

  v0x=UX(vc)
  v0y=UY(vc)
  v0xx=UXX(vc)
  v0xy=UXY(vc)
  v0yy=UYY(vc)

  #If #DIM == "2"
    #If #ORDER == "2" || #ORDER=="24"
      delta2u=ad2(uc)
      delta2v=ad2(vc)
    #End
    #If #ORDER=="4" || #ORDER=="24"
      delta4u=ad4(uc)
      delta4v=ad4(vc)
    #End

  #Else
    #If #ORDER == "2" || #ORDER=="24"
      delta2u=ad23(uc)
      delta2v=ad23(vc)
      delta2w=ad23(wc)
    #End
    #If #ORDER=="4" || #ORDER=="24"
      delta4u=ad43(uc)
      delta4v=ad43(vc)
      delta4w=ad43(wc)
    #End

    u0z=UZ(uc)
    u0xz=UXZ(uc)
    u0yz=UYZ(uc)
    u0zz=UZZ(uc)

    v0z=UZ(vc)
    v0xz=UXZ(vc)
    v0yz=UYZ(vc)
    v0zz=UZZ(vc)

    w0x=UX(wc)
    w0y=UY(wc)
    w0xx=UXX(wc)
    w0xy=UXY(wc)
    w0yy=UYY(wc)
    w0z=UZ(wc)
    w0xz=UXZ(wc)
    w0yz=UYZ(wc)
    w0zz=UZZ(wc)

  #End

  #If #DIM == "2"
    adCoeffu = sign(1.,u0x)*u0xx + sign(1.,u0y)*u0xy +sign(1.,v0x)*v0xx + sign(1.,v0y)*v0xy
    adCoeffv = sign(1.,u0x)*u0xy + sign(1.,u0y)*u0yy +sign(1.,v0x)*v0xy + sign(1.,v0y)*v0yy
  #Else
    adCoeffu = sign(1.,u0x)*u0xx + sign(1.,u0y)*u0xy +sign(1.,v0x)*v0xx + sign(1.,v0y)*v0xy \
              +sign(1.,u0z)*u0xz + sign(1.,v0z)*v0xz +sign(1.,w0x)*w0xx + sign(1.,w0y)*w0xy+ sign(1.,w0z)*w0xz
    adCoeffv = sign(1.,u0x)*u0xy + sign(1.,u0y)*u0yy +sign(1.,v0x)*v0xy + sign(1.,v0y)*v0yy \
              +sign(1.,u0z)*u0yz + sign(1.,v0z)*v0yz +sign(1.,w0x)*w0xy + sign(1.,w0y)*w0yy+ sign(1.,w0z)*w0yz
    adCoeffw = sign(1.,u0x)*u0xz + sign(1.,u0y)*u0yz +sign(1.,v0x)*v0xz + sign(1.,v0y)*v0yz \
              +sign(1.,u0z)*u0zz + sign(1.,v0z)*v0zz +sign(1.,w0x)*w0xz + sign(1.,w0y)*w0yz+ sign(1.,w0z)*w0zz
  #End

#endMacro

! ============================================================================
! Define the turbulent eddy viscosity and derivatives for SPAL
! ============================================================================
#beginMacro defineSPAL_Derivatives(DIM)
  u0x=UX(uc)
  u0y=UY(uc)
  v0x=UX(vc)
  v0y=UY(vc)
  u0Lap=ULAP(uc)
  v0Lap=ULAP(vc)
  #If #DIM == "3" 
   u0z=UZ(uc)
   v0z=UZ(vc)
   w0x=UX(wc)
   w0y=UY(wc)
   w0z=UZ(wc)
   w0Lap=ULAP(wc)
  #End

  chi=u(i1,i2,i3,nc)/nu
  chi3= chi**3
  nuT = nu+u(i1,i2,i3,nc)*chi3/(chi3+cv1e3)
  nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
  nuTdd= 6*chi**2*cv1e3*(-chi3+2*cv1e3)/(chi3+cv1e3)**3/nu  ! this is really nuTdd/nu : from spal.maple

  n0x=UX(nc)
  n0y=UY(nc)
  n0xx=UXX(nc)
  n0xy=UXY(nc)
  n0yy=UYY(nc)

  nuTx=n0x*nuTd
  nuTy=n0y*nuTd
  nuTxx=n0xx*nuTd+n0x*n0x*nuTdd
  nuTxy=n0xy*nuTd+n0x*n0y*nuTdd
  nuTyy=n0yy*nuTd+n0y*n0y*nuTdd

!  write(*,'(" inspf:SPAL: chi,nuTx,nuTy,nuTxx,nuTxy,nuTyy=",10e10.2)') chi,nuTx,nuTy,nuTxx,nuTxy,nuTyy
  #If #DIM == "3" 
    n0z=UZ(nc)
    n0xz=UXZ(nc)
    n0yz=UYZ(nc)
    n0zz=UZZ(nc)

    nuTz=n0z*nuTd
    nuTxz=n0xz*nuTd+n0x*n0z*nuTdd
    nuTyz=n0yz*nuTd+n0y*n0z*nuTdd
    nuTzz=n0zz*nuTd+n0z*n0z*nuTdd

  #End

#endMacro

! ============================================================================
! Define the turbulent eddy viscosity and derivatives for BL
! ============================================================================
#beginMacro defineBL_Derivatives(DIM)
  u0x=UX(uc)
  u0y=UY(uc)
  v0x=UX(vc)
  v0y=UY(vc)
  u0Lap=ULAP(uc)
  v0Lap=ULAP(vc)
  #If #DIM == "3" 
   u0z=UZ(uc)
   v0z=UZ(vc)
   w0x=UX(wc)
   w0y=UY(wc)
   w0z=UZ(wc)
   w0Lap=ULAP(wc)
  #End

  nuT = nu+u(i1,i2,i3,nc)

  nuTx=UX(nc)
  nuTy=UY(nc)
  nuTxx=UXX(nc)
  nuTxy=UXY(nc)
  nuTyy=UYY(nc)

  #If #DIM == "3" 
    nuTz =UZ(nc)
    nuTxz=UXZ(nc)
    nuTyz=UYZ(nc)
    nuTzz=UZZ(nc)
  #End

#endMacro

! ============================================================================
! Define the dervatives needed for the visco plastic model
! ============================================================================
#beginMacro defineVP_Derivatives(DIM)
  u0x=UX(uc)
  u0y=UY(uc)
  v0x=UX(vc)
  v0y=UY(vc)
  u0Lap=ULAP(uc)
  v0Lap=ULAP(vc)
  #If #DIM == "3" 
   u0z=UZ(uc)
   v0z=UZ(vc)
   w0x=UX(wc)
   w0y=UY(wc)
   w0z=UZ(wc)
   w0Lap=ULAP(wc)
  #End

  nuT = u(i1,i2,i3,vsc)

  nuTx=UX(vsc)
  nuTy=UY(vsc)
  nuTxx=UXX(vsc)
  nuTxy=UXY(vsc)
  nuTyy=UYY(vsc)

  #If #DIM == "3" 
    nuTz =UZ(vsc)
    nuTxz=UXZ(vsc)
    nuTyz=UYZ(vsc)
    nuTzz=UZZ(vsc)
  #End

#endMacro


! ============================================================================
! Define the turbulent eddy viscosity and derivatives for KE
! ============================================================================
#beginMacro defineKE_Derivatives(DIM)
 u0x=UX(uc)
 u0y=UY(uc)
 v0x=UX(vc)
 v0y=UY(vc)
 u0Lap=ULAP(uc)
 v0Lap=ULAP(vc)
 #If #DIM == "3" 
  u0z=UZ(uc)
  v0z=UZ(vc)
  w0x=UX(wc)
  w0y=UY(wc)
  w0z=UZ(wc)
  w0Lap=ULAP(wc)
 #End

 k0=u(i1,i2,i3,kc)
 e0=u(i1,i2,i3,ec)
 nuT = nu + cMu*k0**2/e0
 k0x=UX(kc)
 e0x=UX(ec)
 k0y=UY(kc)
 e0y=UY(ec)
 nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
 nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2

 k0xx=UXX(kc)
 k0xy=UXY(kc)
 k0yy=UYY(kc)
 e0xx=UXX(ec)
 e0xy=UXY(ec)
 e0yy=UYY(ec)
 nuTxx=cMu*(2.*k0xx*e0**2-4.*k0*k0x*e0x*e0+2*k0*k0xx*e0**2+2*k0**2.*e0x**2-k0**2.*e0xx*e0)/e0**3
 nuTxy=cMu*(2*k0y*k0x*e0**2-2*k0*k0x*e0y*e0+2*k0*k0xy*e0**2-2*k0*e0x*k0y*e0+2*k0**2*e0x*e0y-k0**2*e0xy*e0)/e0**3
 nuTyy=cMu*(2.*k0yy*e0**2-4.*k0*k0y*e0y*e0+2*k0*k0yy*e0**2+2*k0**2.*e0y**2-k0**2.*e0yy*e0)/e0**3
 #If #DIM == "3" 
  k0z=UZ(kc)
  e0z=UZ(ec)
  nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
  k0xy=UXY(kc)
  k0yz=UYZ(kc)
  k0zz=UZZ(kc)
  e0xy=UXY(ec)
  e0xz=UXZ(ec)
  e0yz=UYZ(ec)
  e0zz=UZZ(ec)
  nuTxz=cMu*(2*k0z*k0x*e0**2-2*k0*k0x*e0z*e0+2*k0*k0xz*e0**2-2*k0*e0x*k0z*e0+2*k0**2*e0x*e0z-k0**2*e0xz*e0)/e0**3
  nuTyz=cMu*(2*k0z*k0y*e0**2-2*k0*k0y*e0z*e0+2*k0*k0yz*e0**2-2*k0*e0y*k0z*e0+2*k0**2*e0y*e0z-k0**2*e0yz*e0)/e0**3
  nuTzz=cMu*(2.*k0zz*e0**2-4.*k0*k0z*e0z*e0+2*k0*k0zz*e0**2+2*k0**2.*e0z**2-k0**2.*e0zz*e0)/e0**3
 #End


#endMacro



#beginMacro defineTM_Derivatives(TYPE,DIM)
#If #TYPE == "INSSPAL"
 defineSPAL_Derivatives(DIM)
#Elif #TYPE == "INSBL"
 defineBL_Derivatives(DIM)
#Elif #TYPE == "INSKE"
 defineKE_Derivatives(DIM)
#Elif #TYPE == "INSVP"  || #TYPE == "INSTP"
 defineVP_Derivatives(DIM)
#Else
  stop 666
#End
#endMacro

#beginMacro axisSymmetricCorrection()
  ! Here is the correction to the axisymmetric divergence:
  !   div(U) = U_x + V_r + V/r 
  if( radiusInverse(i1,i2,i3).ne.0. )then
    f(i1,i2,i3)=f(i1,i2,i3) + divDamping(i1,i2,i3)*( u(i1,i2,i3,vc)*radiusInverse(i1,i2,i3) )
  else
    f(i1,i2,i3)=f(i1,i2,i3) + divDamping(i1,i2,i3)*( UY(vc) )
  end if
#endMacro

! ************************************************************************************
!   This macro is used to fill in the RHS f(i1,i2,i3) for the pressure equation
!   for equations in the interior
!
!  TYPE: INS, INSSPAL, INSBL, INSKE
!
! ************************************************************************************
#beginMacro assignPressureRHS(TYPE,DIM,ORDER,GRIDTYPE)

defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

#If #TYPE == "INS" 
 #If #DIM == "2"
   loopse4(u0x=UX(uc),\
           v0y=UY(vc),\
           f(i1,i2,i3)=(-advectionCoefficient)*( u0x**2+2.*UY(uc)*UX(vc)+v0y**2 )\
          + divDamping(i1,i2,i3)*(u0x+v0y), )
   if( isAxisymmetric.eq.1 )then
     loopse1($axisSymmetricCorrection())
   end if
 #Else
 
  ! ************* 3D ************** 
   loopse4(u0x=UX(uc),\
           v0y=UY(vc),\
           w0z=UZ(wc),\
           f(i1,i2,i3)=(-advectionCoefficient)*( u0x**2+v0y**2+w0z**2+\
               2.*(UY(uc)*UX(vc)+ UZ(uc)*UX(wc) + UZ(vc)*UY(wc)) )\
          + divDamping(i1,i2,i3)*(u0x+v0y+w0z) )
 
 #End

#Elif #TYPE == "INSSPAL" || #TYPE == "INSBL" || #TYPE == "INSKE" || #TYPE == "INSVP" 

 #If #DIM == "2"
  loopse4($defineTM_Derivatives(TYPE,2),\
          f(i1,i2,i3)=(-advectionCoefficient)*( u0x**2+2.*u0y*v0x+v0y**2 )\
         + divDamping(i1,i2,i3)*(u0x+v0y) \
         + 2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+ \
                nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y),, )
 #Else
  loopse4($defineTM_Derivatives(TYPE,3),\
          f(i1,i2,i3)=(-advectionCoefficient)*( u0x**2+v0y**2+w0z**2+\
              2.*(u0y*v0x+ u0z*w0x + v0z*w0y) )\
         + divDamping(i1,i2,i3)*(u0x+v0y+w0z) \
         + 2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+ \
                nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+ \
                nuTz*w0Lap+nuTxz*w0x+nuTyz*w0y+nuTzz*w0z),, )
 #End

#Elif #TYPE == "INSTP" 
 ! Two-phase flow : divide viscous terms by rho 
 #If #DIM == "2"
  loopse4($defineTM_Derivatives(TYPE,2),\
          f(i1,i2,i3)=(-advectionCoefficient)*( u0x**2+2.*u0y*v0x+v0y**2 )\
         + divDamping(i1,i2,i3)*(u0x+v0y) \
         + 2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+ \
                nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y)/u(i1,i2,i3,rc),, )
 #Else
  loopse4($defineTM_Derivatives(TYPE,3),\
          f(i1,i2,i3)=(-advectionCoefficient)*( u0x**2+v0y**2+w0z**2+\
              2.*(u0y*v0x+ u0z*w0x + v0z*w0y) )\
         + divDamping(i1,i2,i3)*(u0x+v0y+w0z) \
         + 2.*( nuTx*u0Lap+nuTxx*u0x+nuTxy*u0y+nuTxz*u0z+ \
                nuTy*v0Lap+nuTxy*v0x+nuTyy*v0y+nuTyz*v0z+ \
                nuTz*w0Lap+nuTxz*w0x+nuTyz*w0y+nuTzz*w0z)/u(i1,i2,i3,rc),, )
 #End

#Else
 ! Error unknown TYPE
 stop 777
#End

 ! For the Boussinesq approximation: -alpha*( (gravity.grad) T )
 if( pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel )then
  #If #DIM == "2"
   loopse1(f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(0)*UX(tc)+gravity(1)*UY(tc)))
  #Else
   loopse1(f(i1,i2,i3)=f(i1,i2,i3)-thermalExpansivity*(gravity(0)*UX(tc)+gravity(1)*UY(tc)+gravity(2)*UZ(tc)))
  #End

 end if

 ! -- Add on the divergence of the user defined force ---
 if( turnOnBodyForcing.eq.1 )then
!!120224 kkc gets annoying after awhile  write(*,'(" *** inspf: add divergence of the body force to the pressure RHS ***")') 
  #If #DIM == "2"
   loopse1(f(i1,i2,i3)=f(i1,i2,i3) + UDFX(uc) + UDFY(vc))
  #Else
   loopse1(f(i1,i2,i3)=f(i1,i2,i3) + UDFX(uc) + UDFY(vc)) + UDFZ(wc))
  #End

 end if

#endMacro





! *************************************************************************
! ****** Header info for pressure RHS and pressure BC subroutines  ********
! *************************************************************************
#beginMacro INITIALIZE()
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b, \
    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndb
 integer nr1a,nr1b,nr2a,nr2b,nr3a,nr3b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real udf(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:*)        ! user defined force
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real bcData(0:ndb-1,0:1,0:2)

 real normal00(nr1a:nr1a,nd2a:nd2b,nd3a:nd3b,0:*)
 real normal10(nr1b:nr1b,nd2a:nd2b,nd3a:nd3b,0:*)
 real normal01(nd1a:nd1b,nr2a:nr2a,nd3a:nd3b,0:*)
 real normal11(nd1a:nd1b,nr2b:nr2b,nd3a:nd3b,0:*)
 real normal02(nd1a:nd1b,nd2a:nd2b,nr3a:nr3a,0:*)
 real normal12(nd1a:nd1b,nd2a:nd2b,nr3b:nr3b,0:*)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),indexRange(0:1,0:2), ierr

 integer ipar(0:*)
 real rpar(0:*)
      
 integer \
      noSlipWall, \
      inflowWithVelocityGiven, \
      outflow, \
      convectiveOutflow, \
      tractionFree, \
      inflowWithPandTV, \
      dirichletBoundaryCondition, \
      symmetry, \
      axisymmetric, \
      interfaceBoundaryCondition,\
      freeSurfaceBoundaryCondition 
 parameter( noSlipWall=1,inflowWithVelocityGiven=2, \
  outflow=5,convectiveOutflow=14,tractionFree=15, \
  inflowWithPandTV=3, \
   dirichletBoundaryCondition=12, \
   symmetry=11,axisymmetric=13,interfaceBoundaryCondition=17, \
   freeSurfaceBoundaryCondition=31 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )
!  enum BoundaryCondition
!  {
!0    interpolation=0,
!1    noSlipWall,
!2    inflowWithVelocityGiven,
!3    inflowWithPressureAndTangentialVelocityGiven,
!4    slipWall,
!5    outflow,
!6    superSonicInflow,
!7    superSonicOutflow,
!8    subSonicInflow,
!9    subSonicInflow2,
!0    subSonicOutflow,
!1    symmetry,
!2    dirichletBoundaryCondition,
!3    axisymmetric,
!4    convectiveOutflow,  
!5    tractionFree,
!6    numberOfBCNames     // counts number of entries
!  };

!     ---- local variables -----
 integer c,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask,twilightZoneFlow,turnOnBodyForcing,debug
 integer isAxisymmetric,is,is1,is2,is3,pressureBC,gridType,initialConditionsAreBeingProjected
 integer rc,pc,uc,vc,wc,grid,side,axis,bc0,numberOfComponents,axisp1,axisp2,sidep1,sidep2,axisp
 integer nc,tc,vsc
 real nu,dt,advectionCoefficient,advectCoeff,inflowPressure,a1,an(0:2)
 real an1,an2,an3,aNormi,epsx, nTauN,mu,fluidDensity,t
 real an1r,an2r,an3r,an1s,an2s,an3s,an1t,an2t,an3t
 real ayrys
 real gravity(0:2),thermalExpansivity,adcBoussinesq

 real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
 real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

 real u0x,u0y,u0z,v0x,v0y,v0z,w0x,w0y,w0z,u0Lap,v0Lap,w0Lap
 real u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
 real v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
 real w0xx,w0xy,w0xz,w0yy,w0yz,w0zz
 real delta2u,delta2v,delta2w,delta4u,delta4v,delta4w

 real n0x,n0y,n0z,n0xx,n0xy,n0xz,n0yy,n0yz,n0zz
 real chi,chi3,nuT,nuTd,nuTdd
 real nuTx,nuTy,nuTz,nuTxx,nuTxy,nuTxz,nuTyy,nuTyz,nuTzz

 real rsxy1, dr1,dx1

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

 integer m,n,kd,kdd,kd3,ndc,dir,ks

 ! for SPAL TM
 real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0

 ! for KE turbulence model
 real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI
 integer kc,ec
 real k0,k0x,k0y,k0z,k0xx,k0xy,k0xz,k0yy,k0yz,k0zz
 real e0,e0x,e0y,e0z,e0xx,e0xy,e0xz,e0yy,e0yz,e0zz

 ! for visco-plastic model
 ! real nuVP,etaVP,yieldStressVP,exponentVP,epsVP
 ! real eDotNorm,exp0,eDotNormSqx,eDotNormSqy,eDotNormSqxx,eDotNormSqxy,eDotNormSqyy
 ! real u0xxx,u0xxy,u0xyy,u0yyy,v0xxx,v0xxy,v0xyy,v0yyy

 integer use2ndOrderAD,use4thOrderAD,useImplicit4thOrderAD,includeADinPressure
 real ad21,ad22,ad41,ad42,cd22,cd42
 real adCoeffu,adCoeffv,adCoeffw,adCoeff2,adCoeff4

 ! -- variables for the free surface: 
 real det,deti,detMin,detr,dets,dett
 real xr,yr,zr,xs,ys,zs, xt,yt,zt
 real xrr,yrr,zrr,xss,yss,zss,xtt,ytt,ztt
 real xrs,yrs,zrs,xrt,yrt,zrt,xst,yst,zst
 real rxi,ryi,rzi, sxi,syi,szi, txi,tyi,tzi
 real rxr,ryr,rzr, sxr,syr,szr, txr,tyr,tzr
 real rxs,rys,rzs, sxs,sys,szs, txs,tys,tzs
 real rxt,ryt,rzt, sxt,syt,szt, txt,tyt,tzt

 real axrr,ayrr,azrr, axsr,aysr,azsr, axtr,aytr,aztr
 real axrs,ayrs,azrs, axss,ayss,azss, axts,ayts,azts
 real axrt,ayrt,azrt, axst,ayst,azst, axtt,aytt,aztt

 real pAtmosphere,surfaceTension,meanCurvature
 real aEi,aFi,aGi,aLi,aMi,aNi

 ! -- variables for boundary forcing (bcData)
 integer dim(0:1,0:2,0:1,0:2), addBoundaryForcing(0:1,0:2)
 real bcf0(0:*)
 integer*8 bcOffset(0:1,0:2)
 real bcf

!     .....begin statement functions
 real rx,ry,rz,sx,sy,sz,tx,ty,tz
 real dr(0:2), dx(0:2)


 declareDifferenceNewOrder2(u,rsxy,dr,dx,RX)
 declareDifferenceNewOrder4(u,rsxy,dr,dx,RX)

 declareDifferenceNewOrder2(udf,rsxy1,dr1,dx1,RX)
 declareDifferenceNewOrder4(udf,rsxy1,dr1,dx1,RX)

 real ad2,ad23,ad4,ad43

!     --- begin statement functions

! Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
! an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
 bcf(side,axis,i1,i2,i3,m) = bcf0(bcOffset(side,axis) + \
     (i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)* \
     (i2-dim(0,1,side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)* \
     (i3-dim(0,2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(m)))))

!    --- 2nd order 2D artificial diffusion ---
 ad2(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
            +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
!    --- 2nd order 3D artificial diffusion ---
 ad23(c)= \
      (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   \
      +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  \
      +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c)) 
!     ---fourth-order artificial diffusion in 2D
 ad4(c)= \
      (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
          -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
      +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
          +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  \
       -12.*u(i1,i2,i3,c) ) 
!     ---fourth-order artificial diffusion in 3D
 ad43(c)= \
      (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  \
          -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)  \
          -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  \
      +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  \
          +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)  \
          +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
       -18.*u(i1,i2,i3,c) )

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

 ! define these for the derivatives of udf:
 rsxy1(i1,i2,i3,kd,ks) = rsxy(i1,i2,i3,kd,ks)
 dr1(kd) = dr(kd)
 dx1(kd) = dx(kd)


!     The next macro call will define the difference approximation statement functions
 defineDifferenceNewOrder2Components1(u,rsxy,dr,dx,RX)
 defineDifferenceNewOrder4Components1(u,rsxy,dr,dx,RX)

 defineDifferenceNewOrder2Components1(udf,rsxy1,dr1,dx1,RX)
 defineDifferenceNewOrder4Components1(udf,rsxy1,dr1,dx1,RX)

!     --- end statement functions

 ierr=0
 ! write(*,*) 'Inside assignPressureRHSOpt'

 pc                   =ipar(0)
 uc                   =ipar(1)
 vc                   =ipar(2)
 wc                   =ipar(3)
 tc                   =ipar(4) ! **new**
 nc                   =ipar(5)
 grid                 =ipar(6)
 orderOfAccuracy      =ipar(7)
 gridIsMoving         =ipar(8)
 useWhereMask         =ipar(9)
 isAxisymmetric       =ipar(10)
 pressureBC           =ipar(11)
 numberOfComponents   =ipar(12)
 gridType             =ipar(13)
 turbulenceModel      =ipar(14)

 use2ndOrderAD        =ipar(15)
 use4thOrderAD        =ipar(16)
 useImplicit4thOrderAD=ipar(17)
 includeADinPressure  =ipar(18)
 pdeModel             =ipar(19) ! **new**
 vsc                  =ipar(20)
 twilightZoneFlow     =ipar(21)
 rc                   =ipar(22)
 initialConditionsAreBeingProjected=ipar(23)
 turnOnBodyForcing    =ipar(24)
 debug                =ipar(25)

 dr(0)               =rpar(0)
 dr(1)               =rpar(1)
 dr(2)               =rpar(2)
 dx(0)               =rpar(3)
 dx(1)               =rpar(4)
 dx(2)               =rpar(5)
 nu                  =rpar(6)
 advectionCoefficient=rpar(7)
 inflowPressure      =rpar(8)

 ad21                =rpar(9)
 ad22                =rpar(10)
 ad41                =rpar(11)
 ad42                =rpar(12)

 gravity(0)          =rpar(13) ! **new**
 gravity(1)          =rpar(14)
 gravity(2)          =rpar(15)
 thermalExpansivity  =rpar(16)
 adcBoussinesq       =rpar(17)
 surfaceTension      =rpar(18)
 pAtmosphere         =rpar(19)
 fluidDensity        =rpar(20)
 t                   =rpar(21)
 dt                  =rpar(22) 
!  nuVP                =rpar(18) ! visco-plastic parameters
!  etaVP               =rpar(19)
!  yieldStressVP       =rpar(20)
!  exponentVP          =rpar(21)
!  epsVP               =rpar(22)


 detMin=1.e-30     ! **FIX ME**
 epsx  =1.e-30  ! for normal computation to prevent division by zero 

 mu = nu*fluidDensity 

 if( debug.gt.1 .and. surfaceTension.ne.0. .and. t .le. 3.*dt )then
   write(*,'("inspf: t=%8.2e, dt=%8.2e, surfaceTension=",e10.2," pAtmosphere=",e10.2)') t,dt,surfaceTension,pAtmosphere
 end if

 cd22=ad22/(nd**2)
 cd42=ad42/(nd**2)

 kc=nc
 ec=kc+1

 ! for non-moving grids, u=uu, and we need to multiply uu by advectionCoefficient in the pressure BC
 advectCoeff=advectionCoefficient  
 if( gridIsMoving.ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then
   ! For moving grids we need to multiply only u by advectionCoefficient, and mutiply by advectCoeff=1 in the pressure BC
   advectCoeff=1. 
 end if

 ! for visco-plastic

 if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
   write(*,'("assignPressureRHSOpt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
   stop 1
 end if
 if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
   write(*,'("assignPressureRHSOpt:ERROR gridType=",i6)') gridType
   stop 2
 end if
 if( numberOfComponents.le.nd )then
   write(*,'("assignPressureRHSOpt:ERROR nd,numberOfComponents=",2i6)') nd,numberOfComponents
   stop 3
 end if
 if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
   write(*,'("assignPressureRHSOpt:ERROR uc,vc,ws=",2i6)') uc,vc,wc
   stop 4
 end if

#endMacro


#beginMacro applyBcOnSides(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM)

  defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

 if( side.eq.0. .and. axis.eq.0 )then
   applyWallBC(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM,R,normal00)
 else if( side.eq.1 .and. axis.eq.0 )then
   applyWallBC(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM,R,normal10)
 else if( side.eq.0 .and. axis.eq.1 )then
   applyWallBC(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM,S,normal01)
 else if( side.eq.1 .and. axis.eq.1 )then
   applyWallBC(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM,S,normal11)
#If #DIM == "3"
 else if( side.eq.0 .and. axis.eq.2 )then
   applyWallBC(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM,T,normal02)
 else if( side.eq.1 .and. axis.eq.2 )then
   applyWallBC(TYPE,ADTYPE,GRIDTYPE,ORDER,DIM,T,normal12)
#End
 else
   stop 33
 end if
#endMacro

#beginMacro applyBcByGridType(TYPE,ADTYPE,ORDER,DIM)
 if( gridType.eq.rectangular )then
   applyBcOnSides(TYPE,ADTYPE,rectangular,ORDER,DIM)
 else if( gridType.eq.curvilinear )then 
   applyBcOnSides(TYPE,ADTYPE,curvilinear,ORDER,DIM)
 else
   stop 35
 end if
#endMacro

!$$$#beginMacro applyBcByArtificialDissipation(TYPE,DIM,ORDER)
!$$$ if( includeADinPressure.eq.1 )then
!$$$   if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 ) then
!$$$     applyBcByGridType(TYPE,AD2,ORDER,DIM)
!$$$   else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 ) then
!$$$     applyBcByGridType(TYPE,AD4,ORDER,DIM)
!$$$   else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 ) then
!$$$     applyBcByGridType(TYPE,AD24,ORDER,DIM)
!$$$   end if
!$$$ end if
!$$$ if( includeADinPressure.eq.0 .or. (use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 ) )then
!$$$   applyBcByGridType(TYPE,noAD,ORDER,DIM)
!$$$ end if
!$$$#endMacro

!******************************************************************************
!  Here is the main macro for filling in all the different cases
!******************************************************************************
#beginMacro assignPressureBoundaryCondition(TYPE,ORDER,DIM)
! if( (includeADinPressure.eq.1) )then
!   if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 ) then
!     applyBcByGridType(TYPE,AD2,ORDER,DIM)
!   else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 ) then
!     applyBcByGridType(TYPE,AD4,ORDER,DIM)
!   else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 ) then
!     applyBcByGridType(TYPE,AD24,ORDER,DIM)
!   end if
! end if
! if( (includeADinPressure.eq.0) .or. (use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 ) )then
   applyBcByGridType(TYPE,noAD,ORDER,DIM)
! end if
#endMacro

#beginMacro getNormal2d(i1,i2,i3,axis)
 an1 = rsxy(i1,i2,i3,axis,0)
 an2 = rsxy(i1,i2,i3,axis,1)
 aNormi = -is/max(epsx,sqrt(an1**2 + an2**2))
 an1=an1*aNormi
 an2=an2*aNormi
#endMacro

#beginMacro getNormal3d(i1,i2,i3,axis)
 an1 = rsxy(i1,i2,i3,axis,0)
 an2 = rsxy(i1,i2,i3,axis,1)
 an3 = rsxy(i1,i2,i3,axis,2)
 aNormi = -is/max(epsx,sqrt(an1**2 + an2**2+ an3**2))
 an1=an1*aNormi
 an2=an2*aNormi
 an3=an3*aNormi
#endMacro

! *******************************************************************************
!   Macro to assign the TRACTION (including free surface) BC on the pressure
! *******************************************************************************
#beginMacro assignFreeSurfaceBoundaryCondition()

  ! The free surface BC for pressure is
  !     n.sigma.n = gamma* kappa 
  !     -(p-p_a) + n.tau.n = gamma*kappa 
  ! 
  !   p = p_a  n.sigma.n + surfaceTension * 2 *H 
  !   H = mean-curvature = .5( 1/R_1 + 1/R_2)
  !       2 H = - div( normal )
  !
  !  E = xr .xr,   F = xr .xs,   G = xs .xs
  !  L = xrr.n ,   M = xrs.n ,   N = xvv.n
  !  2 H = (E*N - 2*F*M + G*L) / (E*G - F^2)
  !
  if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then 
    write(*,'(" --inspf-- add RHS to traction (or free surface) BC")')
    write(*,'("nd = ",1i3)') nd
  end if

  is = 1 -2*side ! for normal calculation to get outward normal

  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
   if( mask(i1,i2,i3).ne.0 )then


     ! -- Compute the mean curvature --
     !   Note: curvature is zero on a rectangular grid, so skip this part:
     if( surfaceTension.ne.0. .and. gridType.ne.rectangular )then
        
      if( nd.eq.2 )then

        rxi= rsxy(i1,i2,i3,0,0)
        ryi= rsxy(i1,i2,i3,0,1)
        sxi= rsxy(i1,i2,i3,1,0)
        syi= rsxy(i1,i2,i3,1,1)

        det = rxi*syi-sxi*ryi
        deti=1./max( detMin, det )

        xr = syi * deti
        yr =-sxi * deti
        xs =-ryi * deti
        ys = rxi * deti

        if( axis.eq.0 )then
          ! left or right side: tangential direction is "s"
          rxs = rsxys2(i1,i2,i3,0,0)
          rys = rsxys2(i1,i2,i3,0,1)
          sxs = rsxys2(i1,i2,i3,1,0)
          sys = rsxys2(i1,i2,i3,1,1)
          dets = rxs*syi + rxi*sys - sxs*ryi - sxi*rys

          xss = (-rys*det + ryi*dets )*( deti**2 )
          yss = ( rxs*det - rxi*dets )*( deti**2 )

          meanCurvature = .5*( xs*yss - ys*xss )/( (xs**2 + ys**2)**(1.5) )
          write(*,'(" i1,i2=",2i3," meanCurvature=",f6.2)') i1,i2,meanCurvature

        else if( axis.eq.1 )then
          ! top or bottom side : tangential direction is "r"
          rxr = rsxyr2(i1,i2,i3,0,0)
          ryr = rsxyr2(i1,i2,i3,0,1)
          sxr = rsxyr2(i1,i2,i3,1,0)
          syr = rsxyr2(i1,i2,i3,1,1)
          detr = rxr*syi + rxi*syr - sxr*ryi - sxi*ryr

          xrr = ( syr*det - syi*detr )*( deti**2 )
          yrr = (-sxr*det + sxi*detr )*( deti**2 )

          meanCurvature = .5*( xr*yrr - yr*xrr )/( (xr**2 + yr**2)**(1.5) )

        else
          stop 1009
        end if
        

        ! ---- add viscous stess contribution: n.tau.n ------
        !   tauv = mu [ 2*ux  (uy+vx) ]
        !             [ (uy+vx) 2*vy  ]
        ! nv.tauv.nv = 2*mu*[  ux*n1^2 + (uy+vx)*n1*n2 + vy*n2^2 ]

        ! *** CHECK ME ***

        ! normal vector (an1,an2): 
        getNormal2d(i1,i2,i3,axis)

        u0x =  ux22(i1,i2,i3,uc)
        u0y =  uy22(i1,i2,i3,uc)
        v0x =  ux22(i1,i2,i3,vc)
        v0y =  uy22(i1,i2,i3,vc)

        ! write(*,'("ux,uy=",e12.3,e12.3)') u0x,u0y
        ! write(*,'("vx,vy=",e12.3,e12.3)') v0x,v0y

        nTauN = 2.*mu*( u0x*an1**2 + (u0y+v0x)*an1*an2 + v0y*an2**2 )

      else if( nd.eq.3 )then

        ! ---- compute mean curvature ------
        ! 
        !   H = mean-curvature = .5( 1/R_1 + 1/R_2)
        !       2 H = - div( normal )
        !
        !  E = xr .xr,   F = xr .xs,   G = xs .xs
        !  L = xrr.n ,   M = xrs.n ,   N = xvv.n
        !  2 H = (E*N - 2*F*M + G*L) / (E*G - F^2)
        !
        ! todo, not all these computations are needed

        ! get components of jacobian
        rxi= rsxy(i1,i2,i3,0,0)
        ryi= rsxy(i1,i2,i3,0,1)
        rzi= rsxy(i1,i2,i3,0,2)
        sxi= rsxy(i1,i2,i3,1,0)
        syi= rsxy(i1,i2,i3,1,1)
        szi= rsxy(i1,i2,i3,1,2)
        txi= rsxy(i1,i2,i3,2,0)
        tyi= rsxy(i1,i2,i3,2,1)
        tzi= rsxy(i1,i2,i3,2,2)

        ! get r deriv of jacobian
        rxr= rsxyr2(i1,i2,i3,0,0)
        ryr= rsxyr2(i1,i2,i3,0,1)
        rzr= rsxyr2(i1,i2,i3,0,2)
        sxr= rsxyr2(i1,i2,i3,1,0)
        syr= rsxyr2(i1,i2,i3,1,1)
        szr= rsxyr2(i1,i2,i3,1,2)
        txr= rsxyr2(i1,i2,i3,2,0)
        tyr= rsxyr2(i1,i2,i3,2,1)
        tzr= rsxyr2(i1,i2,i3,2,2)

        ! get s deriv of jacobian
        rxs= rsxys2(i1,i2,i3,0,0)
        rys= rsxys2(i1,i2,i3,0,1)
        rzs= rsxys2(i1,i2,i3,0,2)
        sxs= rsxys2(i1,i2,i3,1,0)
        sys= rsxys2(i1,i2,i3,1,1)
        szs= rsxys2(i1,i2,i3,1,2)
        txs= rsxys2(i1,i2,i3,2,0)
        tys= rsxys2(i1,i2,i3,2,1)
        tzs= rsxys2(i1,i2,i3,2,2)

        ! get t deriv of jacobian
        rxt= rsxyt2(i1,i2,i3,0,0)
        ryt= rsxyt2(i1,i2,i3,0,1)
        rzt= rsxyt2(i1,i2,i3,0,2)
        sxt= rsxyt2(i1,i2,i3,1,0)
        syt= rsxyt2(i1,i2,i3,1,1)
        szt= rsxyt2(i1,i2,i3,1,2)
        txt= rsxyt2(i1,i2,i3,2,0)
        tyt= rsxyt2(i1,i2,i3,2,1)
        tzt= rsxyt2(i1,i2,i3,2,2)

        ! compute determinant
        det = rxi * syi * tzi - rxi * szi * tyi - ryi * sxi * tzi + ryi * szi * txi + rzi * sxi * tyi - rzi * syi * txi
        deti= 1./det

        ! write(*,'("(x,y,z) = (",f20.16,f20.16,f20.16,")")') xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2)
        ! write(*,'("(rxi,ryi,rzi) = ",f20.16,f20.16,f20.16)') rxi,ryi,rzi
        ! write(*,'("(sxi,syi,szi) = ",f20.16,f20.16,f20.16)') sxi,syi,szi
        ! write(*,'("(txi,tyi,tzi) = ",f20.16,f20.16,f20.16)') txi,tyi,tzi
        ! write(*,'("(det,deti) = ",f20.16,f20.16)') det,deti

        ! get components of inverse jacobian
        xr= deti*(syi*tzi-szi*tyi)
        xs= deti*(-ryi*tzi+rzi*tyi)
        xt= deti*(ryi*szi-rzi*syi)
        yr= deti*(-sxi*tzi+szi*txi)
        ys= deti*(rxi*tzi-rzi*txi)
        yt= deti*(-rxi*szi+rzi*sxi)
        zr= deti*(sxi*tyi-syi*txi)
        zs= deti*(-rxi*tyi+ryi*txi)
        zt= deti*(rxi*syi-ryi*sxi)

        ! write(*,'("(xr,xs,xt) = ",f20.16,f20.16,f20.16)') xr,xs,xt
        ! write(*,'("(yr,ys,yt) = ",f20.16,f20.16,f20.16)') yr,ys,yt
        ! write(*,'("(zr,zs,zt) = ",f20.16,f20.16,f20.16)') zr,zs,zt

        ! get r derivatives of mapping terms 
        axrr = syi*tzr+syr*tzi-szi*tyr-szr*tyi
        axsr = -ryi*tzr-ryr*tzi+rzi*tyr+rzr*tyi
        axtr = ryi*szr+ryr*szi-rzi*syr-rzr*syi
        ayrr = -sxi*tzr-sxr*tzi+szi*txr+szr*txi
        aysr = rxi*tzr+rxr*tzi-rzi*txr-rzr*txi
        aytr = -rxi*szr-rxr*szi+rzi*sxr+rzr*sxi
        azrr = sxi*tyr+sxr*tyi-syi*txr-syr*txi
        azsr = -rxi*tyr-rxr*tyi+ryi*txr+ryr*txi
        aztr = rxi*syr+rxr*syi-ryi*sxr-ryr*sxi

        ! get s derivatives of mapping terms 
        axrs = syi*tzs+sys*tzi-szi*tys-szs*tyi
        axss = -ryi*tzs-rys*tzi+rzi*tys+rzs*tyi
        axts = ryi*szs+rys*szi-rzi*sys-rzs*syi
        ayrs = -sxi*tzs-sxs*tzi+szi*txs+szs*txi
        ayss = rxi*tzs+rxs*tzi-rzi*txs-rzs*txi
        ayts = -rxi*szs-rxs*szi+rzi*sxs+rzs*sxi
        azrs = sxi*tys+sxs*tyi-syi*txs-sys*txi
        azss = -rxi*tys-rxs*tyi+ryi*txs+rys*txi
        azts = rxi*sys+rxs*syi-ryi*sxs-rys*sxi

        ! get t derivatives of mapping terms 
        axrt = syi*tzt+syt*tzi-szi*tyt-szt*tyi
        axst = -ryi*tzt-ryt*tzi+rzi*tyt+rzt*tyi
        axtt = ryi*szt+ryt*szi-rzi*syt-rzt*syi
        ayrt = -sxi*tzt-sxt*tzi+szi*txt+szt*txi
        ayst = rxi*tzt+rxt*tzi-rzi*txt-rzt*txi
        aytt = -rxi*szt-rxt*szi+rzi*sxt+rzt*sxi
        azrt = sxi*tyt+sxt*tyi-syi*txt-syt*txi
        azst = -rxi*tyt-rxt*tyi+ryi*txt+ryt*txi
        aztt = rxi*syt+rxt*syi-ryi*sxt-ryt*sxi

        ! derivatives of the determinant
        detr = rxi*syi*tzr+rxi*syr*tzi-rxi*szi*tyr-rxi*szr*tyi+rxr*syi*tzi-rxr*szi*tyi-ryi*sxi*tzr-ryi*sxr*tzi+ryi*szi*txr+ryi*szr*txi-ryr*sxi*tzi+ryr*szi*txi+rzi*sxi*tyr+rzi*sxr*tyi-rzi*syi*txr-rzi*syr*txi+rzr*sxi*tyi-rzr*syi*txi
        dets = rxi*syi*tzs+rxi*sys*tzi-rxi*szi*tys-rxi*szs*tyi+rxs*syi*tzi-rxs*szi*tyi-ryi*sxi*tzs-ryi*sxs*tzi+ryi*szi*txs+ryi*szs*txi-rys*sxi*tzi+rys*szi*txi+rzi*sxi*tys+rzi*sxs*tyi-rzi*syi*txs-rzi*sys*txi+rzs*sxi*tyi-rzs*syi*txi
        dett = rxi*syi*tzt+rxi*syt*tzi-rxi*szi*tyt-rxi*szt*tyi+rxt*syi*tzi-rxt*szi*tyi-ryi*sxi*tzt-ryi*sxt*tzi+ryi*szi*txt+ryi*szt*txi-ryt*sxi*tzi+ryt*szi*txi+rzi*sxi*tyt+rzi*sxt*tyi-rzi*syi*txt-rzi*syt*txi+rzt*sxi*tyi-rzt*syi*txi

        ! compute second derivatives 
        xrr = deti*( -xr*detr + axrr )
        yrr = deti*( -yr*detr + ayrr )
        zrr = deti*( -zr*detr + azrr )

        xrs = deti*( -xr*dets + axrs )
        yrs = deti*( -yr*dets + ayrs )
        zrs = deti*( -zr*dets + azrs )

        xrt = deti*( -xr*dett + axrt )
        yrt = deti*( -yr*dett + ayrt )
        zrt = deti*( -zr*dett + azrt )

        xss = deti*( -xs*dets + axss )
        yss = deti*( -ys*dets + ayss )
        zss = deti*( -zs*dets + azss )

        xst = deti*( -xs*dett + axst )
        yst = deti*( -ys*dett + ayst )
        zst = deti*( -zs*dett + azst )

        xtt = deti*( -xt*dett + axtt )
        ytt = deti*( -yt*dett + aytt )
        ztt = deti*( -zt*dett + aztt )

        ! get normal and derivatives
        getNormal3d(i1,i2,i3,axis)

        if( axis.eq.2 )then
          ! tangential directions are r and s

          ! calculate first fundamental form
          aEi = xr*xr+yr*yr+zr*zr
          aFi = xr*xs+yr*ys+zr*zs
          aGi = xs*xs+ys*ys+zs*zs

          ! write(*,'(" i1,i2,i3=",3i3," (xr,yr,zr)=",f20.16,f20.16,f20.16)') i1,i2,i3,xr,yr,zr

          ! calculate second fundamental form
          aLi = xrr*an1+yrr*an2+zrr*an3
          aMi = xrs*an1+yrs*an2+zrs*an3
          aNi = xss*an1+yss*an2+zss*an3

          meanCurvature = -.5*(aEi*aNi-2*aFi*aMi+aGi*aLi) \
                             /(aEi*aGi-aFi*aFi)

          ! write(*,'(" i1,i2,i3=",3i3," (E,F,G)=",f20.16,f20.16,f20.16)') i1,i2,i3,aEi,aFi,aGi
          ! write(*,'(" i1,i2,i3=",3i3," (L,M,N)=",f20.16,f20.16,f20.16)') i1,i2,i3,aLi,aMi,aNi
          ! write(*,'(" i1,i2,i3=",3i3," meanCurvature=",f6.2)') i1,i2,i3,meanCurvature
        else if( axis.eq.1 )then
          ! tangential directions are r and t
          stop 1011
        else 
          ! tangential directions are s and t
          stop 1012
        end if

        ! ---- add viscous stess contribution: n.tau.n ------
        !   tauv = mu [ 2*ux     (uy+vx)  (uz+wx)]
        !             [ (uy+vx)    2*vy   (vz+wy)]
        !             [ (uz+wx)  (vz+wy)    2*wz ]
        ! nv.tauv.nv = 2*mu*( n1^2*ux + n2^2*vy + n3^2*wz
        !                    +n1*n2*(uy+vx)
        !                    +n1*n3*(uz+wx)
        !                    +n2*n3*(vz+wy)               )
        !
        ! *** CHECK ME ***

        ! normal vector (an1,an2): 
        getNormal2d(i1,i2,i3,axis)

        ! get derivatives
        u0x =  ux23(i1,i2,i3,uc)
        u0y =  uy23(i1,i2,i3,uc)
        u0z =  uz23(i1,i2,i3,uc)
        v0x =  ux23(i1,i2,i3,vc)
        v0y =  uy23(i1,i2,i3,vc)
        v0z =  uz23(i1,i2,i3,vc)
        w0x =  ux23(i1,i2,i3,wc)
        w0y =  uy23(i1,i2,i3,wc)
        w0z =  uz23(i1,i2,i3,wc)

        ! write(*,'("ux,uy,uz=",e12.3,e12.3,e12.3)') u0x,u0y,u0z
        ! write(*,'("vx,vy,vz=",e12.3,e12.3,e12.3)') v0x,v0y,v0z
        ! write(*,'("wx,wy,wz=",e12.3,e12.3,e12.3)') w0x,w0y,w0z
        ! write(*,'("n1,n2,n3=",e12.3,e12.3,e12.3)') an1,an2,an3
        ! write(*,'("mu=",e12.3)') mu

        nTauN = 2*mu*( u0x*an1**2 + v0y*an2**2 + w0z*an3**2 \
                      +an1*an2*(u0y+v0x) \
                      +an1*an3*(u0z+w0x) \
                      +an2*an3*(v0z+w0y))

        ! write(*,'("nTauN=",e12.3)') nTauN
        ! write(*,'("comp1=",e12.3)') u0x*an1**2
        ! write(*,'("comp1=",e12.3)') v0y*an2**2
        ! write(*,'("comp1=",e12.3)') an1*an2*(u0y+v0x)
        ! nTauN = 2.*mu*( u0x*an1**2 + (u0y+v0x)*an1*an2 + v0y*an2**2 )
        ! write(*,'("nTauN=",e12.3)') nTauN
      else
        stop 8257
      end if

      if( t.le.10.*dt )then
        write(*,'(" i1,i2,i3=",3i3," pAtm=",f12.8," meanCurvature=",f12.8," mu=",e9.3," n.tau.n=",e10.3)') i1,i2,i3,pAtmosphere,meanCurvature,mu,nTauN
      end if

      f(i1,i2,i3)= pAtmosphere + 2.*surfaceTension*meanCurvature + nTauN 

    else
      ! surfaceTension==0 : 
      f(i1,i2,i3)= pAtmosphere
    end if

     ! 2014/12/17 -- add forcing to traction BC
     if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then 
        f(i1,i2,i3)= f(i1,i2,i3) + bcf(side,axis,i1,i2,i3,pc)
     end if

   end if ! end if mask 
  end do
  end do
  end do
#endMacro


! *******************************************************************************
!
! TYPE: INS, INSSPAL, INSBL
! ADTYPE: noAD, AD2, AD4, AD24
! GRIDTYPE: rectangular, curvilinear
! ORDER: 2,4
! DIM: 2,3
! AXIS: R,S,T
! 
! *******************************************************************************
#beginMacro buildPressureFunction(TYPE,ORDER,DIM)

 subroutine assignPressureRhs ## TYPE ## ORDER ## DIM(nd,  \
      n1a,n1b,n2a,n2b,n3a,n3b,  \
      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,  \
      mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData,   \
      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,  \
      normal00,normal10,normal01,normal11,normal02,normal12,  \
      dim, bcf0,bcOffset,addBoundaryForcing, \
      ipar, rpar, ierr )

 INITIALIZE()


 #If #TYPE == "INS"
 #Elif #TYPE == "INSTP"
 #Elif #TYPE == "INSVP"
 #Elif #TYPE == "INSBL"
 #Elif #TYPE == "INSSPAL"
  call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, \
           cv1e3, cd0, cr0)
 #Elif #TYPE == "INSKE"
  ! call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
 #Else
   stop 36
 #End

 if( gridType.eq.rectangular )then
  assignPressureRHS(TYPE,DIM,ORDER,rectangular)
 else ! curvilinear
  assignPressureRHS(TYPE,DIM,ORDER,curvilinear)
 end if

!     ***************** assign RHS for BC ********************      
!**      if( gridType.ne.rectangular )then
!**         write(*,*) 'ERROR:assignPressureRHSOpt gridType.ne.rectangular'
!**         stop 1
!**        return
!**      end if

 do axis=0,nd-1
  axisp1=mod(axis+1,nd)
  axisp2=mod(axis+2,nd)

  do side=0,1

   n1a=indexRange(0,0)
   n1b=indexRange(1,0)
   n2a=indexRange(0,1)
   n2b=indexRange(1,1)
   n3a=indexRange(0,2)
   n3b=indexRange(1,2)
   is1=0
   is2=0
   is3=0
   if( axis.eq.0 )then
     n1a=indexRange(side,axis)
     n1b=indexRange(side,axis)
     is1=2*side-1
   else if( axis.eq.1 )then
     n2a=indexRange(side,axis)
     n2b=indexRange(side,axis)
     is2=2*side-1
   else
     n3a=indexRange(side,axis)
     n3b=indexRange(side,axis)
     is3=2*side-1
   end if

   ! an : outward normal on a Cartesian grid
   an(0)=0.
   an(1)=0.
   an(2)=0.
   an(axis)=2*side-1 

   bc0=bc(side,axis)


   if( bc0.le.0 )then
     ! do nothing
   else if( bc0.eq.outflow .or. \
           bc0.eq.convectiveOutflow ) then

     a1=bcData(pc+numberOfComponents*2,side,axis) ! coeff of p.n

     ! write(*,*) 'pressureBC opt: pc,nc,side,axis,a1=',pc,numberOfComponents,side,axis,a1
     if( a1.ne.0. ) then
       ! printf("**apply mixed BC on pressure rhs...\n");
       ! if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then ! *wdh* 2013/12/01
       ! *wdh* 2014/11/21 - turn off RHS when projecting initial conditions:
       ! *wdh* 2016/11/25 -- make sure to use zero RHS when projecting initial conditions:
       ! if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then 
       if( addBoundaryForcing(side,axis).ne.0 )then
         if( initialConditionsAreBeingProjected.eq.0 )then 
          ! write(*,'("inspf:INFO: set pressure profile at outflow")')
          loopse4(f(i1+is1,i2+is2,i3+is3)=bcf(side,axis,i1,i2,i3,pc),,,)
         else
          loopse4(f(i1+is1,i2+is2,i3+is3)=0.,,,)
         end if
       else 
        loopse4(f(i1+is1,i2+is2,i3+is3)=bcData(pc,side,axis),,,)
       end if
     else
       ! dirichlet :
       ! if( addBoundaryForcing(side,axis).ne.0 )then ! *wdh* 2013/12/01
       ! *wdh* 2014/11/21 - turn off RHS when projecting initial conditions:
       ! *wdh* 2016/11/25 -- make sure to use zero RHS when projecting initial conditions:
       ! if( addBoundaryForcing(side,axis).ne.0 .and. initialConditionsAreBeingProjected.eq.0 )then 
       if( addBoundaryForcing(side,axis).ne.0 )then
         if( initialConditionsAreBeingProjected.eq.0 )then 
          ! write(*,'("inspf:INFO: set pressure profile at outflow")')
          loopse4(f(i1,i2,i3)=bcf(side,axis,i1,i2,i3,pc),,,)
         else
          loopse4(f(i1,i2,i3)=0.,,,)
         end if
       else
        loopse4(f(i1,i2,i3)=bcData(pc,side,axis),,,)
       end if
       ! for extrapolation :
       loopse4(f(i1+is1,i2+is2,i3+is3)=0.,,,)
     end if

   else if( bc0.eq.inflowWithPandTV .or.     \
            bc0.eq.dirichletBoundaryCondition )then

     if( addBoundaryForcing(side,axis).ne.0 )then ! *wdh* 2013/12/01
      loopse4(f(i1,i2,i3)=bcf(side,axis,i1,i2,i3,pc),,,)
     else
      inflowPressure=bcData(pc,side,axis) ! *wdh* 100809
      loopse4(f(i1,i2,i3)=inflowPressure,,,)
     end if

   else if( bc0.eq.freeSurfaceBoundaryCondition .or. \
            bc0.eq.tractionFree )then

     ! Free surface and tractionFree are really the same thing *wdh* 2014/12/17

     assignFreeSurfaceBoundaryCondition()

   else if( bc0.eq.symmetry .or. bc0.eq.axisymmetric .or. pressureBC.eq.2 ) then
      !  p.n=0
     loopse4(f(i1+is1,i2+is2,i3+is3)=0.,,,)

   else 
!         ********* wall condition *********

     assignPressureBoundaryCondition(TYPE,ORDER,DIM)

   end if ! end bc

  end do ! side
 end do ! axis

 return
 end

#endMacro

! *******************************************************************************
!  Null version
! *******************************************************************************
#beginMacro buildPressureFunctionNull(TYPE,ORDER,DIM)

 subroutine assignPressureRhs ## TYPE ## ORDER ## DIM(nd,  \
      n1a,n1b,n2a,n2b,n3a,n3b,  \
      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,  \
      mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData,   \
      nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,  \
      normal00,normal10,normal01,normal11,normal02,normal12,  \
      dim, bcf0,bcOffset,addBoundaryForcing,\
      ipar, rpar, ierr )
! *******************************************************************************
!  Null version
! *******************************************************************************


 write(*,'("ERROR: NULL version of assignPressureRhs ## TYPE ## ORDER ## DIM called")')
 write(*,'(" You may have to turn on an option in the Makefile.")')
 ! ' 
 stop 1070

 return
 end

#endMacro

      subroutine assignPressureRHSOpt(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, 
     & nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,
     & normal00,normal10,normal01,normal11,normal02,normal12,
     & dim, bcf0,bcOffset,addBoundaryForcing,
     & ipar, rpar, ierr )
!======================================================================
!   Assign the RHS for the pressure equations.
!     OPTIMIZED version for rectangular grids.
!
! nd : number of space dimensions
!
! bc (input) : NOTE this is a special bc array with value defining the various pressure BC's
!
! NOTE:
!   u==uu for a non-moving grid. For a moving grid, uu is a temp space to hold u-gv
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndb
      integer nr1a,nr1b,nr2a,nr2b,nr3a,nr3b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real udf(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:*)          ! user defined force
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real bcData(0:ndb-1,0:1,0:2)

      real normal00(nr1a:nr1a,nd2a:nd2b,nd3a:nd3b,0:*)
      real normal10(nr1b:nr1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real normal01(nd1a:nd1b,nr2a:nr2a,nd3a:nd3b,0:*)
      real normal11(nd1a:nd1b,nr2b:nr2b,nd3a:nd3b,0:*)
      real normal02(nd1a:nd1b,nd2a:nd2b,nr3a:nr3a,0:*)
      real normal12(nd1a:nd1b,nd2a:nd2b,nr3b:nr3b,0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),indexRange(0:1,0:2), ierr

      integer ipar(0:*)
      real rpar(0:*)
      
      ! -- variables for boundary forcing (bcData)
      integer dim(0:1,0:2,0:1,0:2), addBoundaryForcing(0:1,0:2)
      real bcf0(0:*)
      integer*8 bcOffset(0:1,0:2)

!     ---- local variables -----
      integer c,i1,i2,i3,orderOfAccuracy,gridIsMoving,useWhereMask
      integer isAxisymmetric,pressureBC,gridType,initialConditionsAreBeingProjected
      integer pc,uc,vc,wc,tc,nc,grid,numberOfComponents
      real advectionCoefficient


      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

      ierr=0
      ! write(*,*) 'Inside assignPressureRHSOpt'

      pc                   =ipar(0)
      uc                   =ipar(1)
      vc                   =ipar(2)
      wc                   =ipar(3)
      tc                   =ipar(4) ! **new**
      nc                   =ipar(5)
      grid                 =ipar(6)
      orderOfAccuracy      =ipar(7)
      gridIsMoving         =ipar(8)
      useWhereMask         =ipar(9)
      isAxisymmetric       =ipar(10)
      pressureBC           =ipar(11)
      numberOfComponents   =ipar(12)
      gridType             =ipar(13)
      turbulenceModel      =ipar(14)

      pdeModel             =ipar(19)

      initialConditionsAreBeingProjected=ipar(23)

      advectionCoefficient=rpar(7)

      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("assignPressureRHSOpt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if
      if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
        write(*,'("assignPressureRHSOpt:ERROR gridType=",i6)') gridType
        stop 2
      end if
      if( numberOfComponents.le.nd )then
        write(*,'("assignPressureRHSOpt:ERROR nd,numberOfComponents=",2i6)') nd,numberOfComponents
        ! '
        stop 3
      end if
      if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
        write(*,'("assignPressureRHSOpt:ERROR uc,vc,ws=",2i6)') uc,vc,wc
        stop 4
      end if
      if( initialConditionsAreBeingProjected.lt.0 .or. initialConditionsAreBeingProjected.gt.1 )then
        write(*,'("assignPressureRHSOpt:ERROR initialConditionsAreBeingProjected=",i6)') initialConditionsAreBeingProjected
        ! '
        stop 4
      end if

      if( gridIsMoving.ne.0 )then
        ! compute uu = u -gv    *wdh* 080418  added advectionCoefficient here instead of above
        if( nd.eq.2 )then
          beginLoops()
            uu(i1,i2,i3,uc)=advectionCoefficient*u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
            uu(i1,i2,i3,vc)=advectionCoefficient*u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
          endLoops()
        else if( nd.eq.3 )then
          beginLoops()
            uu(i1,i2,i3,uc)=advectionCoefficient*u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
            uu(i1,i2,i3,vc)=advectionCoefficient*u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
            uu(i1,i2,i3,wc)=advectionCoefficient*u(i1,i2,i3,wc)-gv(i1,i2,i3,2)
          endLoops()
        else
          stop 11
        end if
      end if

!     *********** assign interior forcing ******************    

      ! when the advectionCoefficient==0 we are projecting the initial conditions and we 
      ! only need to include the div-damping term.
      ! *wdh* 080418 if( turbulenceModel.eq.noTurbulenceModel .or. advectionCoefficient.eq.0. )then
      if( turbulenceModel.eq.noTurbulenceModel .or. initialConditionsAreBeingProjected.eq.1 )then
!          *********************************************************
!          ***********No turbulence model***************************
!          *********************************************************
       if( pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel )then
        if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call assignPressureRhsINS22(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINS23(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           end if
         else if( orderOfAccuracy.eq.4 )then 
          if( nd.eq.2 )then
            call assignPressureRhsINS42(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINS43(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           end if
         else
           stop 111
         end if
       
       else if( pdeModel.eq.viscoPlasticModel )then
        ! new way : all turbulence models can use this generic one ---

        if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call assignPressureRhsINSVP22(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINSVP23(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           end if
         else if( orderOfAccuracy.eq.4 )then 
          if( nd.eq.2 )then
            call assignPressureRhsINSVP42(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINSVP43(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
            stop 5524
           end if
         else
           stop 213
         end if

       else if( pdeModel.eq.twoPhaseFlowModel  )then

        if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call assignPressureRhsINSTP22(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINSTP23(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           end if
         else if( orderOfAccuracy.eq.4 )then 
          if( nd.eq.2 )then
!            call assignPressureRhsINSTP42(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
!             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
!             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
             stop 5524
           else
!            call assignPressureRhsINSTP43(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
!             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
!             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
            stop 5524
           end if
         else
           stop 213
         end if

       else
         write(*,'(" ERROR: unknown pdeModel")') 
         stop 8823
       end if

      else if( turbulenceModel.eq.spalartAllmaras )then
!          *********************************************************
!          ********spalartAllmaras turbulence model*****************
!          *********************************************************

        if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call assignPressureRhsINSSPAL22(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINSSPAL23(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           end if
         else
           stop 111
         end if

      else if( turbulenceModel.eq.baldwinLomax .or. turbulenceModel.eq.kEpsilon .or. turbulenceModel.eq.largeEddySimulation  )then
!          *********************************************************
!          ********Generic turbulence model: BL, KE   **************
!          *********************************************************
        ! new way : all turbulence models can use this generic one (VP) ---

        if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call assignPressureRhsINSVP22(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINSVP23(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           end if
         else if( orderOfAccuracy.eq.4 )then 
          if( nd.eq.2 )then
            call assignPressureRhsINSVP42(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           else
            call assignPressureRhsINSVP43(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
           end if
         else
           stop 213
         end if


!$$$
!$$$        if( orderOfAccuracy.eq.2 )then
!$$$          if( nd.eq.2 )then
!$$$            call assignPressureRhsINSBL22(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
!$$$             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
!$$$             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
!$$$           else
!$$$            call assignPressureRhsINSBL23(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
!$$$             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
!$$$             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
!$$$           end if
!$$$         else
!$$$           stop 111
!$$$         end if

!$$$      else if( turbulenceModel.eq.kEpsilon )then
!$$$c          *********************************************************
!$$$c          ********kEpsilon turbulence model*****************
!$$$c          *********************************************************
!$$$
!$$$        if( orderOfAccuracy.eq.2 )then
!$$$          if( nd.eq.2 )then
!$$$            call assignPressureRhsINSKE22(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
!$$$             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
!$$$             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
!$$$           else
!$$$            call assignPressureRhsINSKE23(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
!$$$             mask,xy,rsxy,radiusInverse,  u,uu,f,gv,divDamping,udf,  bc, indexRange, ndb, bcData, nr1a,nr1b,nr2a,nr2b,nr3a,nr3b,\
!$$$             normal00,normal10,normal01,normal11,normal02,normal12, dim,bcf0,bcOffset,addBoundaryForcing, ipar, rpar, ierr )
!$$$           end if
!$$$         else
!$$$           stop 111
!$$$         end if

      else
        write(*,'("inspf:ERROR: turbulenceModel=",i6," not implemented")') turbulenceModel
        ! '
        stop 62

      end if


      return 
      end


! ***************************************************************
!   Build different versions of the pressure function
!  Also build "null" versions" for use when we don't want the option
! ***************************************************************
#beginMacro buildFunction(fileName,TYPE,ORDER,DIM)

#beginFile src/fileName.f
 buildPressureFunction(TYPE,ORDER,DIM)
#endFile

#beginFile src/fileName ## Null.f
 buildPressureFunctionNull(TYPE,ORDER,DIM)
#endFile

#endMacro

!t:  temporarily turn off the building of these files 
!     Here we create subroutines in separate files to define the pressure BC
      ! order=2, 2d and 3d
      buildFunction(inspINS22,INS,2,2)
      buildFunction(inspINS23,INS,2,3)
      ! order=4
      buildFunction(inspINS42,INS,4,2)
      buildFunction(inspINS43,INS,4,3)

      ! *************** SPAL ***************************
      ! order=2
      buildFunction(inspINSSPAL22,INSSPAL,2,2)
      buildFunction(inspINSSPAL23,INSSPAL,2,3)
      ! order=4
!     buildFunction(inspINSSPAL42,INSSPAL,4,2)
!     buildFunction(inspINSSPAL43,INSSPAL,4,3)

      ! *************** BL ***************************
      ! order=2
!      buildFunction(inspINSBL22,INSBL,2,2)
!      buildFunction(inspINSBL23,INSBL,2,3)
      ! order=4
!     buildFunction(inspINSBL42,INSBL,4,2)
!     buildFunction(inspINSBL43,INSBL,4,3)

      ! *************** KE ***************************
      ! order=2
!      buildFunction(inspINSKE22,INSKE,2,2)
!      buildFunction(inspINSKE23,INSKE,2,3)

!t ---

      ! *************** Visco-plastic ***************************
      ! order=2
      buildFunction(inspINSVP22,INSVP,2,2)
      buildFunction(inspINSVP23,INSVP,2,3)
      ! order=4 
      buildFunction(inspINSVP42,INSVP,4,2)
      buildFunction(inspINSVP43,INSVP,4,3)

      ! *************** Variable density INS ***************************
      buildFunction(inspINSTP22,INSTP,2,2)
      buildFunction(inspINSTP23,INSTP,2,3)
