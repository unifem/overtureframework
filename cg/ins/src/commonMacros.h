c **********************************************************************
c  This file contains some commonly used macros.
c **********************************************************************


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


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================
#beginMacro defineArtificialDissipationMacro(ADTYPE,DIM,TURBULENCE_MODEL)

 ! By default there is no AD:
 #defineMacro artificialDissipation(cc) 
 #defineMacro artificialDissipationTM(cc) 

 ! 2nd-order artificial dissipation
 #If #ADTYPE == "AD2"
   #If #DIM == "2"
     #defineMacro artificialDissipation(cc)  +adCoeff2*delta22(cc)
   #Else
     #defineMacro artificialDissipation(cc)  +adCoeff2*delta23(cc)
   #End

 #End

 ! 4th-order artficial dissipation  **todo** implicit-line, self-adjoint versions
 #If #ADTYPE == "AD4" || #ADTYPE == "AD24"
   #If #DIM == "2"
     #defineMacro artificialDissipation(cc)  +adCoeff4*delta42(cc)
   #Else
     #defineMacro artificialDissipation(cc)  +adCoeff4*delta43(cc)
   #End
 #End

 ! Both 2nd and 4th order dissipation
 #If #ADTYPE == "AD24"
   #If #DIM == "2"
     #defineMacro artificialDissipation(cc)  +adCoeff2*delta22(cc) + adCoeff4*delta42(cc)
   #Else
     #defineMacro artificialDissipation(cc)  +adCoeff2*delta23(cc) + adCoeff4*delta43(cc)
   #End
 #End

 #If #TURBULENCE_MODEL == "SPAL"
  ! Define the ad macro for the turbulent eddy viscosity equation
  ! Just base the coefficient on the derivatives of that component.
  #If #ADTYPE == "AD2" 
    #If #DIM == "2"
     #defineMacro artificialDissipationTM(cc)  +( ad21n+cd22n*( abs(UX(cc))+abs(UY(cc)) ) )*delta22(cc)
    #Else
     #defineMacro artificialDissipationTM(cc)  +( ad21n+cd22n*( abs(UX(cc))+abs(UY(cc))+abs(UZ(cc)) ) )*delta23(cc)
    #End
  #Elif #ADTYPE == "AD4" 
    #If #DIM == "2"
     #defineMacro artificialDissipationTM(cc)  +(  ad41n+cd42n*( abs(UX(cc))+abs(UY(cc)) ) )*delta42(cc)
    #Else
     #defineMacro artificialDissipationTM(cc)  +( ad41n+cd42n*( abs(UX(nc))+abs(UY(nc))+abs(UZ(nc)) ) )*delta43(cc)
    #End
  #Elif #ADTYPE == "AD24"
    #If #DIM == "2"
     #defineMacro artificialDissipationTM(cc)  +( ad21n+cd22n*( abs(UX(cc))+abs(UY(cc)) ) )*delta22(cc)\
                                               +(  ad41n+cd42n*( abs(UX(cc))+abs(UY(cc)) ) )*delta42(cc)
    #Else
     #defineMacro artificialDissipationTM(cc)  +( ad21n+cd22n*( abs(UX(cc))+abs(UY(cc))+abs(UZ(cc)) ) )*delta23(cc)\
                                               +( ad41n+cd42n*( abs(UX(nc))+abs(UY(nc))+abs(UZ(nc)) ) )*delta43(cc)
    #End
  #Else
    stop 4444
  #End

 #End
#endMacro


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================
#beginMacro getArtificialDissipationCoeff(ADTYPE,DIM,TURBULENCE_MODEL)

 #If #ADTYPE == "AD2" || #ADTYPE == "AD24"
   #If #DIM == "2"
     adCoeff2 = ad21+cd22*( abs(UX(uc))+abs(UY(uc))+abs(UX(vc))+abs(UY(vc)) )
   #Else
     adCoeff2 = ad21+cd22*( abs(UX(uc))+abs(UY(uc))+abs(UZ(uc))+\
                            abs(UX(vc))+abs(UY(vc))+abs(UZ(vc))+\
                            abs(UX(wc))+abs(UY(wc))+abs(UZ(wc)))
   #End

 #End

 ! Artficial Dissipation  **todo** implicit-line, self-adjoint versions
 #If #ADTYPE == "AD4" || #ADTYPE == "AD24"
   #If #DIM == "2"
     adCoeff4 = ad41+cd42*( abs(UX(uc))+abs(UY(uc))+abs(UX(vc))+abs(UY(vc)) )
   #Else
     adCoeff4 = ad41+cd42*( abs(UX(uc))+abs(UY(uc))+abs(UZ(uc))+\
                            abs(UX(vc))+abs(UY(vc))+abs(UZ(vc))+\
                            abs(UX(wc))+abs(UY(wc))+abs(UZ(wc)))
   #End
 #End

#endMacro


