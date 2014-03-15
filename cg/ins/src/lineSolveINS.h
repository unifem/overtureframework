! 
! --- Macros to define INS and Temperature line-solve functions:---
!          fillEquationsRectangularGridINS
!          fillEquationsCurvilinearGridINS
!          fillEquationsRectangularGridTemperature
!          fillEquationsCurvilinearGridTemperature
!          computeResidualINS

!     This file is included in insLineSolveNew.bf 


c ===================================================================================
c        INS: Incompressible Navier Stokes 
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsRectangularGridINS(dir)

 !  ****** Incompressible NS, No turbulence model *****
 if( use4thOrderAD.eq.1 )then
  write(*,*) 'insLineSolve: 4th order diss not finished'
  stop 7654
 end if

 ! set default values for no 2nd order artificial diffusion: 
 cdm=0.
 cdDiag=0.
 cdp=0.

! INS - RHS forcing for rectangular grids, directions=0,1,2 (do NOT include grad(p) terms, 
!  since then macro is valid for m=uc,vc,wc)
#If #dir == "0"
 #defineMacro fr2d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+nu*uyy0(m) \
                         -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
 #defineMacro fr3d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+nu*(uyy0(m)+uzz0(m))\
                        -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
#Elif #dir == "1"
 #defineMacro fr2d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+nu*uxx0(m) \
                       -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
 #defineMacro fr3d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+nu*(uxx0(m)+uzz0(m)) -\
                         thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
#Else
 #defineMacro fr2d(m) (0.)
 #defineMacro fr3d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+nu*(uxx0(m)+uyy0(m)) \
                         -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
#End

if( nd.eq.2 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
  defineDerivativeMacros(2,2,rectangular)
  beginLoops()
   if( mask(i1,i2,i3).gt.0 )then
    if( use2ndOrderAD.eq.1 )then
      defineArtificialDiffusionCoefficients(2,dir,R,)
    end if
    if( computeMatrix.eq.1 )then
      am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdm 
      bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*nu*(dxvsqi(0)+dxvsqi(1)) +cdDiag
      cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdp
    end if
    if( computeRHS.eq.1 )then
      f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr2d(uc)-ux2(pc)
      f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr2d(vc)-uy2(pc)
      if( use2ndOrderAD.eq.1 )then
        f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+ adE ## dir(i1,i2,i3,uc)
        f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+ adE ## dir(i1,i2,i3,vc)
      end if
    end if
   else
    if( computeMatrix.eq.1 )then ! for interpolation points or unused:
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.
    end if
    if( computeRHS.eq.1 )then
     f(i1,i2,i3,fcu)=uu(uc)
     f(i1,i2,i3,fcv)=uu(vc)
    end if
   end if
  endLoops()

else if( nd.eq.3 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
  defineDerivativeMacros(3,2,rectangular)

  beginLoops()
   if( mask(i1,i2,i3).gt.0 )then
    if( use2ndOrderAD.eq.1 )then
      defineArtificialDiffusionCoefficients(3,dir,R,)
    end if
    if( computeMatrix.eq.1 )then
     am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdm
     bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*nu*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +cdDiag
     cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-nu*dxvsqi(dir) -cdp
    end if
    if( computeRHS.eq.1 )then
     f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fr3d(uc)-ux2(pc)
     f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fr3d(vc)-uy2(pc)
     f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fr3d(wc)-uz2(pc)
     if( use2ndOrderAD.eq.1 )then
      f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+adE3d ## dir(i1,i2,i3,uc)
      f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+adE3d ## dir(i1,i2,i3,vc)
      f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+adE3d ## dir(i1,i2,i3,wc)
     end if
    end if
   else
    if( computeMatrix.eq.1 )then ! for interpolation points or unused:
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.
    end if
    if( computeRHS.eq.1 )then
     f(i1,i2,i3,fcu)=uu(uc)
     f(i1,i2,i3,fcv)=uu(vc)
     f(i1,i2,i3,fcw)=uu(wc)
    end if
   end if
  endLoops()

else
  stop 888 ! unexpected value for nd
end if
                     
#endMacro


c ===================================================================================
c  ****** Incompressible NS, No turbulence model *****
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsCurvilinearGridINS(dir)

 if( use4thOrderAD.eq.1 )then
  write(*,*) 'insLineSolve: 4th order diss not finished'
  stop 7655
 end if

 dirp1=mod(dir+1,nd)
 dirp2=mod(dir+2,nd)

 ! INS - RHS forcing for curvilinear grids, directions=0,1,2  (do NOT include grad(p) terms)
#If #dir == "0"
 #defineMacro fc2d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+nu*(rxx(1,0)+ryy(1,1)))*us(m)+\
  nu*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
 #defineMacro fc3d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nu*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
#Elif #dir == "1"
 #defineMacro fc2d(m) (uu(m)*dtScale/dt(i1,i2,i3) +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+nu*(rxx(0,0)+ryy(0,1)))*ur(m)+\
  nu*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
 #defineMacro fc3d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+nu*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  nu*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
#Else
 #defineMacro fc2d(m)  (0.)
 #defineMacro fc3d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+nu*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+nu*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  nu*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) -thermalExpansivity*gravity(m-uc)*u(i1,i2,i3,tc))
#End

 ! set default values for no 2nd order artificial diffusion:
 cdm=0.
 cdDiag=0.
 cdp=0.

if( nd.eq.2 )then
 defineDerivativeMacros(2,2,curvilinear)

 beginLoops()
  if( mask(i1,i2,i3).gt.0 )then
   if( use2ndOrderAD.eq.1 )then
     defineArtificialDiffusionCoefficients(2,dir,C,)
   end if
   if( computeMatrix.eq.1 )then
    t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-nu*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir)
    t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir)
    am(i1,i2,i3)= -t1-t2 -cdm
    bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+nu*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )+cdDiag
    cm(i1,i2,i3)=  t1-t2 -cdp
   end if
   if( computeRHS.eq.1 )then
    f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc2d(uc)-ux2c(pc)
    f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc2d(vc)-uy2c(pc)
    if( use2ndOrderAD.eq.1 )then
     f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+adE ## dir(i1,i2,i3,uc)
     f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+adE ## dir(i1,i2,i3,vc)
    end if
   end if
  else ! for interpolation points or unused:
   if( computeMatrix.eq.1 )then
    am(i1,i2,i3)=0.
    bm(i1,i2,i3)=1.
    cm(i1,i2,i3)=0.
   end if
   if( computeRHS.eq.1 )then
    f(i1,i2,i3,fcu)=uu(uc)
    f(i1,i2,i3,fcv)=uu(vc)
   end if
  end if
 endLoops()

else if( nd.eq.3 )then

 defineDerivativeMacros(3,2,curvilinear)

 beginLoops()
  if( mask(i1,i2,i3).gt.0 )then 
   if( use2ndOrderAD.eq.1 )then
    defineArtificialDiffusionCoefficients(3,dir,C,)
   end if
   if( computeMatrix.eq.1 )then
    t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)-nu*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)))*drv2i(dir)
    t2=nu*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir)
    am(i1,i2,i3)= -t1-t2 -cdm
    bm(i1,i2,i3)= dtScale/dt(i1,i2,i3)\
      +2.*(t2+nu*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                   (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )) +cdDiag
    cm(i1,i2,i3)=  t1-t2 -cdp
   end if
   if( computeRHS.eq.1 )then
     f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+fc3d(uc)-ux3c(pc)
     f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+fc3d(vc)-uy3c(pc)
     f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+fc3d(wc)-uz3c(pc)
     if( use2ndOrderAD.eq.1 )then
      f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu)+adE3d ## dir(i1,i2,i3,uc)
      f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv)+adE3d ## dir(i1,i2,i3,vc)
      f(i1,i2,i3,fcw)=f(i1,i2,i3,fcw)+adE3d ## dir(i1,i2,i3,wc)
     end if
   end if
  else  ! for interpolation points or unused:
   if( computeMatrix.eq.1 )then
    am(i1,i2,i3)=0.
    bm(i1,i2,i3)=1.
    cm(i1,i2,i3)=0.
   end if
   if( computeRHS.eq.1 )then
    f(i1,i2,i3,fcu)=uu(uc)
    f(i1,i2,i3,fcv)=uu(vc)
    f(i1,i2,i3,fcw)=uu(wc)
   end if
  end if
 endLoops()

else
  stop 222 ! unexpected value for nd 
end if


#endMacro


c ===================================================================================
c        INS: Incompressible Navier Stokes Temperature Equation
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsRectangularGridTemperature(dir)

 ! write(*,*) 'new: fillEquationsRectangularGridTemperature'

 !  ****** Temperature Equation for INS *****
 if( use4thOrderAD.eq.1 )then
  write(*,*) 'insLineSolve: T : 4th order diss not finished'
  stop 7654
 end if

 ! set default values for no 2nd order artificial diffusion: 
 cdm=0.
 cdDiag=0.
 cdp=0.

#If #dir == "0"
 #defineMacro frt2d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)+kThermal*uyy0(m))
 #defineMacro frt3d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(vc)*uy2(m)-uu(wc)*uz2(m)+kThermal*(uyy0(m)+uzz0(m)))
#Elif #dir == "1"
 #defineMacro frt2d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)+kThermal*uxx0(m))
 #defineMacro frt3d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(wc)*uz2(m)+kThermal*(uxx0(m)+uzz0(m)))
#Else
 #defineMacro frt2d(m) (0.)
 #defineMacro frt3d(m) (uu(m)*dtScale/dt(i1,i2,i3) -uu(uc)*ux2(m)-uu(vc)*uy2(m)+kThermal*(uxx0(m)+uyy0(m)))
#End

if( nd.eq.2 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
  defineDerivativeMacros(2,2,rectangular)
  beginLoops()
   if( mask(i1,i2,i3).gt.0 )then
    if( use2ndOrderAD.eq.1 )then
      defineArtificialDiffusionCoefficients(2,dir,R,)
    end if
    if( computeMatrix.eq.1 )then
      am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir) -cdm 
      bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3)  +2.*kThermal*(dxvsqi(0)+dxvsqi(1)) +cdDiag
      cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir) -cdp
    end if
    if( computeRHS.eq.1 )then
      f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+frt2d(tc)
      if( use2ndOrderAD.eq.1 )then
        f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+ adE ## dir(i1,i2,i3,tc)
      end if
    end if
   else
    if( computeMatrix.eq.1 )then ! for interpolation points or unused:
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.
    end if
    if( computeRHS.eq.1 )then
     f(i1,i2,i3,fct)=uu(tc)
    end if
   end if
  endLoops()

else if( nd.eq.3 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE)
  defineDerivativeMacros(3,2,rectangular)

  beginLoops()
   if( mask(i1,i2,i3).gt.0 )then
    if( use2ndOrderAD.eq.1 )then
      defineArtificialDiffusionCoefficients(3,dir,R,)
    end if
    if( computeMatrix.eq.1 )then
     am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir) -cdm
     bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +2.*kThermal*(dxvsqi(0)+dxvsqi(1)+dxvsqi(2)) +cdDiag
     cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir)-kThermal*dxvsqi(dir) -cdp
    end if
    if( computeRHS.eq.1 )then
     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+frt3d(tc)
     if( use2ndOrderAD.eq.1 )then
      f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+adE3d ## dir(i1,i2,i3,tc)
     end if
    end if
   else
    if( computeMatrix.eq.1 )then ! for interpolation points or unused:
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.
    end if
    if( computeRHS.eq.1 )then
     f(i1,i2,i3,fct)=uu(tc)
    end if
   end if
  endLoops()

else
  stop 888 ! unexpected value for nd
end if
                     
#endMacro


c ===================================================================================
c  ****** Temperature Equation for INS *****
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsCurvilinearGridTemperature(dir)

 ! write(*,*) 'new: fillEquationsCurvilinearGridTemperature'

 if( use4thOrderAD.eq.1 )then
  write(*,*) 'insLineSolve: T : 4th order diss not finished'
  stop 7655
 end if

 dirp1=mod(dir+1,nd)
 dirp2=mod(dir+2,nd)

#If #dir == "0"
 #defineMacro fct2d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)+kThermal*(rxx(1,0)+ryy(1,1)))*us(m)+\
  kThermal*((rxi(1,0)**2+rxi(1,1)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) )
 #defineMacro fct3d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  kThermal*( (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) )
#Elif #dir == "1"
 #defineMacro fct2d(m) (uu(m)*dtScale/dt(i1,i2,i3) +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)+kThermal*(rxx(0,0)+ryy(0,1)))*ur(m)+\
  kThermal*((rxi(0,0)**2+rxi(0,1)**2)*urr0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1))*urs(m)) )
 #defineMacro fct3d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(2,0)-uu(vc)*rxi(2,1)-uu(wc)*rxi(2,2)+kThermal*(rxx3(2,0)+rxy3(2,1)+rxz3(2,2)))*ut(m)+\
  kThermal*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(2,0)**2+rxi(2,1)**2+rxi(2,2)**2)*utt0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) )
#Else
 #defineMacro fct2d(m)  (0.)
 #defineMacro fct3d(m) (uu(m)*dtScale/dt(i1,i2,i3)  +\
  (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1)-uu(wc)*rxi(0,2)+kThermal*(rxx3(0,0)+rxy3(0,1)+rxz3(0,2)))*ur(m)+\
  (-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1)-uu(wc)*rxi(1,2)+kThermal*(rxx3(1,0)+rxy3(1,1)+rxz3(1,2)))*us(m)+\
  kThermal*( (rxi(0,0)**2+rxi(0,1)**2+rxi(0,2)**2)*urr0(m)+\
       (rxi(1,0)**2+rxi(1,1)**2+rxi(1,2)**2)*uss0(m)+\
       2.*(rxi(0,0)*rxi(1,0)+rxi(0,1)*rxi(1,1)+rxi(0,2)*rxi(1,2))*urs(m)+\
       2.*(rxi(0,0)*rxi(2,0)+rxi(0,1)*rxi(2,1)+rxi(0,2)*rxi(2,2))*urt(m)+\
       2.*(rxi(1,0)*rxi(2,0)+rxi(1,1)*rxi(2,1)+rxi(1,2)*rxi(2,2))*ust(m)\
     ) )
#End

 ! set default values for no 2nd order artificial diffusion:
 cdm=0.
 cdDiag=0.
 cdp=0.

if( nd.eq.2 )then
 defineDerivativeMacros(2,2,curvilinear)

 beginLoops()
  if( mask(i1,i2,i3).gt.0 )then
   if( use2ndOrderAD.eq.1 )then
     defineArtificialDiffusionCoefficients(2,dir,C,)
   end if
   if( computeMatrix.eq.1 )then
    t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)-kThermal*(rxx(dir,0)+rxy(dir,1)))*drv2i(dir)
    t2=kThermal*(rxi(dir,0)**2+rxi(dir,1)**2)*drvsqi(dir)
    am(i1,i2,i3)= -t1-t2 -cdm
    bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +2.*(t2+kThermal*(rxi(dirp1,0)**2+rxi(dirp1,1)**2)*drvsqi(dirp1) )+cdDiag
    cm(i1,i2,i3)=  t1-t2 -cdp
   end if
   if( computeRHS.eq.1 )then
    f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+fct2d(tc)
    if( use2ndOrderAD.eq.1 )then
     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+adE ## dir(i1,i2,i3,tc)
    end if
   end if
  else ! for interpolation points or unused:
   if( computeMatrix.eq.1 )then
    am(i1,i2,i3)=0.
    bm(i1,i2,i3)=1.
    cm(i1,i2,i3)=0.
   end if
   if( computeRHS.eq.1 )then
    f(i1,i2,i3,fct)=uu(tc)
   end if
  end if
 endLoops()

else if( nd.eq.3 )then

 defineDerivativeMacros(3,2,curvilinear)

 beginLoops()
  if( mask(i1,i2,i3).gt.0 )then 
   if( use2ndOrderAD.eq.1 )then
    defineArtificialDiffusionCoefficients(3,dir,C,)
   end if
   if( computeMatrix.eq.1 )then
    t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1)+uu(wc)*rxi(dir,2)-kThermal*(rxx3(dir,0)+rxy3(dir,1)+rxz3(dir,2)))*drv2i(dir)
    t2=kThermal*(rxi(dir,0)**2+rxi(dir,1)**2+rxi(dir,2)**2)*drvsqi(dir)
    am(i1,i2,i3)= -t1-t2 -cdm
    bm(i1,i2,i3)= dtScale/dt(i1,i2,i3)\
      +2.*(t2+kThermal*( (rxi(dirp1,0)**2+rxi(dirp1,1)**2+rxi(dirp1,2)**2)*drvsqi(dirp1)+\
                   (rxi(dirp2,0)**2+rxi(dirp2,1)**2+rxi(dirp2,2)**2)*drvsqi(dirp2) )) +cdDiag
    cm(i1,i2,i3)=  t1-t2 -cdp
   end if
   if( computeRHS.eq.1 )then
     f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+fct3d(tc)
     if( use2ndOrderAD.eq.1 )then
      f(i1,i2,i3,fct)=f(i1,i2,i3,fct)+adE3d ## dir(i1,i2,i3,tc)
     end if
   end if
  else  ! for interpolation points or unused:
   if( computeMatrix.eq.1 )then
    am(i1,i2,i3)=0.
    bm(i1,i2,i3)=1.
    cm(i1,i2,i3)=0.
   end if
   if( computeRHS.eq.1 )then
    f(i1,i2,i3,fct)=uu(tc)
   end if
  end if
 endLoops()

else
  stop 222 ! unexpected value for nd 
end if


#endMacro



c==================================================================================
c  Residual Computation for INS: incompressible Navier Stokes (including Boussinesq)
c
c Macro args:
c  GRIDTYPE: rectangular, curvilinear
c====================================================================================
#beginMacro computeResidualINS(GRIDTYPE)

 ! write(*,*) 'new: computeResidualINS'

 if( nd.eq.2 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
  ! * defineDerivativeMacros(DIM,2,GRIDTYPE)


  beginLoops()
  if( mask(i1,i2,i3).gt.0 )then

   #If #GRIDTYPE == "rectangular"
     ! ************ INS 2d rectangular case  ********************

    ! u.t + u.grad(u) + p.x = nu Delta(u)
    ! v.t + u.grad(v) + p.y = nu Delta(v)
    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)-u(i1,i2,i3,vc)*uy2(uc)-ux2(pc)+nu*lap2d2(uc)\
                          -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) 

    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)-u(i1,i2,i3,vc)*uy2(vc)-uy2(pc)+nu*lap2d2(vc)\
                          -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) 

    if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+adSelfAdjoint2dR(i1,i2,i3,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+adSelfAdjoint2dR(i1,i2,i3,vc)
    else if( use4thOrderAD.eq.1 )then
     ! compute adc2, adc4: 
     getDerivativesAndDissipation(2,2,GRIDTYPE)
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+ad2(adc2,uc)+ad4(adc4,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+ad2(adc2,vc)+ad4(adc4,vc)
    end if

    if( computeTemperature.ne.0 )then
      residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2(tc)-u(i1,i2,i3,vc)*uy2(tc)+kThermal*lap2d2(tc)
      ! --- artificial dissipation for T: do this for now: 
      if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
        residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+adSelfAdjoint2dR(i1,i2,i3,tc)
      else if( use4thOrderAD.eq.1 )then
       residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad2(adc2,tc)+ad4(adc4,tc)
     end if
    end if

   #Else
     ! ************ INS 2d curvilinear case  ********************

    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2c(uc)-u(i1,i2,i3,vc)*uy2c(uc)-ux2c(pc)+nu*lap2d2c(uc)\
                          -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc)
    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(vc)-u(i1,i2,i3,vc)*uy2c(vc)-uy2c(pc)+nu*lap2d2c(vc)\
                          -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc)

    if(  use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+adSelfAdjoint2dC(i1,i2,i3,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+adSelfAdjoint2dC(i1,i2,i3,vc)
    else if( use4thOrderAD.eq.1 )then
     ! compute adc2, adc4: 
     getDerivativesAndDissipation(2,2,GRIDTYPE)
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+ad2(adc2,uc)+ad4(adc4,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+ad2(adc2,vc)+ad4(adc4,vc)
    end if

    if( computeTemperature.ne.0 )then
      residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2c(tc)-u(i1,i2,i3,vc)*uy2c(tc)+kThermal*lap2d2c(tc)
      ! --- artificial dissipation for T: do this for now: 
      if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
        residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+adSelfAdjoint2dC(i1,i2,i3,tc)
      else if( use4thOrderAD.eq.1 )then
       residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad2(adc2,tc)+ad4(adc4,tc)
     end if
    end if

   #End

  end if
  endLoops()

 else if( nd.eq.3 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
  ! defineDerivativeMacros(DIM,2,GRIDTYPE)


  beginLoops()
  if( mask(i1,i2,i3).gt.0 )then

   #If #GRIDTYPE == "rectangular"

     ! ************ INS 3d rectangular case  ********************

    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)\
                                        -u(i1,i2,i3,vc)*uy2(uc)\
                                        -u(i1,i2,i3,wc)*uz2(uc)\
                                        -ux2(pc)+nu*lap3d2(uc)\
                                        -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc)
    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)\
                                        -u(i1,i2,i3,vc)*uy2(vc)\
                                        -u(i1,i2,i3,wc)*uz2(vc)\
                                        -uy2(pc)+nu*lap3d2(vc)\
                                        -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc)
    residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux2(wc)\
                                        -u(i1,i2,i3,vc)*uy2(wc)\
                                        -u(i1,i2,i3,wc)*uz2(wc)\
                                        -uz2(pc)+nu*lap3d2(wc)\
                                        -thermalExpansivity*gravity(2)*u(i1,i2,i3,tc)
    if(  use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+adSelfAdjoint3dR(i1,i2,i3,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+adSelfAdjoint3dR(i1,i2,i3,vc)
     residual(i1,i2,i3,wc)=residual(i1,i2,i3,wc)+adSelfAdjoint3dR(i1,i2,i3,wc)
    else if( use4thOrderAD.eq.1 )then
     ! compute adc2, adc4: 
     getDerivativesAndDissipation(3,2,GRIDTYPE)
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+ad23(adc2,uc)+ad43(adc4,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+ad23(adc2,vc)+ad43(adc4,vc)
     residual(i1,i2,i3,wc)=residual(i1,i2,i3,wc)+ad23(adc2,vc)+ad43(adc4,wc)
    end if


    if( computeTemperature.ne.0 )then
     residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2(tc)\
                                         -u(i1,i2,i3,vc)*uy2(tc)\
                                         -u(i1,i2,i3,wc)*uz2(tc)\
                                         +kThermal*lap3d2(tc)
      ! --- artificial dissipation for T: do this for now: 
      if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
        residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+adSelfAdjoint3dR(i1,i2,i3,tc)
      else if( use4thOrderAD.eq.1 )then
       residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad23(adc2,tc)+ad43(adc4,tc)
     end if
    end if

   #Else
     ! ************ INS 3d curvilinear case  ********************


    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux3c(uc)\
                                        -u(i1,i2,i3,vc)*uy3c(uc)\
                                        -u(i1,i2,i3,wc)*uz3c(uc)\
                                        -ux3c(pc)+nu*lap3d2c(uc)\
	                                -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc)
    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux3c(vc)\
                                        -u(i1,i2,i3,vc)*uy3c(vc)\
                                        -u(i1,i2,i3,wc)*uz3c(vc)\
                                        -uy3c(pc)+nu*lap3d2c(vc)\
                                        -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc)
    residual(i1,i2,i3,wc)=f(i1,i2,i3,wc)-u(i1,i2,i3,uc)*ux3c(wc)\
                                        -u(i1,i2,i3,vc)*uy3c(wc)\
                                        -u(i1,i2,i3,wc)*uz3c(wc)\
                                        -uz3c(pc)+nu*lap3d2c(wc)\
                                        -thermalExpansivity*gravity(2)*u(i1,i2,i3,tc)
    if(  use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+adSelfAdjoint3dC(i1,i2,i3,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+adSelfAdjoint3dC(i1,i2,i3,vc)
     residual(i1,i2,i3,wc)=residual(i1,i2,i3,wc)+adSelfAdjoint3dC(i1,i2,i3,wc)
    else if( use4thOrderAD.eq.1 )then
     ! compute adc2, adc4: 
     getDerivativesAndDissipation(3,2,GRIDTYPE)
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+ad23(adc2,uc)+ad43(adc4,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+ad23(adc2,vc)+ad43(adc4,vc)
     residual(i1,i2,i3,wc)=residual(i1,i2,i3,wc)+ad23(adc2,vc)+ad43(adc4,wc)
    end if

    if( computeTemperature.ne.0 )then
     residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux3c(tc)\
                                         -u(i1,i2,i3,vc)*uy3c(tc)\
                                         -u(i1,i2,i3,wc)*uz3c(tc)\
                                         +kThermal*lap3d2c(tc)
      ! --- artificial dissipation for T: do this for now: 
      if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
        residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+adSelfAdjoint3dC(i1,i2,i3,tc)
      else if( use4thOrderAD.eq.1 )then
       residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad23(adc2,tc)+ad43(adc4,tc)
     end if
    end if


   #End

  end if
  endLoops()

 else
  stop 888 ! unexpected value for nd
 end if
                     
#endMacro
