! --- Macros for the Spalart-Alamras model for the line solver ----
! this file is includes by insLineSOlveNew.bf 


c Define the turbulent eddy viscosity and its derivatives given chi3=chi^3 
#beginMacro defineSADerivatives(dim,gt)
  nuT = nu+u(i1,i2,i3,nc)*chi3/(chi3+cv1e3)
  nuTd= chi3*(chi3+4.*cv1e3)/(chi3+cv1e3)**2
  #If #gt == "rectangular"
    nuTx(0)=ux2(nc)*nuTd
    nuTx(1)=uy2(nc)*nuTd
    #If #dim == "3" 
      nuTx(2)=uz2(nc)*nuTd
    #End
  #Else
    #If #dim == "2" 
      nuTx(0)=ux2c(nc)*nuTd
      nuTx(1)=uy2c(nc)*nuTd
    #Else
      nuTx(0)=ux3c(nc)*nuTd
      nuTx(1)=uy3c(nc)*nuTd
      nuTx(2)=uz3c(nc)*nuTd
    #End
  #End
#endMacro


c Define the turbulent eddy viscosity and its derivatives  
#beginMacro defineValuesSA(dim,gt)
  chi3 = (u(i1,i2,i3,nc)/nu)**3
  ! chi3=0.
  defineSADerivatives(dim,gt)
#endMacro



c Macro to define the set of computations required to compute values for the SA turbulence model.
c used in the macros below
#beginMacro setupSA(dim,gt)
 chi=uu(nc)/nu
 chi3=chi**3
 fnu1=chi3/( chi3+cv1e3)
 fnu2=1.-chi/(1.+chi*fnu1)
 dd = dw(i1,i2,i3)+cd0  
 dKappaSq=(dd*kappa)**2
#If #dim == "2"
  #If #gt == "rectangular"
    s=abs(uy2(uc)-ux2(vc))+ uu(nc)*fnu2/dKappaSq ! turbulence source term 
  #Else
    s=abs(uy2c(uc)-ux2c(vc))+ uu(nc)*fnu2/dKappaSq ! turbulence source term 
  #End
#Else
  #If #gt == "rectangular"
    s=uu(nc)*fnu2/dKappaSq \
     +sqrt( (uy2(uc)-ux2(vc))**2 + (uz2(vc)-uy2(wc))**2 + (ux2(wc)-uz2(uc))**2 )
  #Else
    s=uu(nc)*fnu2/dKappaSq \
     +sqrt( (uy3c(uc)-ux3c(vc))**2 + (uz3c(vc)-uy3c(wc))**2 + (ux3c(wc)-uz3c(uc))**2 )
  #End
#End
 r= min( uu(nc)/( s*dKappaSq ), cr0 )   !  r= uu(nc)/( max( s*dKappaSq, 1.e-20) )
 g=r+cw2*(r**6-r)
 fw=g*( (1.+cw3e6)/(g**6+cw3e6) )**(1./6.)
 ! We use Newton to linearize the quadratic term: y*y -> 2*y*y0 - y0**2
 nSqBydSq=cw1*fw*(uu(nc)/dd)**2     ! for rhs
 nBydSqLhs=2.*cw1*fw*(uu(nc)/dd**2) ! for lhs
 nutb=sigmai*(nu+uu(nc))
#If #dim == "2"
  #If #gt == "rectangular"
   dndx(0)=ux2(nc)
   dndx(1)=uy2(nc)
  #Else
   dndx(0)=ux2c(nc)
   dndx(1)=uy2c(nc)
  #End 
#Else
  #If #gt == "rectangular"
   dndx(0)=ux2(nc)
   dndx(1)=uy2(nc)
   dndx(2)=uz2(nc)
  #Else
   dndx(0)=ux3c(nc)
   dndx(1)=uy3c(nc)
   dndx(2)=uz3c(nc)
  #End 
#End
#endMacro

c Macro for the SA TM on rectangular grids
c Only the equation for the turbulence eddy viscosity is done here
#beginMacro fillSAEquationsRectangularGrid(dir)
if( nd.eq.2 )then
  triLoops(2,SA,\
           $defineArtificialDiffusionCoefficients(2,dir,R,SA),\
           $setupSA(2,rectangular),\
           t1=-(1.+cb2)*sigmai*dndx(dir),\
           am(i1,i2,i3)= -(uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nutb*(dxvsqi(0)+dxvsqi(1)) +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  (uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdp,,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsa2d ## dir(nc) + nSqBydSq + adE ##dir(i1,i2,i3,nc),,)
else ! 3d
  triLoops(3,SA,\
           $defineArtificialDiffusionCoefficients(3,dir,R,SA),\
           $setupSA(3,rectangular),\
           t1=-(1.+cb2)*sigmai*dndx(dir),\
           am(i1,i2,i3)= -(uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nutb*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  (uu(uc+dir)+t1)*dxv2i(dir)-nutb*dxvsqi(dir) -cdp,,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsa3d ## dir(nc) + nSqBydSq +adE3d ##dir(i1,i2,i3,nc),,)
end if
#endMacro


#beginMacro fillSAEquationsCurvilinearGrid(dir)
dirp1=mod(dir+1,nd)
dirp2=mod(dir+2,nd)
if( nd.eq.2 )then
  triLoops(2,SA,\
           $defineArtificialDiffusionCoefficients(2,dir,C,SA),\
           $setupSA(2,curvilinear),\
           t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nutb*(rxx(dir,0)+rxy(dir,1))\
                  -(1.+cb2)*sigmai*(dndx(0)*rxi(dir,0)+dndx(1)*rxi(dir,1)) )*drv2i(dir),\
           t2=nutb*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir),\
           am(i1,i2,i3)= -t1-t2 -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*(t2+nutb*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )\
                           +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  t1-t2 -cdp,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsac2d ## dir(nc) + nSqBydSq +adE ##dir(i1,i2,i3,nc),,)
else ! 3d
  triLoops(3,SA,\
           $defineArtificialDiffusionCoefficients(3,dir,C,SA),\
           $setupSA(3,curvilinear),\
           t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)\
                     -nutb*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)) \
                     -(1.+cb2)*sigmai*(dndx(0)*rxi(dir,0)+dndx(1)*rxi(dir,1)+dndx(2)*rxi(dir,2)) )*drv2i(dir),\
           t2=nutb*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir), \
           am(i1,i2,i3)= -t1-t2 -cdm,\
           bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)\
               +2.*(t2+nutb*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                              (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) ))\
                           +cdDiag +nBydSqLhs - cb1*s,\
           cm(i1,i2,i3)=  t1-t2 -cdp,,,,\
           f(i1,i2,i3,nc)=f(i1,i2,i3,nc)+fsac3d ## dir(nc) + nSqBydSq +adE3d ##dir(i1,i2,i3,nc),,)
end if
#endMacro
