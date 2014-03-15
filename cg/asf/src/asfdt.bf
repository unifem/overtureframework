c
c Compute du/dt for the compressible all-speed Navier-Stokes
c


c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

#Include "commonMacros.h"


#beginMacro loopse1(e1)
if( useWhereMask.ne.0 )then
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
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
  if( mask(i1,i2,i3).gt.0 )then
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
    if( mask(i1,i2,i3).gt.0 )then
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
    if( mask(i1,i2,i3).gt.0 )then
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




c Define the artificial diffusion coefficients
c gt should be R or C (gridType is Rectangular or Curvilinear)
c tb should be blank or SA  (SA=Spalart-Allamras turbulence model)
#beginMacro defineArtificialDiffusionCoefficients(dim,gt,tb)
  #If #dim == "2" 
    cdmz=admz ## gt ## tb(i1  ,i2  ,i3)
    cdpz=admz ## gt ## tb(i1+1,i2  ,i3)
    cdzm=adzm ## gt ## tb(i1  ,i2  ,i3)
    cdzp=adzm ## gt ## tb(i1  ,i2+1,i3)
    cdDiag=cdmz+cdpz+cdzm+cdzp
    ! write(*,'(1x,''asfdt:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
  #Else
    cdmzz=admzz ## gt ## tb(i1  ,i2  ,i3  )
    cdpzz=admzz ## gt ## tb(i1+1,i2  ,i3  )
    cdzmz=adzmz ## gt ## tb(i1  ,i2  ,i3  )
    cdzpz=adzmz ## gt ## tb(i1  ,i2+1,i3  )
    cdzzm=adzzm ## gt ## tb(i1  ,i2  ,i3  )
    cdzzp=adzzm ## gt ## tb(i1  ,i2  ,i3+1)
    cdDiag=cdmzz+cdpzz+cdzmz+cdzpz+cdzzm+cdzzp
  #End
#endMacro

c Define macros for the derivatives based on the dimension, order of accuracy and grid-type
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
   #Else
     #defineMacro UX(cc) ux22(i1,i2,i3,cc)
     #defineMacro UY(cc) uy22(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx22(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy22(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy22(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian22(i1,i2,i3,cc)
   #End
 #Else
   #If #GRIDTYPE == "rectangular" 
     #defineMacro UX(cc) ux42r(i1,i2,i3,cc)
     #defineMacro UY(cc) uy42r(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx42r(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy42r(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy42r(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian42r(i1,i2,i3,cc)
   #Else
     #defineMacro UX(cc) ux42(i1,i2,i3,cc)
     #defineMacro UY(cc) uy42(i1,i2,i3,cc)
     #defineMacro UXX(cc) uxx42(i1,i2,i3,cc)
     #defineMacro UXY(cc) uxy42(i1,i2,i3,cc)
     #defineMacro UYY(cc) uyy42(i1,i2,i3,cc)
     #defineMacro ULAP(cc) ulaplacian42(i1,i2,i3,cc)
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
   #Else
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
   #Else
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
   #End
 #End
#End
#endMacro 


c =============================================================
c Compute derivatives of u,v,w 
c =============================================================
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
c OPTION: ASF : ASF equations
c         ASFSPAL - ASF + Spalart-Allmaras turbulence model
c         ASF-BL - ASF + Baldwin Lomax turbulence model
c SCALAR: NONE
c         PASSIVE - include equations for a passive scalar
c AXISYMMETRIC : YES or NO
c====================================================================
#beginMacro buildEquations(IMPEXP,OPTION,SCALAR,DIM,ORDER,GRIDTYPE,AXISYMMETRIC)
#If #IMPEXP == "EXPLICIT"

 #If #OPTION == "ASF" 
  ! ASF, no AD

  rhoi=1./UU(rc) 
  rL0i=1./rL(i1,i2,i3)
  pL0 = pL(i1,i2,i3)
  p0=UU(pc)
  #If #DIM == "2"
   u0x=UX(uc)
   u0y=UY(uc)
   v0x=UX(vc)
   v0y=UY(vc)
   p0x=UX(pc)
   p0y=UY(pc)
   divu=u0x+v0y

   ut(i1,i2,i3,rc)= -UU(uc)*UX(rc)-UU(vc)*UY(rc) -UU(rc)*divu + nuRho*ULAP(rc)
   ut(i1,i2,i3,uc)= -UU(uc)*u0x-UU(vc)*u0y -cFast*rhoi*p0x + clm*(rL0i-rhoi)*p0x \
                    +(mu*rhoi)*(a43*UXX(uc)+UYY(uc)+a13*(UXY(vc))) + gravity(0)
   ut(i1,i2,i3,vc)= -UU(uc)*v0x-UU(vc)*v0y -cFast*rhoi*p0y + clm*(rL0i-rhoi)*p0y \
                    +(mu*rhoi)*(UXX(vc)+a43*UYY(vc)+a13*(UXY(uc))) + gravity(1)

   ut(i1,i2,i3,Tc)= -UU(uc)*UX(tc)-UU(vc)*UY(tc)-gm1*UU(tc)*divu
   ut(i1,i2,i3,pc)= -UU(uc)*p0x   -UU(vc)*p0y   -cFast*gamma*(p0+pressureLevel)*divu + clm*gamma*(pL0-p0)*divu

   phi = a43*( u0x**2 - u0x*v0y +v0y**2 ) +(v0x+u0y)**2

  #Else
   u0x=UX(uc)
   u0y=UY(uc)
   u0z=UZ(uc)
   v0x=UX(vc)
   v0y=UY(vc)
   v0z=UZ(vc)
   w0x=UX(wc)
   w0y=UY(wc)
   w0z=UZ(wc)
   p0x=UX(pc)
   p0y=UY(pc)
   p0z=UZ(pc)
   divu=u0x+v0y+w0z

   ut(i1,i2,i3,rc)= -UU(uc)*UX(rc)-UU(vc)*UY(rc)-UU(wc)*UZ(rc) -UU(rc)*divu + nuRho*ULAP(rc)
   ut(i1,i2,i3,uc)= -UU(uc)*u0x   -UU(vc)*u0y   -UU(wc)*u0z   -cFast*rhoi*p0x + clm*(rL0i-rhoi)*p0x  \
                    +(mu*rhoi)*(a43*UXX(uc)+UYY(uc)+UZZ(uc)+a13*(UXY(vc)+UXZ(wc))) + gravity(0)
   ut(i1,i2,i3,vc)= -UU(uc)*v0x   -UU(vc)*v0y   -UU(wc)*v0z   -cFast*rhoi*p0y + clm*(rL0i-rhoi)*p0y  \
                    +(mu*rhoi)*(UXX(vc)+a43*UYY(vc)+UZZ(vc)+a13*(UXY(uc)+UYZ(wc))) + gravity(1)
   ut(i1,i2,i3,wc)= -UU(uc)*w0x   -UU(vc)*w0y   -UU(wc)*w0z   -cFast*rhoi*p0z + clm*(rL0i-rhoi)*p0z  \
                    +(mu*rhoi)*(UXX(wc)+UYY(wc)+a43*UZZ(wc)+a13*(UXZ(uc)+UYZ(vc))) + gravity(2)

   ut(i1,i2,i3,tc)= -UU(uc)*UX(tc)-UU(vc)*UY(tc)-UU(wc)*UZ(tc)-gm1*UU(tc)*divu
   ut(i1,i2,i3,pc)= -UU(uc)*p0x   -UU(vc)*p0y   -UU(wc)*p0z   -cFast*gamma*(p0+pressureLevel)*divu \
                    + clm*gamma*(pL0-p0)*divu

   phi = a43*( u0x*(u0x-v0y) + v0y*(v0y-w0z)+ (w0z)**2 ) \
	     +   2.*( u0y*v0x+u0z*w0x + v0z*w0y) \
	     + (u0y)**2 + (u0z)**2 + (v0x)**2 + (v0z)**2 + (w0x)**2 + (w0y)**2
  #End

  lapT = (gm1*kThermal)*ULAP(tc)
  ut(i1,i2,i3,tc)= ut(i1,i2,i3,tc) + rhoi*( lapT + (gm1*mu/Rg)*phi )
  ut(i1,i2,i3,pc)= ut(i1,i2,i3,pc) + Rg*lapT +(gm1*mu)*phi

  #If #AXISYMMETRIC == "YES"
   ! -- add on axisymmetric corrections ---
   yy=xy(i1,i2,i3,1)
   if( abs(yy).gt.yEps )then
     ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+nu*( UY(uc)/yy )
     ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+nu*( (UY(vc)-UU(vc)/yy)/yy )
   else
     ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+nu*( UYY(uc) )
     ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+nu*( .5*UYY(vc) )
   end if
  #End

 #Elif #OPTION == "ASFSPAL" 
  ! ASF with Spalart-Allmaras turbulence model
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
 #Elif #OPTION == "ASFKE" 
  ! ASF with k-epsilon turbulence model
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

 #Elif #OPTION == "ASFVP" 

  ! ASF Visco-plastic  *** finish this ****
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

 #If #AXISYMMETRIC == "YES"
  yy=xy(i1,i2,i3,1)
  if( abs(yy).gt.yEps )then
    uti(i1,i2,i3,uc)=uti(i1,i2,i3,uc)+nu*( UY(uc)/yy )
    uti(i1,i2,i3,vc)=uti(i1,i2,i3,vc)+nu*( (UY(vc)-UU(vc)/yy)/yy )
  else
    uti(i1,i2,i3,uc)=uti(i1,i2,i3,uc)+nu*( UYY(uc) )
    uti(i1,i2,i3,vc)=uti(i1,i2,i3,vc)+nu*( .5*UYY(vc) )
  end if
 #End

#End


#endMacro 

c***************************************************************
c  Define the equations for EXPLICIT time stepping
c
c SOLVER: ASF, ASFSPAL, ASFBL, ASFKE
c ORDER: 2,4
c DIM: 2,3
c GRIDTYPE: rectangular, curvilinear
c
c***************************************************************
#beginMacro fillEquations(SOLVER,DIM,ORDER,GRIDTYPE)

defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

if( isAxisymmetric.eq.0 )then

 if( gridIsImplicit.eq.0 )then
  ! explicit

  loopse1($buildEquations(EXPLICIT,SOLVER,NONE,DIM,ORDER,GRIDTYPE,NO))

 else ! gridIsImplicit
  ! ***** implicit *******

  if( implicitOption .eq.computeImplicitTermsSeparately )then
    loopse1($buildEquations(BOTH,SOLVER,NONE,DIM,ORDER,GRIDTYPE,NO))
  else if( implicitOption.eq.doNotComputeImplicitTerms )then
    loopse1($buildEquations(EXPLICIT_ONLY,SOLVER,NONE,DIM,ORDER,GRIDTYPE,NO))
  else
   write(*,*)'asfdt: Unknown implicitOption=',implicitOption
   stop 5
  end if  ! end implicitOption

 end if

else if( isAxisymmetric.eq.1 )then

 #If (#DIM == "2") && (#SOLVER == "ASF")
 ! **** axisymmetric case ****
 ! write(*,'(" ****  asfdt: axisymmetric case yEps=",e10.3," ****")') yEps
 if( gridIsImplicit.eq.0 )then
  ! explicit

  loopse1($buildEquations(EXPLICIT,SOLVER,NONE,DIM,ORDER,GRIDTYPE,YES))

 else ! gridIsImplicit
  ! ***** implicit *******

  if( implicitOption .eq.computeImplicitTermsSeparately )then
    loopse1($buildEquations(BOTH,SOLVER,NONE,DIM,ORDER,GRIDTYPE,YES))
  else if( implicitOption.eq.doNotComputeImplicitTerms )then
    loopse1($buildEquations(EXPLICIT_ONLY,SOLVER,NONE,DIM,ORDER,GRIDTYPE,YES))
  else
   write(*,*)'asfdt: Unknown implicitOption=',implicitOption
   stop 5
  end if  ! end implicitOption

 end if
 #End

else
  stop 88733
end if 

#endMacro


c$$$#beginMacro fillByOrder(SOLVER,DIM,GRIDTYPE)
c$$$if( orderOfAccuracy.eq.2 )then
c$$$ fillEquations(SOLVER,DIM,2,GRIDTYPE)
c$$$else if( orderOfAccuracy.eq.4 )then
c$$$ fillEquations(SOLVER,DIM,4,GRIDTYPE)
c$$$else
c$$$ stop 88
c$$$end if
c$$$#endMacro
c$$$
c$$$#beginMacro fillByDimension(SOLVER,GRIDTYPE)
c$$$if( nd.eq.2 )then
c$$$ fillByOrder(SOLVER,2,GRIDTYPE)
c$$$else if( nd.eq.3 )then
c$$$ fillByOrder(SOLVER,3,GRIDTYPE)
c$$$else
c$$$ stop 99
c$$$end if
c$$$#endMacro

c====================================================================================
c
c SOLVER: ASF, ASFSPAL, ASFBL, ASFKE
c
c====================================================================================
#beginMacro assignEquations(SOLVER,DIM,ORDER)
if( gridType.eq.rectangular )then
 fillEquations(SOLVER,DIM,ORDER,rectangular)
else if( gridType.eq.curvilinear )then
 fillEquations(SOLVER,DIM,ORDER,curvilinear)
else
  stop 77
end if
#endMacro



c================================================================
c  Add on the artificial dissipation
c================================================================
#beginMacro addArtficialDissipation(ADTYPE,DIM,ORDER,GRIDTYPE)

#If #ADTYPE == "AD2" || #ADTYPE == "AD24" || #ADTYPE == "AD4"
#Else
  stop 99
#End

#If #DIM == "2"
 if( turbulenceModel.eq.noTurbulenceModel )then
  defineArtificialDissipationMacro(ADTYPE,DIM,ASF)
  loopse4($getArtificialDissipationCoeff(ADTYPE,DIM,ASF),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),)
 else if( turbulenceModel.eq.spalartAllmaras )then
   stop 111
c  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
c  loopse4($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
c          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
c          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
c          ut(i1,i2,i3,nc)=ut(i1,i2,i3,nc) artificialDissipationTM(nc))
 else if( turbulenceModel.eq.kEpsilon )then
   stop 111
c  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
c  loopse6($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
c          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
c          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
c          ut(i1,i2,i3,kc)=ut(i1,i2,i3,kc) artificialDissipationTM(kc),\
c          ut(i1,i2,i3,ec)=ut(i1,i2,i3,ec) artificialDissipationTM(ec),)
 else
   stop 44
 end if

#Elif #DIM == "3"

 if( turbulenceModel.eq.noTurbulenceModel )then
  defineArtificialDissipationMacro(ADTYPE,DIM,ASF)
  loopse4($getArtificialDissipationCoeff(ADTYPE,DIM,ASF),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc) artificialDissipation(wc))
 else if( turbulenceModel.eq.spalartAllmaras )then
   stop 111
c  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
c  loopse6($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
c          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
c          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
c          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc) artificialDissipation(wc),\
c          ut(i1,i2,i3,nc)=ut(i1,i2,i3,nc) artificialDissipationTM(nc),)
 else if( turbulenceModel.eq.kEpsilon )then
   stop 111
c  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
c  loopse6($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
c          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
c          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
c          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc) artificialDissipation(wc),\
c          ut(i1,i2,i3,kc)=ut(i1,i2,i3,kc) artificialDissipationTM(kc),\
c          ut(i1,i2,i3,ec)=ut(i1,i2,i3,ec) artificialDissipationTM(ec))
 else
   stop 44
 end if


#Else
  stop 1234

#End

#endMacro

#beginMacro addDissipationByADType( DIM,ORDER,GRIDTYPE )

 ! NOTE: we always use second derivatives for the coefficient:
 defineDerivativeMacros(DIM,2,GRIDTYPE)

  if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
    addArtficialDissipation(AD2,DIM,ORDER,GRIDTYPE)
  else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 )then
    addArtficialDissipation(AD4,DIM,ORDER,GRIDTYPE)
  else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 )then
    addArtficialDissipation(AD24,DIM,ORDER,GRIDTYPE)
  else
    stop 7
  end if

#endMacro

#beginMacro addDissipationByOrder( DIM,GRIDTYPE )
  if( orderOfAccuracy.eq.2 )then
    addDissipationByADType( DIM,2,GRIDTYPE )
  else if( orderOfAccuracy.eq.4 )then
    addDissipationByADType( DIM,4,GRIDTYPE )
  else
    stop 66
  end if
#endMacro

#beginMacro addDissipationByDimension( GRIDTYPE )
  if( nd.eq.2 )then
    addDissipationByOrder( 2,GRIDTYPE )
  else if( nd.eq.3 )then
    addDissipationByOrder( 3,GRIDTYPE )
  else
    stop 66
  end if
#endMacro

#beginMacro extractParameters()

#endMacro


c======================================================================================
c Define the subroutine to compute du/dt
c
c SOLVER: ASF, ASFSPAL, ASFBL, ASFKE
c
c======================================================================================
#beginMacro ASFDT(SOLVER,NAME,DIM,ORDER)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
         mask,xy,rsxy,  u,uu, ut,uti, rL, pL,  gv,dw,  bc, ipar, rpar, ierr )
c======================================================================
c   Compute du/dt for the all-speefd NS
c 
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uti(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

 real rL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real pL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

 integer ipar(0:*)
 real rpar(0:*)
 
 !     ---- local variables -----
 integer pdeModel,linearizeImplicitMethod
 real cFast,clm,rL0i,pL0,p0

 integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
 integer gridIsImplicit,implicitOption,implicitMethod,explicitMethod,isAxisymmetric
 integer use2ndOrderAD,use4thOrderAD
 integer rc,pc,uc,vc,wc,sc,nc,kc,ec,tc,grid,m,advectPassiveScalar
 real nu,dt,nuPassiveScalar,adcPassiveScalar
 real ad21,ad22,ad41,ad42,cd22,cd42,adc
 real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
 real yy,yEps

c real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i

 integer gridType
 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

 integer computeAllTerms,\
     doNotComputeImplicitTerms,\
     computeImplicitTermsSeparately,\
     computeAllWithWeightedImplicit

 parameter( computeAllTerms=0,\
           doNotComputeImplicitTerms=1,\
           computeImplicitTermsSeparately=2,\
           computeAllWithWeightedImplicit=3 )

 real rx,ry,rz,sx,sy,sz,tx,ty,tz
 real dr(0:2), dx(0:2)

 real u0x,u0y,u0z
 real v0x,v0y,v0z
 real w0x,w0y,w0z
 real p0x,p0y,p0z
 real rhoi,divu,phi,lapT,gamma,gm1,mu,kThermal,Rg,nuRho,pressureLevel,a43,a13
 real gravity(0:2)

 ! for SPAL TM
 real n0,n0x,n0y,n0z
 real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
 real chi,chi3,fnu1,fnu2,s,r,g,fw,dKappaSq,nSqBydSq,dd
 real nuT,nuTx,nuTy,nuTz,nuTd

 ! for k-epsilon
 real k0,k0x,k0y,k0z, e0,e0x,e0y,e0z
 real nuP,prod
 real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI

 ! for visco-plastic
 real nuVP,etaVP,yieldStressVP,exponentVP,epsVP
 real eDotNorm,exp0
 real u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
 real v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
 real w0xx,w0xy,w0xz,w0yy,w0yz,w0zz

 real delta22,delta23,delta42,delta43

#If #ORDER == "2"
 declareDifferenceOrder2(u,RX)
#End
#If #ORDER == "4"
 declareDifferenceOrder4(u,RX)
#End
 !  --- begin statement functions
 rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
 ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
 rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
 sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
 sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
 sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
 tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
 ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
 tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

c     The next macro call will define the difference approximation statement functions
#If #ORDER == "2"
 defineDifferenceOrder2Components1(u,RX)
#End
#If #ORDER == "4"
 defineDifferenceOrder4Components1(u,RX)
#End

c    --- For 2nd order 2D artificial diffusion ---
 delta22(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
                +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
c    --- For 2nd order 3D artificial diffusion ---
 delta23(c)= \
   (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   \
   +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  \
   +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c)) 
c     ---For fourth-order artificial diffusion in 2D
 delta42(c)= \
   (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
       -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
       +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  \
    -12.*u(i1,i2,i3,c) ) 
c     ---For fourth-order artificial diffusion in 3D
 delta43(c)= \
   (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  \
       -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)  \
       -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  \
   +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  \
       +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)  \
       +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
    -18.*u(i1,i2,i3,c) )

c     --- end statement functions

 ierr=0
 ! write(*,'("Inside asfdt: gridType=",i2)') gridType

 pdeModel           =ipar(0)
 turbulenceModel    =ipar(1)
 orderOfAccuracy    =ipar(2)
 rc                 =ipar(3)
 uc                 =ipar(4)
 vc                 =ipar(5)
 wc                 =ipar(6)
 tc                 =ipar(7) 
 pc                 =ipar(8)
 nc                 =ipar(9)
 sc                 =ipar(10)
 grid               =ipar(11)
 gridIsMoving       =ipar(12)
 useWhereMask       =ipar(13)
 gridIsImplicit     =ipar(14)
 explicitMethod     =ipar(15)
 implicitMethod     =ipar(16)
 implicitOption     =ipar(17)
 isAxisymmetric     =ipar(18)
 use2ndOrderAD      =ipar(19)
 use4thOrderAD      =ipar(20)
 advectPassiveScalar=ipar(21)
 gridType           =ipar(22)
 linearizeImplicitMethod=ipar(23)

 dr(0)             =rpar(0)
 dr(1)             =rpar(1)
 dr(2)             =rpar(2)
 dx(0)             =rpar(3)
 dx(1)             =rpar(4)
 dx(2)             =rpar(5)
 ad21              =rpar(6)
 ad22              =rpar(7)
 ad41              =rpar(8)
 ad42              =rpar(9) 
 nuPassiveScalar   =rpar(10)
 adcPassiveScalar  =rpar(11)
 ad21n             =rpar(12)
 ad22n             =rpar(13)
 ad41n             =rpar(14)
 ad42n             =rpar(15)
 yEps              =rpar(16) ! for axisymmetric
 gravity(0)        =rpar(17)
 gravity(1)        =rpar(18)
 gravity(2)        =rpar(19)
 mu                =rpar(20)
 kThermal          =rpar(21)
 gamma             =rpar(22)
 Rg                =rpar(23)
 nuRho             =rpar(24)
 pressureLevel     =rpar(25)

 gm1=gamma-1.
 a43=4./3.
 a13=1./3. 


 kc=nc
 ec=kc+1

 if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
   write(*,'("asfdt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
   stop 1
 end if
 if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
   write(*,'("asfdt:ERROR gridType=",i6)') gridType
   stop 2
 end if
 if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
   write(*,'("asfdt:ERROR uc,vc,ws=",3i6)') uc,vc,wc
   stop 4
 end if

c      write(*,'("asfdt: turbulenceModel=",2i6)') turbulenceModel
c      write(*,'("asfdt: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc

 if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. kc.gt.1000) )then
   write(*,'("asfdt:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc
   stop 5
 end if

 
 ! cFast: coefficient of the "fast" terms that are set to zero for the implicit all-speed algorithm
 if( explicitMethod.eq.0 )then
  cFast=0.
 else
  cFast=1.
 end if

 ! clm : coefficient of the linearized terms 
 clm=0.
 if( explicitMethod.eq.0 .and. linearizeImplicitMethod.eq.1 )then
   clm=1
 endif

c write(*,'(" asfdt: cFast,clm=",2f4.1)') cFast,clm

c     adc=adcPassiveScalar ! coefficient of linear artificial diffusion
c     cd22=ad22/(nd**2)
c     cd42=ad42/(nd**2)

 if( gridIsMoving.ne.0 )then
   ! compute uu = u -gv
   if( nd.eq.2 )then
     loopse2(uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0),\
             uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1))
   else if( nd.eq.3 )then
     loopse3(uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0),\
             uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1),\
             uu(i1,i2,i3,wc)=u(i1,i2,i3,wc)-gv(i1,i2,i3,2))
   else
     stop 11
   end if
 end if

c write(*,'(" **** asf: mu,kThermal,Rg,gamma,nuRho=",5e9.2," gravity=",3f6.1)') \
c     mu,kThermal,Rg,gamma,nuRho, gravity(0),gravity(1),gravity(2)
c    ! ' 


c ** these are needed by self-adjoint terms **fix**
cdxi=1./dx(0)
cdyi=1./dx(1)
cdzi=1./dx(2)
cdri=1./dr(0)
cdsi=1./dr(1)
cdti=1./dr(2)
cdr2i=1./(2.*dr(0))
cds2i=1./(2.*dr(1))
cdt2i=1./(2.*dr(2))

c if( turbulenceModel.eq.spalartAllmaras )then
c   call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, \
c      cv1e3, cd0, cr0)
c else if( turbulenceModel.eq.kEpsilon )then
c
c  ! write(*,'(" asfdt: k-epsilon: nc,kc,ec=",3i3)') nc,kc,ec
c
c   call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
c   !  write(*,'(" asfdt: cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=",5f8.3)') cMu,cEps1,cEps2,sigmaEpsI,sigmaKI
c
c else if( turbulenceModel.ne.noTurbulenceModel )then
c   stop 88
c end if

 adc=adcPassiveScalar ! coefficient of linear artificial diffusion
 cd22=ad22/(nd**2)
 cd42=ad42/(nd**2)

c     *********************************      
c     ********MAIN LOOPS***************      
c     *********************************      
 assignEquations(SOLVER,DIM,ORDER)

 return
 end
#endMacro

c 
c : empty version for linking when we don't want an option
c
#beginMacro ASFDT_NULL(SOLVER,NAME,DIM,ORDER)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
         mask,xy,rsxy,  u,uu, ut,uti,rL, pL, gv,dw,  bc, ipar, rpar, ierr )
c======================================================================
c       EMPTY VERSION for Linking without this Capability
c
c   Compute du/dt for the incompressible NS on rectangular grids
c     OPTIMIZED version for rectangular grids.
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uti(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real rL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real pL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

 integer ipar(0:*)
 real rpar(0:*)
 return
 end
#endMacro


#beginMacro buildFile(SOLVER,NAME,DIM,ORDER)
#beginFile src/NAME.f
 ASFDT(SOLVER,NAME,DIM,ORDER)
#endFile
c#beginFile NAME ## Null.f
c ASFDT_NULL(SOLVER,NAME,DIM,ORDER)
c#endFile
#endMacro

c Here we create the files
      buildFile(ASF,asfdtASF2dOrder2,2,2)
c      buildFile(ASF,asfdtASF2dOrder4,2,4)
      buildFile(ASF,asfdtASF3dOrder2,3,2)
c      buildFile(ASF,asfdtASF3dOrder4,3,4)

c      buildFile(ASFSPAL,asfdtSPAL2dOrder2,2,2)
c      buildFile(ASFSPAL,asfdtSPAL2dOrder4,2,4)
c      buildFile(ASFSPAL,asfdtSPAL3dOrder2,3,2)
c      buildFile(ASFSPAL,asfdtSPAL3dOrder4,3,4)

c      buildFile(ASFKE,asfdtKE2dOrder2,2,2)
c      buildFile(ASFKE,asfdtKE2dOrder4,2,4)
c      buildFile(ASFKE,asfdtKE3dOrder2,3,2)
c      buildFile(ASFKE,asfdtKE3dOrder4,3,4)

c      ! Visco-plastic case
c      buildFile(ASFVP,asfdtVP2dOrder2,2,2)
c      buildFile(ASFVP,asfdtVP3dOrder2,3,2)

c ====================================================
c SOLVER: ASF, SPAL, KE
c ====================================================
#beginMacro asfdtFunctions(SOLVER)
 if( orderOfAccuracy.eq.2 )then
  if( nd.eq.2 )then
    call asfdt ## SOLVER ## 2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           mask,xy,rsxy,  u,uu, ut,uti,rL,pL, gv,dw,  bc, ipar, rpar, ierr )
  else 
    call asfdt ## SOLVER ## 3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           mask,xy,rsxy,  u,uu, ut,uti,rL,pL, gv,dw,  bc, ipar, rpar, ierr )
  end if
#If #SOLVER ne "VP"
 else if( orderOfAccuracy.eq.4 )then
  stop 555
  if( nd.eq.2 )then
c    call asfdt ## SOLVER ## 2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c           mask,xy,rsxy,  u,uu, ut,uti,rL,pL, gv,dw,  bc, ipar, rpar, ierr )
  else 
c    call asfdt ## SOLVER ## 3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c           mask,xy,rsxy,  u,uu, ut,uti,rL,pL, gv,dw,  bc, ipar, rpar, ierr )
  end if
#End
 else
   stop 1111
 end if
#endMacro


c ==========================================================
c  Advect a passive scalar -- kernel
c ==========================================================
#beginMacro passiveScalarKernel(DIM,ORDER,GRIDTYPE)

 defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then

  #If #DIM == "2"
   #If #ORDER == "2"
    ut(i1,i2,i3,sc)= -UU(uc)*UX(sc)-UU(vc)*UY(sc)+nuPassiveScalar*ULAP(sc)+adcPassiveScalar*delta2 ## DIM(sc)
   #Else
    ut(i1,i2,i3,sc)= -UU(uc)*UX(sc)-UU(vc)*UY(sc)+nuPassiveScalar*ULAP(sc)+adcPassiveScalar*delta4 ## DIM(sc)
   #End
  #Else
   #If #ORDER == "2"
    ut(i1,i2,i3,sc)= -UU(uc)*UX(sc)-UU(vc)*UY(sc)-UU(wc)*UZ(sc)+nuPassiveScalar*ULAP(sc)\
                      +adcPassiveScalar*delta2 ## DIM(sc)
   #Else
    ut(i1,i2,i3,sc)= -UU(uc)*UX(sc)-UU(vc)*UY(sc)-UU(wc)*UZ(sc)+nuPassiveScalar*ULAP(sc)\
                      +adcPassiveScalar*delta4 ## DIM(sc)
   #End
  #End

  end if
 end do
 end do
 end do

#endMacro

c ==============================================================
c  Advect a passive scalar -- build loops for different cases:
c     DIM,ORDER,GRIDTYPE
c ==============================================================
#beginMacro passiveScalarMacro()

 if( gridType.eq.rectangular )then
  if( orderOfAccuracy.eq.2 )then
    if( nd.eq.2 )then
      passiveScalarKernel(2,2,rectangular)
    else 
      passiveScalarKernel(3,2,rectangular)
    end if
  else if( orderOfAccuracy.eq.4 )then
    if( nd.eq.2 )then
      passiveScalarKernel(2,4,rectangular)
    else 
      passiveScalarKernel(3,4,rectangular)
    end if
  else
   stop 1281
  end if

 else if( gridType.eq.curvilinear )then

  if( orderOfAccuracy.eq.2 )then
    if( nd.eq.2 )then
      passiveScalarKernel(2,2,curvilinear)
    else 
      passiveScalarKernel(3,2,curvilinear)
    end if
  else if( orderOfAccuracy.eq.4 )then
    if( nd.eq.2 )then
      passiveScalarKernel(2,4,curvilinear)
    else 
      passiveScalarKernel(3,4,curvilinear)
    end if
  else
   stop 1282
  end if

 else
   stop 1717
 end if

#endMacro


      subroutine asfdt(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                       mask,xy,rsxy,  u,uu, ut,uti,rL, pL, gv,dw,  bc, ipar, rpar, ierr )
c======================================================================
c   Compute du/dt for the all-speed compressible NS
c 
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uti(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real pL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)
      
c     ---- local variables -----
      integer orderOfAccuracy

c     integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
c     integer gridIsImplicit,implicitOption,implicitMethod,
c    & isAxisymmetric,use2ndOrderAD,use4thOrderAD
c     integer pc,uc,vc,wc,sc,nc,kc,ec,tc,grid,m,advectPassiveScalar
c     real nu,dt,nuPassiveScalar,adcPassiveScalar
c     real gravity(0:2), thermalExpansivity, adcBoussinesq,kThermal
c     real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
c     real ad21,ad22,ad41,ad42,cd22,cd42,adc
c     real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
c     real yy,yEps

      integer gridType
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      integer pdeModel,BoussinesqModel,viscoPlasticModel
      parameter( BoussinesqModel=1,viscoPlasticModel=2 )

c      real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
c      real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
c      real udmzC,udzmC,udmzzC,udzmzC,udzzmC
c      real admzR,adzmR,admzzR,adzmzR,adzzmR
c      real admzC,adzmC,admzzC,adzmzC,adzzmC
c      real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
c      real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
c      real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f

c     real delta22,delta23,delta42,delta43

c     real adCoeff2,adCoeff4

c     real ad2,ad23,ad4,ad43
c     real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
c     real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

c     real rx,ry,rz,sx,sy,sz,tx,ty,tz
c     real dr(0:2), dx(0:2)

      ! for SPAL TM
c      real n0,n0x,n0y,n0z
c      real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
c      real chi,chi3,fnu1,fnu2,s,r,g,fw,dKappaSq,nSqBydSq,dd
c      real nuT,nuTx,nuTy,nuTz,nuTd

      ! for k-epsilon
c      real k0,k0x,k0y,k0z, e0,e0x,e0y,e0z
c      real nuP,prod
c      real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI


      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'

c     declareDifferenceOrder2(u,RX)
c     declareDifferenceOrder4(u,RX)

c     --- begin statement functions
c     rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
c     ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
c     rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
c     sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
c     sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
c     sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
c     tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
c     ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
c     tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

c     The next macro call will define the difference approximation statement functions
c     defineDifferenceOrder2Components1(u,RX)
c     defineDifferenceOrder4Components1(u,RX)


c    --- 2nd order 2D artificial diffusion ---
c     ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
c    &           +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

c    --- 2nd order 3D artificial diffusion ---
c     ad23(c)=adc
c    &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
c    &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) 
c    &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                       
c     ---fourth-order artificial diffusion in 2D
c     ad4(c)=adc
c    &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    
c    &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    
c    &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    
c    &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   
c    &      -12.*u(i1,i2,i3,c) ) 
c     ---fourth-order artificial diffusion in 3D
c     ad43(c)=adc
c    &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   
c    &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   
c    &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   
c    &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
c    &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   
c    &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  
c    &      -18.*u(i1,i2,i3,c) )

c    --- For 2nd order 2D artificial diffusion ---
c     delta22(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
c                  +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
c    --- For 2nd order 3D artificial diffusion ---
c     delta23(c)= \
c       (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   \
c       +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  \
c       +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c)) 
c     ---For fourth-order artificial diffusion in 2D
c     delta42(c)= \
c       (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
c           -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
c       +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
c           +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  \
c        -12.*u(i1,i2,i3,c) ) 
c     ---For fourth-order artificial diffusion in 3D
c     delta43(c)= \
c       (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  \
c           -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)  \
c           -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  \
c       +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  \
c           +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)  \
c           +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
c        -18.*u(i1,i2,i3,c) )


c #Include "selfAdjointArtificialDiffusion.h"

c     --- end statement functions

      ierr=0
      ! write(*,'("Inside asfdt: gridType=",i2)') gridType

      pdeModel           =ipar(0)
      turbulenceModel    =ipar(1) 
      orderOfAccuracy    =ipar(2)


      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("asfdt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if

c     *********************************      
c     ********MAIN LOOPS***************      
c     *********************************      

      if( turbulenceModel.eq.noTurbulenceModel .and. pdeModel.eq.viscoPlasticModel )then
        ! asf + visco-plastic model
c**        asfdtFunctions(VP)
        stop 123

      else if( turbulenceModel.eq.noTurbulenceModel )then

        asfdtFunctions(ASF)

      else if( turbulenceModel.eq.spalartAllmaras )then

c**        asfdtFunctions(SPAL)
        stop 456

      else if( turbulenceModel.eq.kEpsilon )then

c**        asfdtFunctions(KE)
        stop 789

      else
        write(*,'("Unknown turbulence model")') 
        stop 68
      end if


c     *********************************
c     ******** passive scalar *********
c     *********************************

c*      if( advectPassiveScalar.eq.1 )then
c*        passiveScalarMacro()
c*      end if


c     **********************************
c     ****** artificial diffusion ******  
c     **********************************

c*      if( use2ndOrderAD.eq.1 .or. use4thOrderAD.eq.1 )then
c*        if( gridType.eq.rectangular )then
c*          addDissipationByDimension( rectangular )
c*        else if( gridType.eq.curvilinear )then
c*          addDissipationByDimension( curvilinear )
c*        else
c*          stop 77
c*        end if
c*      end if


      return
      end


