c ==============================================================================
c  Incompressible NS K-Epsilon explicit discretization
c ==============================================================================

#Include "insdt.h"

c =============================================================
c Compute k-epsilon quantities
c   This macro assumes u0x,u0y, ... are defined
c =============================================================
#beginMacro setupKEpsilon(DIM)
 k0=u(i1,i2,i3,kc)
 e0=u(i1,i2,i3,ec)
 nuT = cMu*k0**2/e0
 nuP=nu+nuT
 k0x=UX(kc)
 e0x=UX(ec)
 nuTx=cMu*k0*( 2.*k0x*e0 - k0*e0x )/e0**2
 k0y=UY(kc)
 e0y=UY(ec)
 nuTy=cMu*k0*( 2.*k0y*e0 - k0*e0y )/e0**2
#If #DIM == "2"
 prod = nuT*( 2.*(u0x**2+v0y**2) + (v0x+u0y)**2 )
#Else
 prod = nuT*( 2.*(u0x**2+v0y**2+w0z**2) +(v0x+u0y)**2 +(w0y+v0z)**2 +(u0z+w0x)**2  )  
 k0z=UZ(kc)
  e0z=UZ(ec)
  nuTz=cMu*k0*( 2.*k0z*e0 - k0*e0z )/e0**2
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

 ! INS with k-epsilon turbulence model
 setupDerivatives(DIM)
 setupKEpsilon(DIM)
 #If #DIM == "2"
  ut(i1,i2,i3,uc)= -UU(uc)*u0x-UU(vc)*u0y-UX(pc)+nuP*ULAP(uc)+nuTx*(2.*u0x    ) +nuTy*(u0y+v0x)
  ut(i1,i2,i3,vc)= -UU(uc)*v0x-UU(vc)*v0y-UY(pc)+nuP*ULAP(vc)+nuTx*(u0y+v0x) +nuTy*(2.*v0y)
  ut(i1,i2,i3,kc)= -UU(uc)*k0x-UU(vc)*k0y +prod -U(ec)\
            +(nu+sigmaKI*nuT)*ULAP(kc)+sigmaKI*(nuTx*k0x+nuTy*k0y)
  ut(i1,i2,i3,ec)= -UU(uc)*e0x-UU(vc)*e0y\
    +cEps1*(U(ec)/U(kc))*prod-cEps2*(U(ec)**2/U(kc))+(nu+sigmaEpsI*nuT)*ULAP(ec)+sigmaEpsI*(nuTx*e0x+nuTy*e0y)
 #Else
  ut(i1,i2,i3,uc)= -UU(uc)*u0x-UU(vc)*u0y-UU(wc)*u0z-UX(pc)+nuP*ULAP(uc)\
                   +nuTx*(2.*u0x    ) +nuTy*(u0y+v0x) +nuTz*(u0z+w0x)
  ut(i1,i2,i3,vc)= -UU(uc)*v0x-UU(vc)*v0y-UU(wc)*v0z-UY(pc)+nuP*ULAP(vc)\
                   +nuTx*(u0y+v0x) +nuTy*(2.*v0y) +nuTz*(v0z+w0y)
  ut(i1,i2,i3,wc)= -UU(uc)*w0x-UU(vc)*w0y-UU(wc)*w0z-UZ(pc)+nuP*ULAP(wc)\
                   +nuTx*(u0z+w0x) +nuTy*(v0z+w0y) +nuTz*(2.*w0z)
  ut(i1,i2,i3,kc)= -UU(uc)*k0x-UU(vc)*k0y-UU(wc)*k0z+prod -U(ec)\
                        +(nu+sigmaKI*nuT)*ULAP(kc)+sigmaKI*(nuTx*k0x+nuTy*k0y+nuTz*k0z)
  ut(i1,i2,i3,ec)= -UU(uc)*e0x-UU(vc)*e0y-UU(wc)*e0z+cEps1*(U(ec)/U(kc))*prod-cEps2*(U(ec)**2/U(kc))\
           +(nu+sigmaEpsI*nuT)*ULAP(ec)+sigmaEpsI*(nuTx*e0x+nuTy*e0y+nuTz*e0z)
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
  stop 88

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


      buildFile(insdtKE2dOrder2,2,2)
      buildFile(insdtKE2dOrder4,2,4)
      buildFile(insdtKE3dOrder2,3,2)
      buildFile(insdtKE3dOrder4,3,4)
