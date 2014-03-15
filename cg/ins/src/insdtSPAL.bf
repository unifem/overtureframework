c ==============================================================================
c  Incompressible NS Spalart-Almaras explicit discretization
c ==============================================================================


#Include "insdt.h"

c =============================================================
c Compute Spalart-Allmaras quantities
c   This macro assumes u0x,u0y, ... are defined
c =============================================================
#beginMacro setupSpalartAllmaras(DIM)
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

 ! INS with Spalart-Allmaras turbulence model
 setupDerivatives(DIM)
 setupSpalartAllmaras(DIM)
 #If #DIM == "2"
  ut(i1,i2,i3,uc)= -UU(uc)*u0x-UU(vc)*u0y-UX(pc)+nuT*ULAP(uc)+nuTx*(2.*u0x    ) +nuTy*(u0y+v0x)
  ut(i1,i2,i3,vc)= -UU(uc)*v0x-UU(vc)*v0y-UY(pc)+nuT*ULAP(vc)+nuTx*(u0y+v0x) +nuTy*(2.*v0y)
  ut(i1,i2,i3,nc)= -UU(uc)*n0x-UU(vc)*n0y + cb1*s*U(nc) + sigmai*(nu+U(nc))*(ULAP(nc))\
                   + ((1.+cb2)*sigmai)*(n0x**2+n0y**2)- nSqBydSq
 #Else
  ut(i1,i2,i3,uc)= -UU(uc)*u0x-UU(vc)*u0y-UU(wc)*u0z-UX(pc)+nuT*ULAP(uc)\
                   +nuTx*(2.*u0x    ) +nuTy*(u0y+v0x) +nuTz*(u0z+w0x)
  ut(i1,i2,i3,vc)= -UU(uc)*v0x-UU(vc)*v0y-UU(wc)*v0z-UY(pc)+nuT*ULAP(vc)\
                   +nuTx*(u0y+v0x) +nuTy*(2.*v0y) +nuTz*(v0z+w0y)
  ut(i1,i2,i3,wc)= -UU(uc)*w0x-UU(vc)*w0y-UU(wc)*w0z-UZ(pc)+nuT*ULAP(wc)\
                   +nuTx*(u0z+w0x) +nuTy*(v0z+w0y) +nuTz*(2.*w0z)
  ut(i1,i2,i3,nc)= -UU(uc)*n0x-UU(vc)*n0y-UU(wc)*n0z \
       + cb1*s*U(nc) + sigmai*(nu+U(nc))*(ULAP(nc))\
       + ((1.+cb2)*sigmai)*(n0x**2+n0y**2+n0z**2) - nSqBydSq
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

      buildFile(insdtSPAL2dOrder2,2,2)
      buildFile(insdtSPAL2dOrder4,2,4)
      buildFile(insdtSPAL3dOrder2,3,2)
      buildFile(insdtSPAL3dOrder4,3,4)

