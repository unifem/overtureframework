c ==============================================================================
c  Incompressible NS Visco-Plastic explicit discretization
c ==============================================================================

#Include "insdt.h"

c =============================================================================
c Evaluate the coefficients of the visco-plastic model
c ============================================================================
#beginMacro getViscoPlasticCoefficients(DIM)
 nuT = u(i1,i2,i3,vsc)
 nuTx=UX(vsc)
 nuTy=UY(vsc)
 #If #DIM == "3" 
  nuTz=UZ(vsc)
 #End
#endMacro


c====================================================================
c This macro will build the statements that form the body of the loop
c
c IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
c SCALAR: NONE
c         PASSIVE - include equations for a passive scalar
c AXISYMMETRIC : YES or NO
c====================================================================
#beginMacro buildEquations(IMPEXP,SCALAR,DIM,ORDER,GRIDTYPE,AXISYMMETRIC)

#If #AXISYMMETRIC == "NO"

#If #IMPEXP == "EXPLICIT"

 ! INS Visco-plastic  *** finish this ****
 setupDerivatives(DIM)
 getViscoPlasticCoefficients(DIM)
 #If #DIM == "2"
  ut(i1,i2,i3,uc)= -UU(uc)*u0x-UU(vc)*u0y-UX(pc)+nuT*ULAP(uc)+nuTx*(2.*u0x    ) +nuTy*(u0y+v0x)
  ut(i1,i2,i3,vc)= -UU(uc)*v0x-UU(vc)*v0y-UY(pc)+nuT*ULAP(vc)+nuTx*(u0y+v0x) +nuTy*(2.*v0y)
 #Else
  ut(i1,i2,i3,uc)= -UU(uc)*u0x-UU(vc)*u0y-UU(wc)*u0z-UX(pc)+nuT*ULAP(uc)\
                   +nuTx*(2.*u0x    ) +nuTy*(u0y+v0x) +nuTz*(u0z+w0x)
  ut(i1,i2,i3,vc)= -UU(uc)*v0x-UU(vc)*v0y-UU(wc)*v0z-UY(pc)+nuT*ULAP(vc)\
                   +nuTx*(u0y+v0x) +nuTy*(2.*v0y) +nuTz*(v0z+w0y)
  ut(i1,i2,i3,wc)= -UU(uc)*w0x-UU(vc)*w0y-UU(wc)*w0z-UZ(pc)+nuT*ULAP(wc)\
                   +nuTx*(u0z+w0x) +nuTy*(v0z+w0y) +nuTz*(2.*w0z)
 #End


#Elif #IMPEXP == "EXPLICIT_ONLY" || #IMPEXP == "BOTH"
 ! explicit terms only, no diffusion
 #If #DIM == "2"
  ut(i1,i2,i3,uc)= -UU(uc)*UX(uc)-UU(vc)*UY(uc)-UX(pc)
  ut(i1,i2,i3,vc)= -UU(uc)*UX(vc)-UU(vc)*UY(vc)-UY(pc)
 #Else
  ut(i1,i2,i3,uc)= -UU(uc)*UX(uc)-UU(vc)*UY(uc)-UU(wc)*UZ(uc)-UX(pc)
  ut(i1,i2,i3,vc)= -UU(uc)*UX(vc)-UU(vc)*UY(vc)-UU(wc)*UZ(vc)-UY(pc)
  ut(i1,i2,i3,wc)= -UU(uc)*UX(wc)-UU(vc)*UY(wc)-UU(wc)*UZ(wc)-UZ(pc)
 #End
#Else
  stop 788

#End

#If #IMPEXP == "BOTH"
 ! include implicit terms - diffusion
 #If #DIM == "2"
  uti(i1,i2,i3,uc)= nu*ULAP(uc)
  uti(i1,i2,i3,vc)= nu*ULAP(vc)
 #Elif #DIM == "3"
  uti(i1,i2,i3,uc)= nu*ULAP(uc)
  uti(i1,i2,i3,vc)= nu*ULAP(vc)
  uti(i1,i2,i3,wc)= nu*ULAP(wc)
 #Else
   stop 11
 #End

#End

! end AXISYMMETRIC eq NO: 
#End

#endMacro 


      ! Visco-plastic case
      buildFile(insdtVP2dOrder2,2,2)
      buildFile(insdtVP3dOrder2,3,2)
      buildFile(insdtVP2dOrder4,2,4)
      buildFile(insdtVP3dOrder4,3,4)
