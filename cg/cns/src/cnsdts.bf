c
c Compute the time step for the compressible NS on rectangular AND curvilinear grids
c
c ----------- this file started from insdts.bf --------------------
c 


c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


#beginMacro beginLoopsWithMask(n1a,n1b,n2a,n2b,n3a,n3b)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoopsWithMask()
  end if
 end do
 end do
 end do
#endMacro

#beginMacro beginLoops(n1a,n1b,n2a,n2b,n3a,n3b)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
 end do
 end do
 end do
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



c ============== from inspf.bf ***
c Return nuT and it's first derivatives for SPAL
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

c ============== from inspf.bf ***
c Return nuT and it's first derivatives for BL
#beginMacro getBaldwinLomaxEddyViscosityAndFirstDerivatives(DIM)

  nuT = nu+u(i1,i2,i3,nc)

  nuTx=UX(nc)
  nuTy=UY(nc)

  #If #DIM == "3" 
    nuTz=UZ(nc)
  #End
#endMacro

c ============== from inspf.bf ***
c Return nuT and it's first derivatives for KE
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

c ============== from inspf.bf ***
#beginMacro getEddyViscosityAndFirstDerivatives(SOLVER,DIM)

 #If #SOLVER == "CNSSPAL"
   setupSpalartAllmaras(DIM)
 #Elif #SOLVER == "CNSBL"
   getBaldwinLomaxEddyViscosityAndFirstDerivatives(DIM)
 #Elif #SOLVER == "CNSKE"
   getKEpsilonViscosityAndFirstDerivatives(DIM)
 #Else
   write(*,'("cnsdts:ERROR: unknown solver= SOLVER ")')
   stop 987 
 #End

#endMacro

c====================================================================================
c
c SOLVER: CNS, CNSSPAL, CNSBL, CNSKE
c METHOD: GLOBAL, LOCAL  (GLOBAL=fixed time step, LOCAL=local-time stepping -> compute dtVar)
c OPTION: EXPLICIT, IMPLICIT
c ADTYPE: AD2, AD4, AD24 --- NOT USED NOW
c ORDER: 2,4
c DIM: 2,3
c GRIDTYPE: rectangular, curvilinear
c AXISYMMETRIC: notAxisymmetric, or axisymmetric
c====================================================================================
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

imLambda=0.
reLambda=0.

dtVarMin=1.e22  ! we need a REAL_MAX for fortran
dtVarMax=0.


if( av2.ne.0. .or. av4.ne.0. )then
  ! Evaluate the pressure switch for the Jameson style artificial dissipation

  ! evaluate on a bigger box since we need dp and neighbours
  do axis=0,nd-1
    is(0)=0
    is(1)=0
    is(2)=0
    is(axis)=1
    m1a=max(n1a-1,nd1a+is(0))
    m1b=min(n1b+1,nd1b-is(0))
    m2a=max(n2a-1,nd2a+is(1))
    m2b=min(n2b+1,nd2b-is(1))
    if( nd.eq.3 )then
      m3a=max(n3a-1,nd3a+is(2))
      m3b=min(n3b+1,nd3b-is(2))
    else
      m3a=n3a
      m3b=n3b
    end if

    beginLoops(m1a,m1b,m2a,m2b,m3a,m3b)
     if( mask(i1,i2,i3).gt.0 )then
      dp(i1,i2,i3,axis)=abs( ( p(i1+is(0),i2+is(1),i3+is(2))-2.*p(i1,i2,i3)+p(i1-is(0),i2-is(1),i3-is(2)) )/ \
	                     ( p(i1+is(0),i2+is(1),i3+is(2))+2.*p(i1,i2,i3)+p(i1-is(0),i2-is(1),i3-is(2)) ) )
      else
        dp(i1,i2,i3,axis)=0.
      end if
    endLoops()
  end do
end if



c ...............................................
beginLoopsWithMask(n1a,n1b,n2a,n2b,n3a,n3b)

#If #SOLVER == "CNSSPAL" || #SOLVER == "CNSBL" || #SOLVER == "CNSKE" 
  getEddyViscosityAndFirstDerivatives(SOLVER,DIM)
#End

! speed of sound squared:
 cSq = gamma*Rg*U(tc)

#If #GRIDTYPE == "rectangular"

  #If #DIM == "2" 
    imPart= cr*( abs(UU(uc))/dx(0)+abs(UU(vc))/dx(1) ) +sqrt( cSq*( 1./dx(0)**2+1./dx(1)**2 ) )
  #Else
    imPart= cr*( abs(UU(uc))/dx(0) + abs(UU(vc))/dx(1) + abs(UU(wc))/dx(2) )\
            +sqrt( cSq*( 1./dx(0)**2 +1./dx(1)**2 +1./dx(2)**2 ) )
  #End

  #If #SOLVER == "CNSSPAL" 
    #If #DIM == "2"
      imPart= imPart+ cr*(1.+cb2)*sigmai*(abs(NuTx)/dx(0)+abs(nuTy)/dx(1) )
    #Else
      imPart= imPart+ cr*(1.+cb2)*sigmai*( abs(NuTx)/dx(0)+abs(nuTy)/dx(1)+abs(nuTz)/dx(2) )
    #End
  #Elif #SOLVER == "CNSBL" || #SOLVER == "CNSKE"
    ! check this 
    #If #DIM == "2"
      imPart= imPart+ cr*( abs(NuTx)/dx(0)+abs(nuTy)/dx(1) )
    #Else
      imPart= imPart+ cr*( abs(NuTx)/dx(0)+abs(nuTy)/dx(1)+abs(nuTz)/dx(2) )
    #End
  #End

  #If #OPTION == "EXPLICIT"
   #If #SOLVER == "CNS" 
    #If #DIM == "2"
      rePart= crr*(mukt/U(rc))*( 1./dx(0)**2 + 1./dx(1)**2 )
    #Else
      rePart= crr*(mukt/U(rc))*( 1./dx(0)**2 + 1./dx(1)**2 + 1./dx(2)**2 )
    #End
    #If #AXISYMMETRIC == "axisymmetric"
      yy=yc(i2)
      if( abs(yy).ge.dx(1) )then
        imPart=imPart + cr*( 1./(yy*dx(1)) )  ! u.y/y 
      else
        rePart=rePart + crr*nu*( 1./dx(1)**2 )   ! u.yy
      end if
    #Elif #AXISYMMETRIC == "notAxisymmetric"
    #Else
      stop 77542
    #End

   #Elif #SOLVER == "CNSSPAL" || #SOLVER == "CNSBL" || #SOLVER == "CNSKE" 
     ! "
     #If #SOLVER == "CNSSPAL"
       nuMax= max( nu+nuT, sigmai*(nu+n0) )
     #Elif #SOLVER == "CNSKE" 
       nuMax= max( nu+nuT, nu+sigmaKI*nuT, nu+sigmaEpsI*nuT )
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
    #If #SOLVER == "CNSSPAL" 
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


  if( av2.ne.0. .or. av4.ne.0. )then
    ! Jameson style artificial dissipation

    do axis=0,nd-1
      is(0)=0
      is(1)=0
      is(2)=0
      is(axis)=1

      cc = sqrt( gamma*(p(i1,i2,i3)/U(rc)) )   ! speed of sound

      alam= ( abs(U(uc+axis))+cc )/dx(axis)

      wmax= max(dp(i1-is(0),i2-is(1),i3-is(2),axis),max(dp(i1,i2,i3,axis),dp(i1+is(0),i2+is(1),i3+is(2),axis)))
      w2 = av2*alam*min(1.0,wmax/aw2)
      w4=av4*alam*max(0.,1.0-wmax/aw4)
      rePart= rePart + (4.*w2+16.*w4)

    end do
  end if


#Elif #GRIDTYPE == "curvilinear"

  #If #DIM == "2"
    a1   = UU(uc)*rx(i1,i2,i3)+UU(vc)*ry(i1,i2,i3)
    a2   = UU(uc)*sx(i1,i2,i3)+UU(vc)*sy(i1,i2,i3)
  #Else
    a1   = UU(uc)*rx(i1,i2,i3)+UU(vc)*ry(i1,i2,i3)+UU(wc)*rz(i1,i2,i3)
    a2   = UU(uc)*sx(i1,i2,i3)+UU(vc)*sy(i1,i2,i3)+UU(wc)*sz(i1,i2,i3)
    a3   = UU(uc)*tx(i1,i2,i3)+UU(vc)*ty(i1,i2,i3)+UU(wc)*tz(i1,i2,i3)
  #End

  #If #SOLVER == "CNSSPAL" 
    #If #DIM == "2"
      a1=a1 - (1.+cb2)*sigmai*(nuTx*rx(i1,i2,i3)+nuTy*ry(i1,i2,i3))
      a2=a2 - (1.+cb2)*sigmai*(nuTx*sx(i1,i2,i3)+nuTy*sy(i1,i2,i3))
    #Else
      a1=a1 - (1.+cb2)*sigmai*(nuTx*rx(i1,i2,i3)+nuTy*ry(i1,i2,i3)+nuTz*rz(i1,i2,i3))
      a2=a2 - (1.+cb2)*sigmai*(nuTx*sx(i1,i2,i3)+nuTy*sy(i1,i2,i3)+nuTz*sz(i1,i2,i3))
      a3=a3 - (1.+cb2)*sigmai*(nuTx*tx(i1,i2,i3)+nuTy*ty(i1,i2,i3)+nuTz*tz(i1,i2,i3))
    #End
  #Elif #SOLVER == "CNSBL" || #SOLVER == "CNSKE"
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
   #If #SOLVER == "CNS" 
    ! constant nu case
    #If #DIM == "2"
      a1=a1 -nu*( RXX() + RYY() )
      a2=a2 -nu*( SXX() + SYY() )
      rePart = (mukt/U(rc))*( \
                 ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(0))) \
             +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3) )*( cr/(dr(0)*dr(1))) \
                +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )

     #If #AXISYMMETRIC == "axisymmetric"
      yy=xy(i1,i2,i3,1)
      if( abs(yy).gt.yEps )then
        ! u.y/y 
        a1 = a1 -nu*( ry(i1,i2,i3)/yy ) 
        a2 = a2 -nu*( sy(i1,i2,i3)/yy ) 
      else
        ! u.yy 
        rePart= rePart+nu*(  \
                 (                            ry(i1,i2,i3)**2          )*(crr/(dr(0)*dr(0))) \
             +abs(                           ry(i1,i2,i3)*sy(i1,i2,i3) )*( cr/(dr(0)*dr(1))) \
                +(                            sy(i1,i2,i3)**2          )*(crr/(dr(1)*dr(1))) )
      end if
     #Elif #AXISYMMETRIC == "notAxisymmetric"
     #Else
       stop 77542
     #End
    #Else
      muktbr=mukt/U(rc)
      a1=a1 -muktbr*( RXX() + RYY() + RZZ())
      a2=a2 -muktbr*( SXX() + SYY() + SZZ() )
      a3=a3 -muktbr*( TXX() + TYY() + TZZ() )

      rePart = muktbr*( \
                 ( rx(i1,i2,i3)**2          + ry(i1,i2,i3)**2         + rz(i1,i2,i3)**2 )*(crr/(dr(0)*dr(0))) \
                +( sx(i1,i2,i3)**2          + sy(i1,i2,i3)**2         + sz(i1,i2,i3)**2 )*(crr/(dr(1)*dr(1)))  \
                +( tx(i1,i2,i3)**2          + ty(i1,i2,i3)**2         + tz(i1,i2,i3)**2 )*(crr/(dr(2)*dr(2)))  \
        +abs( rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3) )*(cr/(dr(0)*dr(1))) \
        +abs( rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(0)*dr(2))) \
        +abs( sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3) )*(cr/(dr(1)*dr(2))) \
                )

    #End
   #Elif #SOLVER == "CNSSPAL" || #SOLVER == "CNSBL" || #SOLVER == "CNSKE" 
     ! "
     #If #SOLVER == "CNSSPAL"
       nuMax= max( nu+nuT, sigmai*(nu+n0) )
     #Elif #SOLVER == "CNSKE" 
       nuMax= max( nu+nuT, nu+sigmaKI*nuT, nu+sigmaEpsI*nuT )
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
    #If #SOLVER == "CNSSPAL" 
      ! rePart=rePart+ max(0., cb1*s - .5*cw1*fw*n0/dd**2)
      ! add a factor of 2
      ! rePart=rePart+ 2.*cb1*s
      rePart=rePart+ max(0., 2.*cb1*s - .5*cw1*fw*n0/dd**2)
    #End

  #End

  #If #DIM == "2"
    imPart=cr*(abs(a1)/dr(0)+abs(a2)/dr(1)) \
           +sqrt( cSq*( rx(i1,i2,i3)**2 *(1./(dr(0)**2)) \
	               +sy(i1,i2,i3)**2 *(1./(dr(1)**2)) ) )
  #Else
    imPart = cr*( abs(a1)/dr(0)+abs(a2)/dr(1)+abs(a3)/dr(2) ) \
           +sqrt( cSq*( rx(i1,i2,i3)**2 *(1./(dr(0)**2)) \
	               +sy(i1,i2,i3)**2 *(1./(dr(1)**2)) \
	               +tz(i1,i2,i3)**2 *(1./(dr(2)**2)) ) )
  #End


  if( av2.ne.0. .or. av4.ne.0. )then
    ! Jameson style artificial dissipation

    do axis=0,nd-1
      is(0)=0
      is(1)=0
      is(2)=0
      is(axis)=1

      #If #DIM == "2" 
        ! aj = | dx/dr | (centerJacobian)
        aj = rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)  !  (dx(0)/dr(0))*(dx(1)/dr(1))
        aj = 1./max(ajEps,abs(aj))
        a1= rsxy(i1,i2,i3,axis,0)*aj 
        a2= rsxy(i1,i2,i3,axis,1)*aj 
        dist = sqrt(a1**2+a2**2)
        vn = a1*U(uc)+a2*U(vc)
      #Else
        aj = (rx(i1,i2,i3)*sy(i1,i2,i3)-sx(i1,i2,i3)*ry(i1,i2,i3) )*tz(i1,i2,i3)+\
             (sx(i1,i2,i3)*ty(i1,i2,i3)-tx(i1,i2,i3)*sy(i1,i2,i3) )*rz(i1,i2,i3)+\
             (tx(i1,i2,i3)*ry(i1,i2,i3)-rx(i1,i2,i3)*ty(i1,i2,i3) )*sz(i1,i2,i3) 
        aj = 1./max(ajEps,abs(aj))
        a1= rsxy(i1,i2,i3,axis,0)*aj 
        a2= rsxy(i1,i2,i3,axis,1)*aj 
        a3= rsxy(i1,i2,i3,axis,2)*aj
        dist = sqrt(a1**2+a2**2+a3**2)
        vn = a1*U(uc)+a2*U(vc)+a3*U(wc)
      #End
    
      cc = sqrt( gamma*(p(i1,i2,i3)/U(rc)) ) 

      alam= (abs(vn)+cc*dist)/dr(axis)
      wmax= max(dp(i1-is(0),i2-is(1),i3-is(2),axis),max(dp(i1,i2,i3,axis),dp(i1+is(0),i2+is(1),i3+is(2),axis)))
      w2 = av2*alam*min(1.0,wmax/aw2)
      w4=av4*alam*max(0.,1.0-wmax/aw4)
      rePart= rePart + (4.*w2+16.*w4)/aj

    end do
  end if

! end curvilinear:
#End

#If #OPTION == "EXPLICIT"
  if( use2ndOrderAD.eq.1 )then 
    #If #DIM == "2"
      #If #SOLVER == "CNSSPAL"
        adCNS = 8.*( ad21 + cd22*( abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)) ) )
        adSPAL= 8.*( ad21n + cd22n*( abs(UX(nc))+abs(UY(nc)) ) )
        rePart=rePart + max( adCNS, adSPAL )
      #Else
        rePart=rePart + 8.*( ad21 + cd22*( abs(UX(uc))+abs(UX(vc))+abs(UY(uc))+abs(UY(vc)) ) )
      #End
    #Else
      #If #SOLVER == "CNSSPAL"
        adCNS = 12.*( ad21 + cd22*( \
         abs(UX(uc))+abs(UX(vc))+abs(UX(wc))+abs(UY(uc))+abs(UY(vc))+abs(UY(wc))+abs(UZ(uc))+abs(UZ(vc))+abs(UZ(wc))) )
        adSPAL = 12.*( ad21n + cd22n*( abs(UX(nc))+abs(UY(nc))+abs(UZ(nc)) ) )
        rePart=rePart + max( adCNS, adSPAL )
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


endLoopsWithMask()

#endMacro

c ====================================================================================
c ====================================================================================
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
  ! No axis-symmetric versions yet
  ! kkc 051115 there is now !
  #If #SOLVER == "CNS"
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
  ! no fourth order versions yet   
  ! getTimeSteppingEigenvaluesByDimension(SOLVER,METHOD,OPTION,ADTYPE,4)
    stop 321
 else
   stop 123
 end if
#endMacro


#beginMacro getTimeSteppingEigenvaluesByArtificialDissipation(SOLVER,METHOD,OPTION)
c Don't split by ADTYPE since this makes the file too long for no big benefit.
c if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,NONE)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then 
c  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD2)
c else if( use2ndOrderAD.eq.0 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD4)
c else if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.1 )then 
c  getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD24)
c else
c   stop 123
c end if
 getTimeSteppingEigenvaluesByOrder(SOLVER,METHOD,OPTION,AD24) 
#endMacro

#beginMacro getTimeSteppingEigenvaluesByImplicit(SOLVER,METHOD) 
 if( gridIsImplicit.eq.0 )then
  getTimeSteppingEigenvaluesByArtificialDissipation(SOLVER,METHOD,EXPLICIT)
 else
  !kkc 060228 activated this line, not sure what differs yet
  getTimeSteppingEigenvaluesByArtificialDissipation(SOLVER,METHOD,IMPLICIT)
!  stop 634
 end if
#endMacro

#beginMacro getTimeSteppingEigenvaluesByMethod(SOLVER) 
 if( useLocalTimeStepping.eq.0 )then
  getTimeSteppingEigenvaluesByImplicit(SOLVER,GLOBAL)
 else
   getTimeSteppingEigenvaluesByImplicit(SOLVER,LOCAL)
!kkc  stop 916
 end if
#endMacro

! This is not finished yet..
#beginMacro computeAxisymmetricCorrection(DIM,ORDER,GRIDTYPE)
defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
beginLoopsWithMask(n1a,n1b,n2a,n2b,n3a,n3b)
c y corresponds to the radial direction
c y=0 is the axis of symmetry
c   nu*(  u.xx + u.yy + (1/y) u.y )
c   nu*(  v.xx + v.yy + (1/y) v.y - v/y^2 ) 
 #If #GRIDTYPE == "rectangular"
   radiusInverse=1./max(REAL_MIN,YY(i2))
 #Else
   radiusInverse=1./max(REAL_MIN,vertex(i1,i2,i3,1))
 #End 
 endLoopsWithMask()
 urOverR(i1,i2,i3)=UY(uc)*radiusInverse
 vrOverR(i1,i2,i3)=(UY(vc)-U(vc)*radiusInverse)*radiusInverse
 do axis=0,nd-1
 do side=0,1
  if( boundaryCondition(side,axis).eq.axisymmetric )then
    getBoundaryIndex(mg.gridIndexRange(),side,axis)
    beginLoopsWithMask(n1a,n1b,n2a,n2b,n3a,n3b)
      urOverR(i1,i2,i3)=UYY(uc)
      vrOverR(i1,i2,i3)=.5*UYY(vc)
    endLoopsWithMask()
  end if
 end do
 end do
#endMacro


c ============================================================================================================
c  Define the subroutine that compute the time stepping eigenvalues for a given solver
c ============================================================================================================
#beginMacro CNSDTS(SOLVER,NAME)
 subroutine NAME(nd, n1a,n1b,n2a,n2b,n3a,n3b, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
     mask,xy, rsxy,  u,uu, gv,dw, p, dp, dtVar, bc, ipar, rpar, ierr )
c======================================================================
c
c    Determine the time step for the CNS equations.
c    ---------------------------------------------
c
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c p : pressure
c dp : work space for Jameson dissipation
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real p(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)    ! pressure
 real dp(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)  ! work-space for Jameson dissipation
 real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

 integer ipar(0:*)
 real rpar(0:*)
 
 !   ---- local variables -----
 integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
 integer gridIsImplicit,implicitOption,implicitMethod,isAxisymmetric
 integer use2ndOrderAD,use4thOrderAD,useLocalTimeStepping
 integer rc,tc,pc,uc,vc,wc,sc,nc,kc,ec,grid,m,advectPassiveScalar
 real nu,dt,nuPassiveScalar,adcPassiveScalar
 real dtVarMin,dtVarMax,dtMax,dtMaxInverse
 real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
 real ad21,ad22,ad41,ad42,cd22,cd42,adc
 real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
 real adCNS,adSPAL
 real scaleFactor,factor,cDt,cdv,cr,crr
 integer i1a,i2a,i3a
 real yy,yEps,xa,ya,za

 real av2,aw2,av4,aw4,aj,dist,vm,alam,w2,w4,wmax,cc
 integer axis,axisp1,is(0:2)
 integer m1a,m1b,m2a,m2b,m3a,m3b

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


 real rePart,imPart,imLambda,reLambda,a1,a2,a3,nuMax,cSq,gamma,Rg,ajEps,vn,mu,kThermal,mukt,muktbr

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


c Include "selfAdjointArtificialDiffusion.h"

 !     --- end statement functions

 ierr=0

 rc                 =ipar( 0)
 uc                 =ipar( 1)
 vc                 =ipar( 2)
 wc                 =ipar( 3)
 tc                 =ipar( 4)
 nc                 =ipar( 5)
 sc                 =ipar( 6)


 grid                =ipar( 7)
 orderOfAccuracy     =ipar( 8)
 gridIsMoving        =ipar( 9)
 useWhereMask        =ipar(10)
 gridIsImplicit      =ipar(11)
 implicitMethod      =ipar(12)
 implicitOption      =ipar(13)
 isAxisymmetric      =ipar(14)
 use2ndOrderAD       =ipar(15)
 use4thOrderAD       =ipar(16)
 advectPassiveScalar =ipar(17)
 gridType            =ipar(18)
 turbulenceModel     =ipar(19)
 useLocalTimeStepping=ipar(20)
 i1a                 =ipar(21)
 i2a                 =ipar(22)
 i3a                 =ipar(23)

 dr(0)             =rpar( 0)
 dr(1)             =rpar( 1)
 dr(2)             =rpar( 2)
 dx(0)             =rpar( 3)
 dx(1)             =rpar( 4)
 dx(2)             =rpar( 5)

 ad21              =rpar( 6)
 ad22              =rpar( 7)
 ad41              =rpar( 8)
 ad42              =rpar( 9)
 nuPassiveScalar   =rpar(10)
 adcPassiveScalar  =rpar(11)
 dtMax             =rpar(12)
 !     reLambda          =rpar(13) ! returned here
 !     imLambda          =rpar(14) ! returned here
 ad21n             =rpar(15)
 ad22n             =rpar(16)
 ad41n             =rpar(17)
 ad42n             =rpar(18)
 xa                =rpar(19)
 ya                =rpar(20)
 za                =rpar(21)
 yEps              =rpar(22) ! for axisymmetric y<yEps => y is on the axis
 
 av2               =rpar(23)
 aw2               =rpar(24)
 av4               =rpar(25)
 aw4               =rpar(26)
 gamma             =rpar(27)
 Rg                =rpar(28)
 mu                =rpar(29)
 kThermal          =rpar(30)
 ajEps             =rpar(31) ! for minimum value of the jacobian

 kc=nc
 ec=kc+1

 mukt = max(4./3.*mu,(gamma-1.)*kThermal)

c write(*,'("cnsdts: gridType,gridIsImplicit,implicitMethod,implicitOption,useLocalTimeStepping=",10i3)') gridType,gridIsImplicit,implicitMethod,implicitOption,useLocalTimeStepping

c write(*,'("cnsdts: n1a,n1b,n2a,n2b,n3a,n3b=",6i4)') n1a,n1b,n2a,n2b,n3a,n3b
c write(*,'("cnsdts: av2,aw2,av4,aw4,mu,kThermal=",10f6.3)') av2,aw2,av4,aw4,mu,kThermal

 if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
   write(*,'("cnsdts:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
   stop 1
 end if
 if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
   write(*,'("cnsdts:ERROR gridType=",i6)') gridType
   stop 2
 end if
 if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
   write(*,'("cnsdts:ERROR uc,vc,ws=",3i6)') uc,vc,wc
   stop 4
 end if
 if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. kc.gt.1000) )then
   write(*,'("cnsdts:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc
   stop 5
 end if
 if( nd.ne.2 .and. nd.ne.3 )then
   write(*,'("cnsdts:ERROR nd=",i6)') nd
   stop 1
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

  ! write(*,'(" cnsdts: k-epsilon: nc,kc,ec=",3i3)') nc,kc,ec

   call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
   !  write(*,'(" cnsdts: cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=",5f8.3)') cMu,cEps1,cEps2,sigmaEpsI,sigmaKI

 else if( turbulenceModel.ne.noTurbulenceModel )then
   stop 88
 end if

 adc=adcPassiveScalar ! coefficient of linear artificial diffusion
 cd22=ad22/(nd**2)
 cd42=ad42/(nd**2)
 cd22n=ad22/nd
 cd42n=ad42/nd
 dtMaxInverse=1./dtMax

 !     correction factors for divergence damping term
 !     this is an over estimate ****
ckkc 070921 no div damping in cns if( cdv.eq.0. )then
ckkc 070921 no div damping in cns    scaleFactor=0.
ckkc 070921 no div damping in cns  else
ckkc 070921 no div damping in cns    scaleFactor = 1.
ckkc 070921 no div damping in cns    if( isAxisymmetric.eq.1 )then
ckkc 070921 no div damping in cns      scaleFactor=2.
ckkc 070921 no div damping in cns    end if
ckkc 070921 no div damping in cns  end if
ckkc 070921 no div damping in cns  factor=1.5*scaleFactor


 if( gridIsMoving.ne.0 )then
   ! compute uu = u -gv
   if( nd.eq.2 )then
     beginLoopsWithMask(n1a,n1b,n2a,n2b,n3a,n3b)
       uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
       uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
     endLoopsWithMask()
   else if( nd.eq.3 )then
     beginLoopsWithMask(n1a,n1b,n2a,n2b,n3a,n3b)
      uu(i1,i2,i3,uc)=u(i1,i2,i3,uc)-gv(i1,i2,i3,0)
      uu(i1,i2,i3,vc)=u(i1,i2,i3,vc)-gv(i1,i2,i3,1)
      uu(i1,i2,i3,wc)=u(i1,i2,i3,wc)-gv(i1,i2,i3,2)
    endLoopsWithMask()
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

 if( useLocalTimeStepping.eq.1 )then
   write(*,'(" cnsdts: local dt, grid=",i3," dtVar (min,max)=(",e10.2,",",e10.2,")")') \
    grid,dtVarMin,dtVarMax
   ! '
 end if

 rpar(13)=reLambda
 rpar(14)=imLambda

 return
 end

#endMacro


      subroutine cnsdts(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
        mask,xy,rsxy,  u,uu, gv,dw, p, dp, dtVar, bc, ipar, rpar, ierr )
c======================================================================
c
c    Determine the time step for the CNS equations.
c    ---------------------------------------------
c
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c p : pressure
c dp : work space for Jameson dissipation
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real p(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)    ! pressure
      real dp(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)  ! work-space for Jameson dissipation
      real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)
      
c     ---- local variables -----


      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )


c     --- end statement functions

      ierr=0
      ! write(*,'("Inside cnsdts: gridType=",i2)') gridType


      turbulenceModel    =ipar(19)


c     *****************************************************
c     ********DETERMINE THE TIME STEPPING EIGENVALUES *****
c     *****************************************************      

      if( turbulenceModel.eq.noTurbulenceModel )then

        call cnsdtsCNS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          mask,xy,rsxy,  u,uu, gv,dw, p, dp, dtVar, bc, ipar, rpar, ierr )

      else if( turbulenceModel.eq.spalartAllmaras )then

c       call cnsdtsSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c         mask,xy,rsxy,  u,uu, gv,dw, p, dp, dtVar, bc, ipar, rpar, ierr )

      else if( turbulenceModel.eq.baldwinLomax )then

c       call cnsdtsBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c         mask,xy,rsxy,  u,uu, gv,dw, p, dp, dtVar, bc, ipar, rpar, ierr )

      else if( turbulenceModel.eq.kEpsilon )then

c       call cnsdtsKE(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c         mask,xy,rsxy,  u,uu, gv,dw, p, dp, dtVar, bc, ipar, rpar, ierr )

      else 
        stop 33
      end if


      return
      end


#beginMacro buildFile(SOLVER,NAME)
#beginFile src/NAME.f
 CNSDTS(SOLVER,NAME)
#endFile
#endMacro


      buildFile(CNS,cnsdtsCNS)
c     buildFile(CNSSPAL,cnsdtsSPAL)
c     buildFile(CNSBL,cnsdtsBL)
c     buildFile(CNSKE,cnsdtsKE)
      
