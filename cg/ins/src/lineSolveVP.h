c ************************************************************************
c   Visco-Plastic Model: 
c     Macros to define the equations for the line solver 
c
c  This file is included by insLineSolveNew.bf 
c
c In 2d the momentum equations are: 
c
c   u.t + u.grad(u) + p.x = Dx( 2*vp*u.x ) + Dy(   vp*u.y ) + Dy( vp*v.x )
c   v.t + u.grad(v) + p.y = Dx(   vp*v.x ) + Dy( 2*vp*v.y ) + Dx( vp*u.y )
c
c ************************************************************************

! The next include file defines conservative approximations to coefficent matrices
#Include consCoeff.h


c ===================================================================================
c        VP: incompressible Navier Stokes with a visco-plastic model
c                 *** rectangular grid version ***
c Macro arguments:
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsRectangularGridVP(dir)

 if( use4thOrderAD.eq.1 )then
  write(*,*) 'insLineSolve: 4th order diss not finished'
  stop 7654
 end if

 ! set default values for no 2nd order artificial diffusion: 
 cdm=0.
 cdDiag=0.
 cdp=0.

if( nd.eq.2 )then

 ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
 defineDerivativeMacros(2,2,rectangular)


 beginLoops()
  if( mask(i1,i2,i3).gt.0 )then
   if( use2ndOrderAD.eq.1 )then
     defineArtificialDiffusionCoefficients(2,dir,R,)
   end if

   ! Get the nonlinear viscosity at nearby points: 
   nuzmz=u(i1  ,i2-1,i3,vsc)
   numzz=u(i1-1,i2  ,i3,vsc)
   nuzzz=u(i1  ,i2  ,i3,vsc)
   nupzz=u(i1+1,i2  ,i3,vsc)
   nuzpz=u(i1  ,i2+1,i3,vsc)
   ! getViscoPlasticViscosityCoefficient(nuzmz,i1  ,i2-1,i3,2,rectangular)
   ! getViscoPlasticViscosityCoefficient(numzz,i1-1,i2  ,i3,2,rectangular)
   ! getViscoPlasticViscosityCoefficient(nuzzz,i1  ,i2  ,i3,2,rectangular)
   ! getViscoPlasticViscosityCoefficient(nupzz,i1+1,i2  ,i3,2,rectangular)
   ! getViscoPlasticViscosityCoefficient(nuzpz,i1  ,i2+1,i3,2,rectangular)

   ! u.t + u.grad(u) + p.x = Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dy( nu*v.x )
   ! v.t + u.grad(v) + p.y = Dx(   nu*v.x ) + Dy( 2*nu*v.y ) + Dx( nu*u.y )


   nu0ph = .5*( nupzz+nuzzz )  ! nu(i1+1/2,i2,i3)
   nu0mh = .5*( nuzzz+numzz )  ! nu(i1-1/2,i2,i3)

   nu1ph = .5*( nuzpz+nuzzz )  ! nu(i1,i2+1/2,i3)
   nu1mh = .5*( nuzzz+nuzmz )  ! nu(i1,i2-1/2,i3)

   if( computeMatrix.eq.1 )then
    #If #dir == "0"
     if( systemComponent.eq.uc )then  ! solve for u 
      am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir) -cdm   - 2.*nu0mh*dxvsqi(0)
      bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +cdDiag + 2.*(nu0ph+nu0mh)*dxvsqi(0)+(nu1ph+nu1mh)*dxvsqi(1)
      cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir) -cdp   - 2.*nu0ph*dxvsqi(0) 
     else if( systemComponent.eq.vc )then ! solve for v 
      am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir) -cdm   -    nu0mh*dxvsqi(0)
      bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +cdDiag + (nu0ph+nu0mh)*dxvsqi(0)+2.*(nu1ph+nu1mh)*dxvsqi(1)
      cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir) -cdp   -    nu0ph*dxvsqi(0) 
     else
       stop 6139
     end if
    #Else
     if( systemComponent.eq.uc )then  ! solve for u 
      am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir) -cdm   -    nu1mh*dxvsqi(1)
      bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +cdDiag + 2.*(nu0ph+nu0mh)*dxvsqi(0)+(nu1ph+nu1mh)*dxvsqi(1)
      cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir) -cdp   -    nu1ph*dxvsqi(1) 
     else if( systemComponent.eq.vc )then ! solve for v 
      am(i1,i2,i3)= -uu(uc+dir)*dxv2i(dir) -cdm   - 2.*nu1mh*dxvsqi(1)
      bm(i1,i2,i3)=  dtScale/dt(i1,i2,i3) +cdDiag + (nu0ph+nu0mh)*dxvsqi(0)+2.*(nu1ph+nu1mh)*dxvsqi(1)
      cm(i1,i2,i3)=  uu(uc+dir)*dxv2i(dir) -cdp   - 2.*nu1ph*dxvsqi(1) 
     else
       stop 6139
     end if
    #End

   end if
     
!     Here is the full residual: 
!       f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu) + uu(uc)*dtScale/dt(i1,i2,i3) \
!         -u(i1,i2,i3,uc)*ux2(uc)-u(i1,i2,i3,vc)*uy2(uc)-ux2(pc)\
!                          -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
!         +2.*(nu0ph*u(i1+1,i2,i3,uc) -(nu0ph+nu0mh)*u(i1,i2,i3,uc) + nu0mh*u(i1-1,i2,i3,uc))*dxvsqi(0)\
!         +   (nu1ph*u(i1,i2+1,i3,uc) -(nu1ph+nu1mh)*u(i1,i2,i3,uc) + nu1mh*u(i1,i2-1,i3,uc))*dxvsqi(1)\
!	 +   ( nuzpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))\
!             -nuzmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dx(0)*dx(1))
!
!       f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv) + uu(vc)*dtScale/dt(i1,i2,i3) \
!         -u(i1,i2,i3,uc)*ux2(vc)-u(i1,i2,i3,vc)*uy2(vc)-uy2(pc)\
!                          -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
!         +   (nu0ph*u(i1+1,i2,i3,vc) -(nu0ph+nu0mh)*u(i1,i2,i3,vc) + nu0mh*u(i1-1,i2,i3,vc))*dxvsqi(0)\
!         +2.*(nu1ph*u(i1,i2+1,i3,vc) -(nu1ph+nu1mh)*u(i1,i2,i3,vc) + nu1mh*u(i1,i2-1,i3,vc))*dxvsqi(1)\
!	 +   ( nupzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))\
!             -numzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dx(0)*dx(1))

   if( computeRHS.eq.1 )then

    #If #dir == "0"
      ! remove dir=0 implicit terms: 
      f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu) + uu(uc)*dtScale/dt(i1,i2,i3) \
                               -u(i1,i2,i3,vc)*uy2(uc)-ux2(pc)\
                         -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
                                                                                                     \
        +   (nu1ph*u(i1,i2+1,i3,uc)                               + nu1mh*u(i1,i2-1,i3,uc))*dxvsqi(1)\
	 +   (nuzpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))\
             -nuzmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dx(0)*dx(1))

      f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv) + uu(vc)*dtScale/dt(i1,i2,i3) \
                               -u(i1,i2,i3,vc)*uy2(vc)-uy2(pc)\
                         -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
                                                                                                     \
        +2.*(nu1ph*u(i1,i2+1,i3,vc)                               + nu1mh*u(i1,i2-1,i3,vc))*dxvsqi(1)\
	 +   (nupzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))\
             -numzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dx(0)*dx(1))

    #Else

      ! remove dir=1 implicit terms: 
      f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu) + uu(uc)*dtScale/dt(i1,i2,i3) \
        -u(i1,i2,i3,uc)*ux2(uc)                       -ux2(pc)\
                         -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
        +2.*(nu0ph*u(i1+1,i2,i3,uc)                               + nu0mh*u(i1-1,i2,i3,uc))*dxvsqi(0)\
                                                                                                     \
	 +   (nuzpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))\
             -nuzmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dx(0)*dx(1))

      f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv) + uu(vc)*dtScale/dt(i1,i2,i3) \
        -u(i1,i2,i3,uc)*ux2(vc)                       -uy2(pc)\
                         -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
        +   (nu0ph*u(i1+1,i2,i3,vc)                               + nu0mh*u(i1-1,i2,i3,vc))*dxvsqi(0)\
                                                                                                     \
	 +   (nupzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))\
             -numzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dx(0)*dx(1))


    #End
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

  stop 2945

else
  stop 888 ! unexpected value for nd
end if
                     
#endMacro


c ===================================================================================
c        VP: incompressible Navier Stokes with a visco-plastic model
c                 *** curvilinear grid version ***
c Macro arguments:
c  dir: 0,1,2
c====================================================================================
#beginMacro fillEquationsCurvilinearGridVP(dir)

 if( use4thOrderAD.eq.1 )then
  write(*,*) 'insLineSolve: 4th order diss not finished'
  stop 7654
 end if

 ! write(*,'(" -- fill tridiagonal matrix curvlinear VP --")')

 ! set default values for no 2nd order artificial diffusion: 
 cdm=0.
 cdDiag=0.
 cdp=0.

if( nd.eq.2 )then

 ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
 defineDerivativeMacros(2,2,curvilinear)


 beginLoops()
  if( mask(i1,i2,i3).gt.0 )then
   if( use2ndOrderAD.eq.1 )then
     defineArtificialDiffusionCoefficients(2,dir,C,)
   end if

   ! Get the nonlinear viscosity at nearby points: 
   nuzmz=u(i1  ,i2-1,i3,vsc)
   numzz=u(i1-1,i2  ,i3,vsc)
   nuzzz=u(i1  ,i2  ,i3,vsc)
   nupzz=u(i1+1,i2  ,i3,vsc)
   nuzpz=u(i1  ,i2+1,i3,vsc)
   ! Evaluate the nonlinear viscosity
   ! getViscoPlasticViscosityCoefficient(nuzmz,i1  ,i2-1,i3,2,curvilinear)
   ! getViscoPlasticViscosityCoefficient(numzz,i1-1,i2  ,i3,2,curvilinear)
   ! getViscoPlasticViscosityCoefficient(nuzzz,i1  ,i2  ,i3,2,curvilinear)
   ! getViscoPlasticViscosityCoefficient(nupzz,i1+1,i2  ,i3,2,curvilinear)
   ! getViscoPlasticViscosityCoefficient(nuzpz,i1  ,i2+1,i3,2,curvilinear)

   ! u.t + u.grad(u) + p.x = Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dy( nu*v.x )
   ! v.t + u.grad(v) + p.y = Dx(   nu*v.x ) + Dy( 2*nu*v.y ) + Dx( nu*u.y )

   ! evaluate the jacobian at nearby points:
   ajzmz = ajac2d(i1  ,i2-1,i3)
   ajmzz = ajac2d(i1-1,i2  ,i3)
   ajzzz = ajac2d(i1  ,i2  ,i3)
   ajpzz = ajac2d(i1+1,i2  ,i3)
   ajzpz = ajac2d(i1  ,i2+1,i3)

   ! 1. Get coefficients a11ph, a11mh, a22ph, etc. for 
   !          Dx( 2*nu*u.x ) + Dy(   nu*u.y ) 
   getCoeffForDxADxPlusDyBDy(au, 2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz, nuzmz,numzz,nuzzz,nupzz,nuzpz )

   ! 1b. Get coefficients b11ph,b11mh, etc. for 
   !            Dy( nu*v.x )
   getCoeffForDyADx( bu, nuzmz,numzz,nuzzz,nupzz,nuzpz )


   ! 2. Dx( nu*v.x ) + Dy( 2*nu*v.y ) 
   getCoeffForDxADxPlusDyBDy(av, nuzmz,numzz,nuzzz,nupzz,nuzpz, 2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz )

   ! 2b.  Dx( nu*u.y )
   getCoeffForDxADy( bv, nuzmz,numzz,nuzzz,nupzz,nuzpz )


   if( computeMatrix.eq.1 )then

    t1=(uu(uc)*rxi(dir,0)+uu(vc)*rxi(dir,1))*drv2i(dir)

    dr0i = 1./(ajzzz*dr(0)**2)
    dr1i = 1./(ajzzz*dr(1)**2)

    #If #dir == "0"
     if( systemComponent.eq.uc )then  ! solve for u 

      am(i1,i2,i3)= -t1                  -cdm    - au11mh*dr0i
      bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +cdDiag + (au11ph+au11mh)*dr0i +(au22ph+au22mh)*dr1i
      cm(i1,i2,i3)=  t1                  -cdp    - au11ph*dr0i
     else if( systemComponent.eq.vc )then ! solve for v 
      am(i1,i2,i3)= -t1                  -cdm    - av11mh*dr0i
      bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +cdDiag + (av11ph+av11mh)*dr0i +(av22ph+av22mh)*dr1i
      cm(i1,i2,i3)=  t1                  -cdp    - av11ph*dr0i
     else
       stop 6139
     end if
    #Else
     if( systemComponent.eq.uc )then  ! solve for u 
      am(i1,i2,i3)= -t1                  -cdm    - au22mh*dr1i
      bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +cdDiag + (au11ph+au11mh)*dr0i +(au22ph+au22mh)*dr1i
      cm(i1,i2,i3)=  t1                  -cdp    - au22ph*dr1i
     else if( systemComponent.eq.vc )then ! solve for v 
      am(i1,i2,i3)= -t1                  -cdm    - av22mh*dr1i
      bm(i1,i2,i3)= dtScale/dt(i1,i2,i3) +cdDiag + (av11ph+av11mh)*dr0i +(av22ph+av22mh)*dr1i
      cm(i1,i2,i3)=  t1                  -cdp    - av22ph*dr1i
     else
       stop 6139
     end if
    #End

   end if
     
!     Here is the full residual: 
!
!   residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2c(uc)-u(i1,i2,i3,vc)*uy2c(uc)-ux2c(pc)\
!                         -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
!        + ( \
!   + ( au11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - au11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2\
!   + ( au22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - au22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2\
!   + (au12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-au12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
!   + (au21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-au21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
!          \
!   + ( bu11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - bu11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
!   + ( bu22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - bu22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
!   + (bu12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-bu12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!   + (bu21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-bu21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!     )/ajzzz
!
!   residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(vc)-u(i1,i2,i3,vc)*uy2c(vc)-uy2c(pc)\
!                         -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
!          + ( \
!   + ( av11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - av11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
!   + ( av22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - av22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
!   + (av12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-av12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!   + (av21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-av21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!          \
!   + ( bv11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - bv11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2\
!   + ( bv22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - bv22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2\
!   + (bv12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-bv12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
!   + (bv21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-bv21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
!       )/ajzzz



   if( computeRHS.eq.1 )then

    #If #dir == "0"
      ! remove dir=0 implicit terms: 
      f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu) + uu(uc)*dtScale/dt(i1,i2,i3) \
            +(-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1))*us(uc) -ux2c(pc)\
                          -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
         + ( \
                                                                                                      \
    + ( au22ph*(u(i1,i2+1,i3,uc)               ) - au22mh*(              -u(i1,i2-1,i3,uc)) )/dr(1)**2\
    + (au12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-au12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
    + (au21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-au21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
           \
    + ( bu11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - bu11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
    + ( bu22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - bu22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
    + (bu12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-bu12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
    + (bu21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-bu21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
	)/ajzzz

      f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv) + uu(vc)*dtScale/dt(i1,i2,i3) \
            +(-uu(uc)*rxi(1,0)-uu(vc)*rxi(1,1))*us(vc) -uy2c(pc)\
                          -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
           + ( \
                                                                                                      \
    + ( av22ph*(u(i1,i2+1,i3,vc)               ) - av22mh*(              -u(i1,i2-1,i3,vc)) )/dr(1)**2\
    + (av12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-av12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
    + (av21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-av21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
           \
    + ( bv11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - bv11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2\
    + ( bv22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - bv22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2\
    + (bv12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-bv12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
    + (bv21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-bv21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
	)/ajzzz

    #Else

      ! remove dir=1 implicit terms: 
      f(i1,i2,i3,fcu)=f(i1,i2,i3,fcu) + uu(uc)*dtScale/dt(i1,i2,i3) \
               + (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1))*ur(uc) -ux2c(pc) \
                          -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
         + ( \
    + ( au11ph*(u(i1+1,i2,i3,uc)               ) - au11mh*(              -u(i1-1,i2,i3,uc)) )/dr(0)**2\
                                                                                                      \
    + (au12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-au12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
    + (au21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-au21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
           \
    + ( bu11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - bu11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
    + ( bu22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - bu22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
    + (bu12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-bu12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
    + (bu21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-bu21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
	)/ajzzz

      f(i1,i2,i3,fcv)=f(i1,i2,i3,fcv) + uu(vc)*dtScale/dt(i1,i2,i3) \
                 + (-uu(uc)*rxi(0,0)-uu(vc)*rxi(0,1))*ur(vc) -uy2c(pc)\
                          -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
           + ( \
    + ( av11ph*(u(i1+1,i2,i3,vc)               ) - av11mh*(              -u(i1-1,i2,i3,vc)) )/dr(0)**2\
                                                                                                      \
    + (av12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-av12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
    + (av21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-av21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
           \
    + ( bv11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - bv11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2\
    + ( bv22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - bv22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2\
    + (bv12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-bv12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
    + (bv21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-bv21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
	)/ajzzz

    #End
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

  stop 2945

else
  stop 888 ! unexpected value for nd
end if
                     
#endMacro




c==================================================================================
c  Residual Computation for VP: incompressible Navier Stokes with a visco-plastic model
c
c Macro args:
c  GRIDTYPE: rectangular, curvilinear
c====================================================================================
#beginMacro computeResidualVP(GRIDTYPE)

 ! write(*,'("new: computeResidualVP, use2ndOrderAD=",i2)') use2ndOrderAD

 if( use4thOrderAD.eq.1 )then
  write(*,*) 'insLineSolve: computeResidualVP: 4th order diss not finished'
  stop 7654
 end if

 if( nd.eq.2 )then

  ! defineDerivativeMacros(DIM,ORDER,GRIDTYPE) : defineMacro UX(cc) ux22r(i1,i2,i3,cc) etc. 
  ! defineDerivativeMacros(2,2,GRIDTYPE)

  beginLoops()
  if( mask(i1,i2,i3).gt.0 )then

   ! Get the nonlinear viscosity at nearby points: 
   nuzmz=u(i1  ,i2-1,i3,vsc)
   numzz=u(i1-1,i2  ,i3,vsc)
   nuzzz=u(i1  ,i2  ,i3,vsc)
   nupzz=u(i1+1,i2  ,i3,vsc)
   nuzpz=u(i1  ,i2+1,i3,vsc)
   ! Evaluate the nonlinear viscosity "nu"
   ! getViscoPlasticViscosityCoefficient(nuzmz,i1  ,i2-1,i3,2,GRIDTYPE)
   ! getViscoPlasticViscosityCoefficient(numzz,i1-1,i2  ,i3,2,GRIDTYPE)
   ! getViscoPlasticViscosityCoefficient(nuzzz,i1  ,i2  ,i3,2,GRIDTYPE)
   ! getViscoPlasticViscosityCoefficient(nupzz,i1+1,i2  ,i3,2,GRIDTYPE)
   ! getViscoPlasticViscosityCoefficient(nuzpz,i1  ,i2+1,i3,2,GRIDTYPE)

   #If #GRIDTYPE == "rectangular"
    nu0ph = .5*( nupzz+nuzzz )  ! nu(i1+1/2,i2,i3)
    nu0mh = .5*( nuzzz+numzz )  ! nu(i1-1/2,i2,i3)

    nu1ph = .5*( nuzpz+nuzzz )  ! nu(i1,i2+1/2,i3)
    nu1mh = .5*( nuzzz+nuzmz )  ! nu(i1,i2-1/2,i3)

    ! u.t + u.grad(u) + p.x = Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dy( nu*v.x )
    ! v.t + u.grad(v) + p.y = Dx(   nu*v.x ) + Dy( 2*nu*v.y ) + Dx( nu*u.y )
    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2(uc)-u(i1,i2,i3,vc)*uy2(uc)-ux2(pc)\
                          -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
         +2.*(nu0ph*u(i1+1,i2,i3,uc) -(nu0ph+nu0mh)*u(i1,i2,i3,uc) + nu0mh*u(i1-1,i2,i3,uc))*dxvsqi(0)\
         +   (nu1ph*u(i1,i2+1,i3,uc) -(nu1ph+nu1mh)*u(i1,i2,i3,uc) + nu1mh*u(i1,i2-1,i3,uc))*dxvsqi(1)\
	 +   (nuzpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))\
             -nuzmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dx(0)*dx(1))

    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2(vc)-u(i1,i2,i3,vc)*uy2(vc)-uy2(pc)\
                          -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
         +   (nu0ph*u(i1+1,i2,i3,vc) -(nu0ph+nu0mh)*u(i1,i2,i3,vc) + nu0mh*u(i1-1,i2,i3,vc))*dxvsqi(0)\
         +2.*(nu1ph*u(i1,i2+1,i3,vc) -(nu1ph+nu1mh)*u(i1,i2,i3,vc) + nu1mh*u(i1,i2-1,i3,vc))*dxvsqi(1)\
	 +   (nupzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))\
             -numzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dx(0)*dx(1))

    if( use2ndOrderAD.eq.1 )then
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+adSelfAdjoint2dR(i1,i2,i3,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+adSelfAdjoint2dR(i1,i2,i3,vc)
    end if

    if( computeTemperature.ne.0 )then
      residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2(tc)-u(i1,i2,i3,vc)*uy2(tc)+kThermal*lap2d2(tc)
      ! --- artificial dissipation for T: do this for now: 
      if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
        residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+adSelfAdjoint2dR(i1,i2,i3,tc)
      else if( use4thOrderAD.eq.1 )then
       ! compute adc2, adc4: 
       getDerivativesAndDissipation(2,2,GRIDTYPE)
       residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad2(adc2,tc)+ad4(adc4,tc)
      end if
    end if

   #Else
     ! ************ VP curvilinear case  ********************

    ! evaluate the jacobian at nearby points:
    ajzmz = ajac2d(i1  ,i2-1,i3)
    ajmzz = ajac2d(i1-1,i2  ,i3)
    ajzzz = ajac2d(i1  ,i2  ,i3)
    ajpzz = ajac2d(i1+1,i2  ,i3)
    ajzpz = ajac2d(i1  ,i2+1,i3)

    ! 1. Get coefficients a11ph, a11mh, a22ph, etc. for 
    !          Dx( 2*nu*u.x ) + Dy(   nu*u.y ) 
    getCoeffForDxADxPlusDyBDy(a, 2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz, nuzmz,numzz,nuzzz,nupzz,nuzpz )

    ! 1b. Get coefficients b11ph,b11mh, etc. for 
    !            Dy( nu*v.x )
    getCoeffForDyADx( b, nuzmz,numzz,nuzzz,nupzz,nuzpz )


    residual(i1,i2,i3,uc)=f(i1,i2,i3,uc)-u(i1,i2,i3,uc)*ux2c(uc)-u(i1,i2,i3,vc)*uy2c(uc)-ux2c(pc)\
                          -thermalExpansivity*gravity(0)*u(i1,i2,i3,tc) \
         + ( \
    + ( a11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - a11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2\
    + ( a22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - a22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2\
    + (a12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-a12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
    + (a21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-a21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
           \
    + ( b11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - b11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
    + ( b22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - b22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
    + (b12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-b12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
    + (b21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-b21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
	)/ajzzz

    ! 2. Dx( nu*v.x ) + Dy( 2*nu*v.y ) 
    getCoeffForDxADxPlusDyBDy(a, nuzmz,numzz,nuzzz,nupzz,nuzpz, 2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz )

    ! 2b.  Dx( nu*u.y )
    getCoeffForDxADy( b, nuzmz,numzz,nuzzz,nupzz,nuzpz )


    residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(vc)-u(i1,i2,i3,vc)*uy2c(vc)-uy2c(pc)\
                          -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
           + ( \
    + ( a11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - a11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
    + ( a22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - a22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
    + (a12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-a12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
    + (a21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-a21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
           \
    + ( b11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - b11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2\
    + ( b22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - b22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2\
    + (b12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-b12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
    + (b21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-b21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
	)/ajzzz

    if( use2ndOrderAD.eq.1 )then
     residual(i1,i2,i3,uc)=residual(i1,i2,i3,uc)+adSelfAdjoint2dC(i1,i2,i3,uc)
     residual(i1,i2,i3,vc)=residual(i1,i2,i3,vc)+adSelfAdjoint2dC(i1,i2,i3,vc)
    end if

    if( computeTemperature.ne.0 )then
      residual(i1,i2,i3,tc)=f(i1,i2,i3,tc)-u(i1,i2,i3,uc)*ux2c(tc)-u(i1,i2,i3,vc)*uy2c(tc)+kThermal*lap2d2c(tc)
      ! --- artificial dissipation for T: do this for now: 
      if( use2ndOrderAD.eq.1 .and. use4thOrderAD.eq.0 )then
        residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+adSelfAdjoint2dC(i1,i2,i3,tc)
      else if( use4thOrderAD.eq.1 )then
      ! compute adc2, adc4: 
       getDerivativesAndDissipation(2,2,GRIDTYPE)
       residual(i1,i2,i3,tc)=residual(i1,i2,i3,tc)+ad2(adc2,tc)+ad4(adc4,tc)
      end if
    end if

   #End

  end if
  endLoops()

 else if( nd.eq.3 )then

  stop 2945

 else
  stop 888 ! unexpected value for nd
 end if
                     
#endMacro
