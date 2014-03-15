! **************************************************************************************************
!   Define the full implicit matrix for the Visco-plastic model of the incompressible Navier-Stokes
! **************************************************************************************************

! Macro's for forming the general implicit matrix
#Include "insImp.h"

! The next include file defines conservative approximations to coefficent matrices
#Include "consCoeff.h"

! =============================================================
! macro to declare temporary variables:
! =============================================================
#beginMacro declareInsImpTemporaryVariables()
 declareTemporaryVariables(2,2)
 declareParametricDerivativeVariables(uu,3)   ! declare temp variables uu, uur, uus, ...
 declareParametricDerivativeVariables(vv,3) 
 declareParametricDerivativeVariables(ww,3) 
 declareParametricDerivativeVariables(pp,3) 
 declareParametricDerivativeVariables(qq,3) 
 declareParametricDerivativeVariables(uul,3)   ! declare temp variables uu, uur, uus, ...
 declareParametricDerivativeVariables(vvl,3) 
 declareParametricDerivativeVariables(wwl,3) 
 declareParametricDerivativeVariables(qql,3) 
 declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)

 ! This macro is from consCoeff.h
 declareNonLinearViscosityVariables()
#endMacro

! =====================================================================================
! This macro is used in insImp.h and is used to look up parameters etc. for this PDE
! =====================================================================================
#beginMacro initializePdeParameters()
 if( pdeModel.ne.viscoPlasticModel .and. turbulenceModel.ne.largeEddySimulation )then
   write(*,'("InsImpVP:ERROR: pdeModel=",i4," and turbulenceModel=",i4," not expected")') pdeModel,turbulenceModel
   stop 1753
 end if
 if( qc.lt.0 .or. qc.gt.1000 )then
   write(*,'("InsImpVP:ERROR: The component number for temperature qc=",i6," is not valid")') qc
   stop 1754
 end if
 ! make sure these are initialized (used in evaluation of adCoeffl even if cd22=0. )
 ulx=0.
 uly=0.
 ulz=0.
 vlx=0.
 vly=0.
 vlz=0.
 wlx=0.
 wly=0.
 wlz=0.
#endMacro

! =============================================================================================================
!  Get the coefficients for a component of the conservative discretization of the div(tensor grad) operator
!  
!  Macro parameters:
!    ul    : evaluate the viscosity with ul(i1,i2,i3,vsc)
!    scale : scale the coefficients by this value 
! =============================================================================================================
#beginMacro getDivTensorGradCoefficientsINSVP(ul,scale)
#If $DIM == 2 

 ! ---------- 2D -----------

 ! Get the nonlinear viscosity at nearby points: 
 nuzmz=ul(i1  ,i2-1,i3,vsc)
 numzz=ul(i1-1,i2  ,i3,vsc)
 nuzzz=ul(i1  ,i2  ,i3,vsc)
 nupzz=ul(i1+1,i2  ,i3,vsc)
 nuzpz=ul(i1  ,i2+1,i3,vsc)
 
 ! u.t + u.grad(u) + p.x = Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dy( nu*v.x )
 ! v.t + u.grad(v) + p.y = Dx(   nu*v.x ) + Dy( 2*nu*v.y ) + Dx( nu*u.y )

 #If $GRIDTYPE eq "curvilinear"
  ! evaluate the jacobian at nearby points:
  ajzmz = ajac2d(i1  ,i2-1,i3)
  ajmzz = ajac2d(i1-1,i2  ,i3)
  ajzzz = ajac2d(i1  ,i2  ,i3)
  ajpzz = ajac2d(i1+1,i2  ,i3)
  ajzpz = ajac2d(i1  ,i2+1,i3)
 
  ! 1. Get coefficients au11ph, au11mh, au22ph, etc. for 
  !          Dx( 2*nu*u.x ) + Dy(   nu*u.y ) 
  getCoeffForDxADxPlusDyBDy(au, 2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz, nuzmz,numzz,nuzzz,nupzz,nuzpz )
 
  ! 1b. Get coefficients av11ph,av11mh, etc. for 
  !            Dy( nu*v.x )
  getCoeffForDyADx( av, nuzmz,numzz,nuzzz,nupzz,nuzpz )
 
  ! 1. Get coefficients bv11ph, bv11mh, bv22ph, etc. for 
  ! 2. Dx( nu*v.x ) + Dy( 2*nu*v.y ) 
  getCoeffForDxADxPlusDyBDy(bv, nuzmz,numzz,nuzzz,nupzz,nuzpz, 2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz )
 
  ! 2b.  Dx( nu*u.y )
  getCoeffForDxADy( bu, nuzmz,numzz,nuzzz,nupzz,nuzpz )
 
  ! scaling factors: 
  dr0i = (scale)/(ajzzz*dr(0)**2)
  dr1i = (scale)/(ajzzz*dr(1)**2)
  dr0dr1 = (scale)/(ajzzz*4.*dr(0)*dr(1))
 
  scaleCoefficients( au11ph,au11mh,au22ph,au22mh,au12pzz,au12mzz,au21zpz,au21zmz )
  scaleCoefficients( av11ph,av11mh,av22ph,av22mh,av12pzz,av12mzz,av21zpz,av21zmz )

  scaleCoefficients( bu11ph,bu11mh,bu22ph,bu22mh,bu12pzz,bu12mzz,bu21zpz,bu21zmz )
  scaleCoefficients( bv11ph,bv11mh,bv22ph,bv22mh,bv12pzz,bv12mzz,bv21zpz,bv21zmz )

 #Elif $GRIDTYPE eq "rectangular"

   nu0ph = .5*( nupzz+nuzzz )  ! nu(i1+1/2,i2,i3)
   nu0mh = .5*( nuzzz+numzz )  ! nu(i1-1/2,i2,i3)

   nu1ph = .5*( nuzpz+nuzzz )  ! nu(i1,i2+1/2,i3)
   nu1mh = .5*( nuzzz+nuzmz )  ! nu(i1,i2-1/2,i3)


   au11ph = 2.*nu0ph*dxvsqi(0)*(scale)
   au11mh = 2.*nu0mh*dxvsqi(0)*(scale)
   au22ph =    nu1ph*dxvsqi(1)*(scale)
   au22mh =    nu1mh*dxvsqi(1)*(scale)
   au12pzz=0.
   au12mzz=0.
   au21zpz=0.
   au21zmz=0.
  
   av11ph=0.
   av11mh=0.
   av22ph=0.
   av22mh=0.
   av12pzz=0.
   av12mzz=0.
   av21zpz=nuzpz/(4.*dx(0)*dx(1))*(scale)
   av21zmz=nuzmz/(4.*dx(0)*dx(1))*(scale)
   
   bv11ph =    nu0ph*dxvsqi(0)*(scale)
   bv11mh =    nu0mh*dxvsqi(0)*(scale)
   bv22ph = 2.*nu1ph*dxvsqi(1)*(scale)
   bv22mh = 2.*nu1mh*dxvsqi(1)*(scale)
   bv12pzz=0.
   bv12mzz=0.
   bv21zpz=0.
   bv21zmz=0.
  
   bu11ph=0.
   bu11mh=0.
   bu22ph=0.
   bu22mh=0.
   bu12pzz=nupzz/(4.*dx(0)*dx(1))*(scale)
   bu12mzz=numzz/(4.*dx(0)*dx(1))*(scale)
   bu21zpz=0.
   bu21zmz=0.
   

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


 #Else
   stop 1101
 #End


#Else

 ! ---------- 3D -----------

 ! Get the nonlinear viscosity at nearby points: 
 nuzzm=ul(i1  ,i2  ,i3-1,vsc)
 nuzmz=ul(i1  ,i2-1,i3  ,vsc)
 numzz=ul(i1-1,i2  ,i3  ,vsc)
 nuzzz=ul(i1  ,i2  ,i3  ,vsc)
 nupzz=ul(i1+1,i2  ,i3  ,vsc)
 nuzpz=ul(i1  ,i2+1,i3  ,vsc)
 nuzzp=ul(i1  ,i2  ,i3+1,vsc)
 
 ! u.t + u.grad(u) + p.x = Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dz( nu*u.z ) + Dy( nu*v.x )  + Dz( nu*w.x )
 ! v.t + u.grad(v) + p.y = Dx(   nu*v.x ) + Dy( 2*nu*v.y ) + Dz( nu*v.z ) + Dx( nu*u.y )  + Dz( nu*w.y ) 
 ! w.t + u.grad(w) + p.z = Dx(   nu*w.x ) + Dy(   nu*w.y ) + Dz( nu*w.z ) + Dx( nu*u.z )  + Dy( nu*v.z ) 

 #If $GRIDTYPE eq "curvilinear"
  ! evaluate the jacobian at nearby points:
  ajzzm = ajac3d(i1  ,i2  ,i3-1)
  ajzmz = ajac3d(i1  ,i2-1,i3  )
  ajmzz = ajac3d(i1-1,i2  ,i3  )
  ajzzz = ajac3d(i1  ,i2  ,i3  )
  ajpzz = ajac3d(i1+1,i2  ,i3  )
  ajzpz = ajac3d(i1  ,i2+1,i3  )
  ajzzp = ajac3d(i1  ,i2  ,i3+1)
 
  ! ------------------------------------------------------------------------------------------------------------
  ! au. Get coefficients au11ph, au11mh, au22ph, etc. for 
  !          Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dz(   nu*u.z )  
  getCoeffForDxADxPlusDyBDyPlusDzCDz(au, 2.*nuzzm,2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz,2.*nuzzp, \
                                         nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp, \
                                         nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )
  ! av. Get coefficients av11ph,av11mh, etc. for 
  !            Dy( nu*v.x )
  getCoeffForDxADy3d( av, y, x, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )
  ! aw. Get coefficients aw11ph,aw11mh, etc. for 
  !            Dz( nu*w.x )
  getCoeffForDxADy3d( aw, z, x, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )

  ! ------------------------------------------------------------------------------------------------------------
  ! bu. Get coefficients bu11ph,bu11mh, etc. for 
  !            Dx( nu*u.y )
  getCoeffForDxADy3d( bu, x, y, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )
  ! bv. Get coefficients bv11ph, bv11mh, bv22ph, etc. for 
  !          Dx( nu*v.x ) + Dy( 2*nu*v.y ) + Dz(   nv*v.z )  
  getCoeffForDxADxPlusDyBDyPlusDzCDz(bv, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp, \
                                         2.*nuzzm,2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz,2.*nuzzp, \
                                         nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )
  ! bw. Get coefficients bw11ph,bw11mh, etc. for 
  !            Dz( nu*w.y )
  getCoeffForDxADy3d( bw, z, y, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )
 
  ! ------------------------------------------------------------------------------------------------------------
  ! cu. Get coefficients cu11ph,cu11mh, etc. for 
  !            Dx( nu*u.z )
  getCoeffForDxADy3d( cu, x, z, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )

  ! cv. Get coefficients cv11ph,cv11mh, etc. for 
  !            Dy( nu*v.z )
  getCoeffForDxADy3d( cv, y, z, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp )
  ! cw. Get coefficients cw11ph, cw11mh, cw22ph, etc. for 
  !          Dx( nu*w.x ) + Dy( nu*w.y ) + Dz( 2*nv*w.z )  
  getCoeffForDxADxPlusDyBDyPlusDzCDz(cw, nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp, \
                                         nuzzm,nuzmz,numzz,nuzzz,nupzz,nuzpz,nuzzp, \
                                         2.*nuzzm,2.*nuzmz,2.*numzz,2.*nuzzz,2.*nupzz,2.*nuzpz,2.*nuzzp )
 
 
  ! scaling factors: 
  dr0i = (scale)/(ajzzz*dr(0)**2)
  dr1i = (scale)/(ajzzz*dr(1)**2)
  dr2i = (scale)/(ajzzz*dr(2)**2)
  dr0dr1 = (scale)/(ajzzz*4.*dr(0)*dr(1))
  dr0dr2 = (scale)/(ajzzz*4.*dr(0)*dr(2))
  dr1dr2 = (scale)/(ajzzz*4.*dr(1)*dr(2))
 
  scaleCoefficients3d( au11ph,au11mh,au22ph,au22mh,au33ph,au33mh,au12pzz,au12mzz,au13pzz,au13mzz,au21zpz,au21zmz,au23zpz,au23zmz,au31zzp,au31zzm,au32zzp,au32zzm )
  scaleCoefficients3d( av11ph,av11mh,av22ph,av22mh,av33ph,av33mh,av12pzz,av12mzz,av13pzz,av13mzz,av21zpz,av21zmz,av23zpz,av23zmz,av31zzp,av31zzm,av32zzp,av32zzm )
  scaleCoefficients3d( aw11ph,aw11mh,aw22ph,aw22mh,aw33ph,aw33mh,aw12pzz,aw12mzz,aw13pzz,aw13mzz,aw21zpz,aw21zmz,aw23zpz,aw23zmz,aw31zzp,aw31zzm,aw32zzp,aw32zzm )

  scaleCoefficients3d( bu11ph,bu11mh,bu22ph,bu22mh,bu33ph,bu33mh,bu12pzz,bu12mzz,bu13pzz,bu13mzz,bu21zpz,bu21zmz,bu23zpz,bu23zmz,bu31zzp,bu31zzm,bu32zzp,bu32zzm )
  scaleCoefficients3d( bv11ph,bv11mh,bv22ph,bv22mh,bv33ph,bv33mh,bv12pzz,bv12mzz,bv13pzz,bv13mzz,bv21zpz,bv21zmz,bv23zpz,bv23zmz,bv31zzp,bv31zzm,bv32zzp,bv32zzm )
  scaleCoefficients3d( bw11ph,bw11mh,bw22ph,bw22mh,bw33ph,bw33mh,bw12pzz,bw12mzz,bw13pzz,bw13mzz,bw21zpz,bw21zmz,bw23zpz,bw23zmz,bw31zzp,bw31zzm,bw32zzp,bw32zzm )

  scaleCoefficients3d( cu11ph,cu11mh,cu22ph,cu22mh,cu33ph,cu33mh,cu12pzz,cu12mzz,cu13pzz,cu13mzz,cu21zpz,cu21zmz,cu23zpz,cu23zmz,cu31zzp,cu31zzm,cu32zzp,cu32zzm )
  scaleCoefficients3d( cv11ph,cv11mh,cv22ph,cv22mh,cv33ph,cv33mh,cv12pzz,cv12mzz,cv13pzz,cv13mzz,cv21zpz,cv21zmz,cv23zpz,cv23zmz,cv31zzp,cv31zzm,cv32zzp,cv32zzm )
  scaleCoefficients3d( cw11ph,cw11mh,cw22ph,cw22mh,cw33ph,cw33mh,cw12pzz,cw12mzz,cw13pzz,cw13mzz,cw21zpz,cw21zmz,cw23zpz,cw23zmz,cw31zzp,cw31zzm,cw32zzp,cw32zzm )


 #Elif $GRIDTYPE eq "rectangular"

   nu0ph = .5*( nupzz+nuzzz )  ! nu(i1+1/2,i2,i3)
   nu0mh = .5*( nuzzz+numzz )  ! nu(i1-1/2,i2,i3)

   nu1ph = .5*( nuzpz+nuzzz )  ! nu(i1,i2+1/2,i3)
   nu1mh = .5*( nuzzz+nuzmz )  ! nu(i1,i2-1/2,i3)

   nu2ph = .5*( nuzzp+nuzzz )  ! nu(i1,i2,i3+1/2)
   nu2mh = .5*( nuzzz+nuzzm )  ! nu(i1,i2,i3-1/2)

   ! equation for u
   !    Dx( 2*nu*u.x ) + Dy(   nu*u.y ) + Dz(   nu*u.z )  
   au11ph = 2.*nu0ph*dxvsqi(0)*(scale)
   au11mh = 2.*nu0mh*dxvsqi(0)*(scale)
   au22ph =    nu1ph*dxvsqi(1)*(scale)
   au22mh =    nu1mh*dxvsqi(1)*(scale)
   au33ph =    nu2ph*dxvsqi(2)*(scale)
   au33mh =    nu2mh*dxvsqi(2)*(scale)
   au12pzz=0.
   au12mzz=0.
   au13pzz=0.
   au13mzz=0.
   au21zpz=0.
   au21zmz=0.
   au23zpz=0.
   au23zmz=0.
   au31zzp=0.
   au31zzm=0.
   au32zzp=0.
   au32zzm=0.
   !     Dy( nu*v.x )
   av11ph=0.
   av11mh=0.
   av22ph=0.
   av22mh=0.
   av33ph=0.
   av33mh=0.
   av12pzz=0.
   av12mzz=0.
   av13pzz=0.
   av13mzz=0.
   av21zpz=nuzpz/(4.*dx(0)*dx(1))*(scale)
   av21zmz=nuzmz/(4.*dx(0)*dx(1))*(scale)
   av23zpz=0.
   av23zmz=0.
   av31zzp=0.
   av31zzm=0.
   av32zzp=0.
   av32zzm=0.
   !    Dz( nu*w.x )
   aw11ph=0.
   aw11mh=0.
   aw22ph=0.
   aw22mh=0.
   aw33ph=0.
   aw33mh=0.
   aw12pzz=0.
   aw12mzz=0.
   aw13pzz=0.
   aw13mzz=0.
   aw21zpz=0.
   aw21zmz=0.
   aw23zpz=0.
   aw23zmz=0.
   aw31zzp=nuzzp/(4.*dx(0)*dx(2))*(scale)
   aw31zzm=nuzzm/(4.*dx(0)*dx(2))*(scale)
   aw32zzp=0.
   aw32zzm=0.

   ! equation for v 
   !     Dx( nu*u.y )
   bu11ph=0.
   bu11mh=0.
   bu22ph=0.
   bu22mh=0.
   bu33ph=0.
   bu33mh=0.
   bu12pzz=nupzz/(4.*dx(0)*dx(1))*(scale)
   bu12mzz=numzz/(4.*dx(0)*dx(1))*(scale)
   bu13pzz=0.
   bu13mzz=0.
   bu21zpz=0.
   bu21zmz=0.
   bu23zpz=0.
   bu23zmz=0.
   bu31zzp=0.
   bu31zzm=0.
   bu32zzp=0.
   bu32zzm=0.
   !     Dx( nu*v.x ) + Dy( 2*nu*v.y ) + Dz(   nu*v.z ) 
   bv11ph =    nu0ph*dxvsqi(0)*(scale)
   bv11mh =    nu0mh*dxvsqi(0)*(scale)
   bv22ph = 2.*nu1ph*dxvsqi(1)*(scale)
   bv22mh = 2.*nu1mh*dxvsqi(1)*(scale)
   bv33ph =    nu2ph*dxvsqi(2)*(scale)
   bv33mh =    nu2mh*dxvsqi(2)*(scale)
   bv12pzz=0.
   bv12mzz=0.
   bv13pzz=0.
   bv13mzz=0.
   bv21zpz=0.
   bv21zmz=0.
   bv23zpz=0.
   bv23zmz=0.
   bv31zzp=0.
   bv31zzm=0.
   bv32zzp=0.
   bv32zzm=0.
   !  Dz( nu*w.y )
   bw11ph=0.
   bw11mh=0.
   bw22ph=0.
   bw22mh=0.
   bw33ph=0.
   bw33mh=0.
   bw12pzz=0.
   bw12mzz=0.
   bw13pzz=0.
   bw13mzz=0.
   bw21zpz=0.
   bw21zmz=0.
   bw23zpz=0.
   bw23zmz=0.
   bw31zzp=0.
   bw31zzm=0.
   bw32zzp=nuzzp/(4.*dx(1)*dx(2))*(scale)
   bw32zzm=nuzzm/(4.*dx(1)*dx(2))*(scale)

   ! equation for w 
   !    Dx( nu*u.z )
   cu11ph=0.
   cu11mh=0.
   cu22ph=0.
   cu22mh=0.
   cu33ph=0.
   cu33mh=0.
   cu12pzz=0.
   cu12mzz=0.
   cu13pzz=nupzz/(4.*dx(0)*dx(2))*(scale)
   cu13mzz=numzz/(4.*dx(0)*dx(2))*(scale)
   cu21zpz=0.
   cu21zmz=0.
   cu23zpz=0.
   cu23zmz=0.
   cu31zzp=0.
   cu31zzm=0.
   cu32zzp=0.
   cu32zzm=0.
   !   Dy( nu*v.z )
   cv11ph=0.
   cv11mh=0.
   cv22ph=0.
   cv22mh=0.
   cv33ph=0.
   cv33mh=0.
   cv12pzz=0.
   cv12mzz=0.
   cv13pzz=0.
   cv13mzz=0.
   cv21zpz=0.
   cv21zmz=0.
   cv23zpz=nuzpz/(4.*dx(1)*dx(2))*(scale)
   cv23zmz=nuzmz/(4.*dx(1)*dx(2))*(scale)
   cv31zzp=0.
   cv31zzm=0.
   cv32zzp=0.
   cv32zzm=0.
   !   Dx( nu*w.x ) + Dy( nu*w.y ) + Dz( 2*nu*w.z )
   cw11ph =    nu0ph*dxvsqi(0)*(scale)
   cw11mh =    nu0mh*dxvsqi(0)*(scale)
   cw22ph =    nu1ph*dxvsqi(1)*(scale)
   cw22mh =    nu1mh*dxvsqi(1)*(scale)
   cw33ph = 2.*nu2ph*dxvsqi(2)*(scale)
   cw33mh = 2.*nu2mh*dxvsqi(2)*(scale)
   cw12pzz=0.
   cw12mzz=0.
   cw13pzz=0.
   cw13mzz=0.
   cw21zpz=0.
   cw21zmz=0.
   cw23zpz=0.
   cw23zmz=0.
   cw31zzp=0.
   cw31zzm=0.
   cw32zzp=0.
   cw32zzm=0.

 #Else
   stop 1102
 #End

#End
#endMacro


! ==============================================================================================================
!   Fill in the coefficients for the visco-plastic model : Incompressible Navier-Stokes Equations
! 
!  See also : lineSolveVP.h 
! ==============================================================================================================
#beginMacro fillCoeffPDE()

if( fillCoefficients.eq.1 )then
dtImp=dt*implicitFactor
beginLoops()

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,1)

  ! evaluate the coeff operators 
  getCoeff(identity, iCoeff,aj)
  getCoeff(laplacian, lapCoeff,aj)
  getCoeff(x, xCoeff,aj)
  getCoeff(y, yCoeff,aj)
  #If $DIM == 3
   getCoeff(z, zCoeff,aj)
  #End

  ! dissCoeff = dr^2*D_rr + ds^2*D_ss [ + dt^2*D_tt ] 
  getCoeff(r2Dissipation, dissCoeff,aj )

  ! for testing, get coeff for div( s grad )
  ! getOpCoeffDivScalarGrad(s(i1,i2,i3,0))

  ! evaluate forward derivatives of the current solution: 

  ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
  ! MAXDER = max number of parametric derivatives to precompute.
  opEvalParametricDerivative(u,uc,uu,1)    ! computes uur, uus 
  ! Evaluate the spatial derivatives of u (uses uur, uus):
  getOp(x, u,uc,uu,aj,u0x)       ! u.x
  getOp(y, u,uc,uu,aj,u0y)       ! u.y
  #If $DIM == 3
   getOp(z, u,uc,uu,aj,u0z)       ! u.z
  #End

  ! parametric derivatives of v: 
  opEvalParametricDerivative(u,vc,vv,1)        ! computes vvr, vvs 
  getOp(x, u,vc,vv,aj,v0x)       ! v.x
  getOp(y, u,vc,vv,aj,v0y)       ! v.y 
  #If $DIM == 3
   getOp(z, u,vc,vv,aj,v0z)      ! q.z

   opEvalParametricDerivative(u,wc,ww,1)        ! computes vvr, vvs 
   getOp(x, u,wc,ww,aj,w0x)       ! w.x
   getOp(y, u,wc,ww,aj,w0y)       ! w.y 
   getOp(z, u,wc,ww,aj,w0z)       ! w.z
  #End

    ! parametric derivatives of T:
  opEvalParametricDerivative(u,qc,qq,1)        ! computes qqr, qqs 
  getOp(x, u,qc,qq,aj,q0x)       ! q.x
  getOp(y, u,qc,qq,aj,q0y)       ! q.y 
  #If $DIM == 3
   getOp(z, u,qc,qq,aj,q0z)      ! q.z
  #End

 if( gridIsMoving.ne.0 )then
   ugv = advectionCoefficient*uu - gv(i1,i2,i3,0)
   vgv = advectionCoefficient*vv - gv(i1,i2,i3,1)
  #If $DIM == 3
   wgv = advectionCoefficient*ww - gv(i1,i3,i3,2)
  #End
 else
   ugv = advectionCoefficient*uu
   vgv = advectionCoefficient*vv
  #If $DIM == 3
   wgv = advectionCoefficient*ww
  #End
 end if

 ! get coefficients scaled by -dt*implicitFactor: 
 getDivTensorGradCoefficientsINSVP(u,-dtImp)

 getArtificialDissipationCoeff(adCoeff, u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z )

 #If $DIM == 2 


  ! Form : I - dtImp*div( nu(u).grad) + aDt*u*Dx + aDt*v*Dy 


  ! First set coeff = - dt*div( nu(u).grad)
  setDivTensorGradCoeff2d(cmpu,eqnu,au11ph,au11mh,au22ph,au22mh,au12pzz,au12mzz,au21zpz,au21zmz)
  setDivTensorGradCoeff2d(cmpv,eqnu,av11ph,av11mh,av22ph,av22mh,av12pzz,av12mzz,av21zpz,av21zmz)

  setDivTensorGradCoeff2d(cmpu,eqnv,bu11ph,bu11mh,bu22ph,bu22mh,bu12pzz,bu12mzz,bu21zpz,bu21zmz)
  setDivTensorGradCoeff2d(cmpv,eqnv,bv11ph,bv11mh,bv22ph,bv22mh,bv12pzz,bv12mzz,bv21zpz,bv21zmz)

  ! Now add on the identity and nonlinear terms:
  !    u0*ux + v0*uy + u*u0x + v*u0y 
  addCoeff5(cmpu,eqnu,coeff,iCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,bDt*u0x*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpv,eqnu,coeff, bDt*u0y*iCoeff)  ! v*u0y in u eqn
  !    u0*vx + v0*vy + u*v0x + v*v0y 
  addCoeff5(cmpv,eqnv,coeff,iCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,bDt*v0y*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpu,eqnv,coeff, bDt*v0x*iCoeff)  ! u*v0x in v eqn

  ! Temperature equation

  setCoeff4(cmpq,eqnq,coeff,iCoeff,-kDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff)
  addCoeff1(cmpu,eqnq,coeff, bDt*q0x*iCoeff)  ! u*q0x 
  addCoeff1(cmpv,eqnq,coeff, bDt*q0y*iCoeff)  ! v*q0y 

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
!   + ( av11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - av11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
!   + ( av22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - av22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
!   + (av12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-av12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!   + (av21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-av21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!     )/ajzzz
!
!   residual(i1,i2,i3,vc)=f(i1,i2,i3,vc)-u(i1,i2,i3,uc)*ux2c(vc)-u(i1,i2,i3,vc)*uy2c(vc)-uy2c(pc)\
!                         -thermalExpansivity*gravity(1)*u(i1,i2,i3,tc) \
!          + ( \
!   + ( bu11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - bu11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )/dr(0)**2\
!   + ( bu22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - bu22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )/dr(1)**2\
!   + (bu12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-bu12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
!   + (bu21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-bu21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))/(4.*dr(0)*dr(1))\
!          \
!   + ( bv11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - bv11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )/dr(0)**2\
!   + ( bv22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - bv22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )/dr(1)**2\
!   + (bv12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-bv12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!   + (bv21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-bv21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))/(4.*dr(0)*dr(1))\
!       )/ajzzz


 #Else

  ! u eqn : 
  setDivTensorGradCoeff3d(cmpu,eqnu,au)
  setDivTensorGradCoeff3d(cmpv,eqnu,av)
  setDivTensorGradCoeff3d(cmpw,eqnu,aw)
  !    u0*ux + v0*uy + w0*uz + u*u0x + v*u0y + w*u0z 
  addCoeff6(cmpu,eqnu,coeff,iCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,bDt*u0x*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpv,eqnu,coeff, bDt*u0y*iCoeff)  ! v*u0y in u eqn
  addCoeff1(cmpw,eqnu,coeff, bDt*u0z*iCoeff)  ! w*u0z in u eqn

  ! v eqn : 
  setDivTensorGradCoeff3d(cmpu,eqnv,bu)
  setDivTensorGradCoeff3d(cmpv,eqnv,bv)
  setDivTensorGradCoeff3d(cmpw,eqnv,bw)
  !    u0*vx + v0*vy + w0*vz + u*v0x + v*v0y + w*v0z 
  addCoeff6(cmpv,eqnv,coeff,iCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,bDt*v0y*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpu,eqnv,coeff, bDt*v0x*iCoeff)  ! u*v0x in v eqn
  addCoeff1(cmpw,eqnv,coeff, bDt*v0z*iCoeff)  ! w*v0z in v eqn

  ! w eqn : 
  setDivTensorGradCoeff3d(cmpu,eqnw,cu)
  setDivTensorGradCoeff3d(cmpv,eqnw,cv)
  setDivTensorGradCoeff3d(cmpw,eqnw,cw)
  !    u0*wx + v0*wy + w0*wz + u*w0x + v*w0y + w*w0z 
  addCoeff6(cmpw,eqnw,coeff,iCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff,bDt*w0z*iCoeff,-adCoeff*dt*dissCoeff)
  addCoeff1(cmpu,eqnw,coeff, bDt*w0x*iCoeff)  ! u*w0x in w eqn
  addCoeff1(cmpv,eqnw,coeff, bDt*w0y*iCoeff)  ! v*w0y in w eqn

  ! Temperature equation
  setCoeff5(cmpq,eqnq,coeff,iCoeff,-kDt*lapCoeff,adt*ugv*xCoeff,adt*vgv*yCoeff,adt*wgv*zCoeff)
  addCoeff1(cmpu,eqnq,coeff, bDt*q0x*iCoeff)  ! u*q0x 
  addCoeff1(cmpv,eqnq,coeff, bDt*q0y*iCoeff)  ! v*q0y 
  addCoeff1(cmpw,eqnq,coeff, bDt*q0z*iCoeff)  ! w*q0z 

 #End

endLoops()
end if
#endMacro





#defineMacro divNuGradu2d(u) ( \
     ( au11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - au11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )\
   + ( au22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - au22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )\
   + (au12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-au12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))\
   + (au21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-au21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))\
          \
   + ( av11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - av11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )\
   + ( av22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - av22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )\
   + (av12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-av12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))\
   + (av21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-av21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))\
     )
#defineMacro divNuGradv2d(u) ( \
     ( bu11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - bu11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )\
   + ( bu22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - bu22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )\
   + (bu12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-bu12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))\
   + (bu21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-bu21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))\
          \
   + ( bv11ph*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc)) - bv11mh*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc)) )\
   + ( bv22ph*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc)) - bv22mh*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc)) )\
   + (bv12pzz*(u(i1+1,i2+1,i3,vc)-u(i1+1,i2-1,i3,vc))-bv12mzz*(u(i1-1,i2+1,i3,vc)-u(i1-1,i2-1,i3,vc)))\
   + (bv21zpz*(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc))-bv21zmz*(u(i1+1,i2-1,i3,vc)-u(i1-1,i2-1,i3,vc)))\
       )

#defineMacro divNuGradu3dTerm(a,u,uc)\
     ( a ## 11ph*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc)) - a ## 11mh*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc)) )\
   + ( a ## 22ph*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc)) - a ## 22mh*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc)) )\
   + ( a ## 33ph*(u(i1,i2,i3+1,uc)-u(i1,i2,i3,uc)) - a ## 33mh*(u(i1,i2,i3,uc)-u(i1,i2,i3-1,uc)) )\
   + (a ## 12pzz*(u(i1+1,i2+1,i3,uc)-u(i1+1,i2-1,i3,uc))-a ## 12mzz*(u(i1-1,i2+1,i3,uc)-u(i1-1,i2-1,i3,uc)))\
   + (a ## 13pzz*(u(i1+1,i2,i3+1,uc)-u(i1+1,i2,i3-1,uc))-a ## 13mzz*(u(i1-1,i2,i3+1,uc)-u(i1-1,i2,i3-1,uc)))\
   + (a ## 21zpz*(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc))-a ## 21zmz*(u(i1+1,i2-1,i3,uc)-u(i1-1,i2-1,i3,uc)))\
   + (a ## 23zpz*(u(i1,i2+1,i3+1,uc)-u(i1,i2+1,i3-1,uc))-a ## 23zmz*(u(i1,i2-1,i3+1,uc)-u(i1,i2-1,i3-1,uc)))\
   + (a ## 31zzp*(u(i1+1,i2,i3+1,uc)-u(i1-1,i2,i3+1,uc))-a ## 31zzm*(u(i1+1,i2,i3-1,uc)-u(i1-1,i2,i3-1,uc)))\
   + (a ## 32zzp*(u(i1,i2+1,i3+1,uc)-u(i1,i2-1,i3+1,uc))-a ## 32zzm*(u(i1,i2+1,i3-1,uc)-u(i1,i2-1,i3-1,uc)))


#defineMacro divNuGradu3d(au,av,aw,u) (divNuGradu3dTerm(au,u,uc)+divNuGradu3dTerm(av,u,vc)+divNuGradu3dTerm(aw,u,wc))


! ===================================================================================
! Macro to evaluate the RHS for the INS equations
! ===================================================================================
#beginMacro assignRHSPDE()
if( evalRightHandSide.eq.1 .or. evalResidual.eq.1 )then

! **** to do : optimize this for backward-Euler : fe=0, fi=0 !!

! NOTE: For moving grid problems we must eval the RHS as some mask==0 (exposed) points
beginLoopsNoMask()
  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,1)

  ! evaluate forward derivatives of the current solution: 

  ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
  ! MAXDER = max number of parametric derivatives to precompute.
  opEvalParametricDerivative(u,uc,uu,1)
  ! Evaluate the spatial derivatives of u:
  getOp(x, u,uc,uu,aj,u0x)       ! u.x
  getOp(y, u,uc,uu,aj,u0y)       ! u.y

  ! getOp(xx, u,uc,uu,aj,u0xx)       ! u.xx
  ! getOp(yy, u,uc,uu,aj,u0yy)       ! u.yy

  ! Evaluate the spatial derivatives of v:
  opEvalParametricDerivative(u,vc,vv,1)
  getOp(x, u,vc,vv,aj,v0x)       ! v.x
  getOp(y, u,vc,vv,aj,v0y)       ! v.y 

  ! getOp(xx, u,vc,vv,aj,v0xx)       ! v.xx
  ! getOp(yy, u,vc,vv,aj,v0yy)       ! v.yy

  ! Evaluate the spatial derivatives of p:
  opEvalParametricDerivative(u,pc,pp,1)
  getOp(x, u,pc,pp,aj,p0x)       ! p.x
  getOp(y, u,pc,pp,aj,p0y)       ! p.y 

  ! Evaluate the spatial derivatives of q:
  opEvalParametricDerivative(u,qc,qq,2)
  getOp(x, u,qc,qq,aj,q0x)       ! q.x
  getOp(y, u,qc,qq,aj,q0y)       ! q.y 
  getOp(xx, u,qc,qq,aj,q0xx)       ! q.xx
  getOp(yy, u,qc,qq,aj,q0yy)       ! q.yy

 #If $DIM == 3

  getOp(z, u,uc,uu,aj,u0z)       ! u.z
  getOp(z, u,vc,vv,aj,v0z)       ! v.z
  getOp(z, u,pc,pp,aj,p0z)       ! p.z
  getOp(z, u,qc,qq,aj,q0z)       ! q.z

  ! getOp(zz, u,uc,uu,aj,u0zz)       ! u.zz
  ! getOp(zz, u,vc,vv,aj,v0zz)       ! v.zz
  getOp(zz, u,qc,qq,aj,q0zz)       ! q.zz

  opEvalParametricDerivative(u,wc,ww,1)
  getOp(x, u,wc,ww,aj,w0x)       ! w.x
  getOp(y, u,wc,ww,aj,w0y)       ! w.y
  getOp(z, u,wc,ww,aj,w0z)       ! w.z

  ! getOp(xx, u,wc,ww,aj,w0xx)       ! w.xx
  ! getOp(yy, u,wc,ww,aj,w0yy)       ! w.yy
  ! getOp(zz, u,wc,ww,aj,w0zz)       ! w.zz

 #End

 if( evalLinearizedDerivatives.eq.1 )then

  #If $DIM == 2
   opEvalParametricDerivative(ul,uc,uul,1)
   getOp(x, ul,uc,uul,aj,ulx)       ! ul.x
   getOp(y, ul,uc,uul,aj,uly)       ! ul.y
 
   opEvalParametricDerivative(ul,vc,vvl,1)
   getOp(x, ul,vc,vvl,aj,vlx)       ! vl.x
   getOp(y, ul,vc,vvl,aj,vly)       ! vl.y
  #Else
   opEvalParametricDerivative(ul,uc,uul,1)
   getOp(x, ul,uc,uul,aj,ulx)       ! ul.x
   getOp(y, ul,uc,uul,aj,uly)       ! ul.y
   getOp(z, ul,uc,uul,aj,ulz)       ! ul.y
   opEvalParametricDerivative(ul,vc,vvl,1)
   getOp(x, ul,vc,vvl,aj,vlx)       ! vl.x
   getOp(y, ul,vc,vvl,aj,vly)       ! vl.y
   getOp(z, ul,vc,vvl,aj,vlz)       ! vl.y
   opEvalParametricDerivative(ul,wc,wwl,1)
   getOp(x, ul,wc,wwl,aj,wlx)       ! wl.x
   getOp(y, ul,wc,wwl,aj,wly)       ! wl.y
   getOp(z, ul,wc,wwl,aj,wlz)       ! wl.y
  #End
 end if

 ! get coefficients scaled by 1.
 getDivTensorGradCoefficientsINSVP(u,1.)


 ! compute the relative velocity :  ugv = u-gridVelocity
 if( gridIsMoving.ne.0 )then
   ugv = advectionCoefficient*uu - gv(i1,i2,i3,0)
   vgv = advectionCoefficient*vv - gv(i1,i2,i3,1)
  #If $DIM == 3
   wgv = advectionCoefficient*ww - gv(i1,i3,i3,2)
  #End
 else
   ugv = advectionCoefficient*uu
   vgv = advectionCoefficient*vv
  #If $DIM == 3
   wgv = advectionCoefficient*ww
  #End
 end if

 getArtificialDissipationCoeff(adCoeff, u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z )

 #If $DIM == 2

 ! ---------------- 2D RHS and Residual ---------------------------
 divNuGradu = divNuGradu2d(u)
 divNuGradv = divNuGradv2d(u)

 if( evalRightHandSide.eq.1 )then

  getArtificialDissipationCoeff(adCoeffl, ulx,uly,ulz, vlx,vly,vlz, wlx,wly,wlz )

  if( gridIsImplicit.eq.0 )then
    ! explicit

   fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y - p0x -thermalExpansivity*gravity(0)*qq + divNuGradu + adCoeff*uDiss22(u,uc) 
   fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y - p0y -thermalExpansivity*gravity(1)*qq + divNuGradv + adCoeff*uDiss22(u,vc) 
   fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y + kThermal*(q0xx+q0yy)

  else 

    ! implicit method -- compute explicit part
    !     fe = f(u) - L(u,ul)

    ! evaluate div( nu(ul).grad ) u
    getDivTensorGradCoefficientsINSVP(ul,1.)
    divNuGradul = divNuGradu2d(u)
    divNuGradvl = divNuGradv2d(u)

    fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y - p0x -thermalExpansivity*gravity(0)*qq + divNuGradu - divNuGradul \
                           + (adCoeff-adCoeffl)*uDiss22(u,uc)
    fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y - p0y -thermalExpansivity*gravity(1)*qq + divNuGradv - divNuGradvl \
                           + (adCoeff-adCoeffl)*uDiss22(u,vc)
    fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y 

    if( nonlinearTermsAreImplicit.eq.1 )then
      ! include linearized terms u0*ulx + ul*u0x 

      opEvalParametricDerivative(ul,qc,qql,1)
      getOp(x, ul,qc,qql,aj,qlx)       ! ql.x
      getOp(y, ul,qc,qql,aj,qly)       ! ql.y

      ! compute the linearized relative velocity:  ugv = u-gridVelocity 
      if( gridIsMoving.ne.0 )then
       ugvl = advectionCoefficient*uul - gvl(i1,i2,i3,0)
       vgvl = advectionCoefficient*vvl - gvl(i1,i2,i3,1)
      else
       ugvl = advectionCoefficient*uul
       vgvl = advectionCoefficient*vvl
      end if

      ! ulterm = uul*u0x + vvl*u0y + bImp*(uu*ulx +  vv*uly)

      ulterm = ugvl*u0x + vgvl*u0y + bImp*(uu*ulx +  vv*uly)
      vlterm = ugvl*v0x + vgvl*v0y + bImp*(uu*vlx +  vv*vly)
      qlterm = ugvl*q0x + vgvl*q0y + bImp*(uu*qlx +  vv*qly)

      fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + ulterm
      fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + vlterm
      fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + qlterm
    end if

    if( implicitOption.eq.computeImplicitTermsSeparately )then
      ! implicit method -- compute implicit part 
      fi(i1,i2,i3,uc) = divNuGradul+ adCoeffl*uDiss22(u,uc) 
      fi(i1,i2,i3,vc) = divNuGradvl+ adCoeffl*uDiss22(u,vc) 
      fi(i1,i2,i3,qc) = kThermal*(q0xx+q0yy)
      if( nonlinearTermsAreImplicit.eq.1 )then
        ! include linearized terms u0*ulx + ul*u0x 
        fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) - aImp*( ulterm )
        fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) - aImp*( vlterm )
        fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) - aImp*( qlterm )
      end if
    end if
  end if
 end if
 if( evalResidual.eq.1 )then
   ! residual in 2D: (NOTE: currently ul is not available when evaluating the residual)

   fe(i1,i2,i3,uc) = fi(i1,i2,i3,uc) - ugv*u0x -vgv*u0y - p0x -thermalExpansivity*gravity(0)*qq + divNuGradu \
                     + adCoeff*uDiss22(u,uc) 
   fe(i1,i2,i3,vc) = fi(i1,i2,i3,vc) - ugv*v0x -vgv*v0y - p0y -thermalExpansivity*gravity(1)*qq + divNuGradv \
                     + adCoeff*uDiss22(u,vc) 
   fe(i1,i2,i3,qc) = fi(i1,i2,i3,qc) - ugv*q0x -vgv*q0y  +kThermal*(q0xx+q0yy)
   ! write(*,'("i=",2i3," divNuGradu,divNuGradu=",f6.2,2x,f6.2)') i1,i2,divNuGradu,divNuGradv
 end if

 #Else

 ! ---------------- 3D RHS and Residual ---------------------------

 divNuGradu = divNuGradu3d(au,av,aw,u)
 divNuGradv = divNuGradu3d(bu,bv,bw,u)
 divNuGradw = divNuGradu3d(cu,cv,cw,u)

  if( evalRightHandSide.eq.1 )then

   getArtificialDissipationCoeff(adCoeffl, ulx,uly,ulz, vlx,vly,vlz, wlx,wly,wlz )

   if( gridIsImplicit.eq.0 )then
     ! explicit
     fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y -wgv*u0z - p0x -thermalExpansivity*gravity(0)*qq\
              + divNuGradu + adCoeff*uDiss23(u,uc)
     fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y -wgv*v0z - p0y -thermalExpansivity*gravity(1)*qq\
              + divNuGradv + adCoeff*uDiss23(u,vc)
     fe(i1,i2,i3,wc) = -ugv*w0x -vgv*w0y -wgv*w0z - p0z -thermalExpansivity*gravity(2)*qq\
              + divNuGradw + adCoeff*uDiss23(u,wc)
     fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y -wgv*q0z + kThermal*(q0xx+q0yy+q0zz)
   else 
    ! implicit method -- compute explicit part
    !     fe = f(u) - L(u,ul)

    ! evaluate div( nu(ul).grad ) u
    getDivTensorGradCoefficientsINSVP(ul,1.)
    divNuGradul = divNuGradu3d(au,av,aw,u)
    divNuGradvl = divNuGradu3d(bu,bv,bw,u)
    divNuGradwl = divNuGradu3d(cu,cv,cw,u)

     fe(i1,i2,i3,uc) = -ugv*u0x -vgv*u0y -wgv*u0z - p0x -thermalExpansivity*gravity(0)*qq \
                       + divNuGradu - divNuGradul + (adCoeff-adCoeffl)*uDiss23(u,uc)
     fe(i1,i2,i3,vc) = -ugv*v0x -vgv*v0y -wgv*v0z - p0y -thermalExpansivity*gravity(1)*qq \
                       + divNuGradv - divNuGradvl + (adCoeff-adCoeffl)*uDiss23(u,vc)
     fe(i1,i2,i3,wc) = -ugv*w0x -vgv*w0y -wgv*w0z - p0z -thermalExpansivity*gravity(2)*qq \
                       + divNuGradw - divNuGradwl + (adCoeff-adCoeffl)*uDiss23(u,wc)
     fe(i1,i2,i3,qc) = -ugv*q0x -vgv*q0y -wgv*q0z 

     if( nonlinearTermsAreImplicit.eq.1 )then
       ! include linearized terms u0*ulx + ul*u0x + ...

       opEvalParametricDerivative(ul,qc,qql,1)
       getOp(x, ul,qc,qql,aj,qlx)       ! ql.x
       getOp(y, ul,qc,qql,aj,qly)       ! ql.y
       getOp(z, ul,qc,qql,aj,qlz)       ! ql.z
 
      ! compute the linearized relative velocity:  ugv = u-gridVelocity 
      if( gridIsMoving.ne.0 )then
       ugvl = advectionCoefficient*uul - gvl(i1,i2,i3,0)
       vgvl = advectionCoefficient*vvl - gvl(i1,i2,i3,1)
       wgvl = advectionCoefficient*wwl - gvl(i1,i2,i3,2)
      else
       ugvl = advectionCoefficient*uul
       vgvl = advectionCoefficient*vvl
       wgvl = advectionCoefficient*wwl
      end if

       ulterm = ugvl*u0x + vgvl*u0y + wgvl*u0z + bImp*(uu*ulx + vv*uly+ ww*ulz)
       vlterm = ugvl*v0x + vgvl*v0y + wgvl*v0z + bImp*(uu*vlx + vv*vly+ ww*vlz)
       wlterm = ugvl*w0x + vgvl*w0y + wgvl*w0z + bImp*(uu*wlx + vv*wly+ ww*wlz)
       qlterm = ugvl*q0x + vgvl*q0y + wgvl*q0z + bImp*(uu*qlx + vv*qly+ ww*qlz)
  
       fe(i1,i2,i3,uc) = fe(i1,i2,i3,uc) + ulterm
       fe(i1,i2,i3,vc) = fe(i1,i2,i3,vc) + vlterm
       fe(i1,i2,i3,wc) = fe(i1,i2,i3,wc) + wlterm
       fe(i1,i2,i3,qc) = fe(i1,i2,i3,qc) + qlterm
     end if
     if( implicitOption.eq.computeImplicitTermsSeparately )then
       ! implicit method -- compute implicit part 
       fi(i1,i2,i3,uc) = divNuGradul + adCoeffl*uDiss23(u,uc)
       fi(i1,i2,i3,vc) = divNuGradvl + adCoeffl*uDiss23(u,vc)
       fi(i1,i2,i3,wc) = divNuGradwl + adCoeffl*uDiss23(u,wc)
       fi(i1,i2,i3,qc) = kThermal*(q0xx+q0yy+q0zz)
       if( nonlinearTermsAreImplicit.eq.1 )then
         ! include linearized terms u0*ulx + ul*u0x 
         fi(i1,i2,i3,uc) = fi(i1,i2,i3,uc) - aImp*( ulterm )
         fi(i1,i2,i3,vc) = fi(i1,i2,i3,vc) - aImp*( vlterm )
         fi(i1,i2,i3,wc) = fi(i1,i2,i3,wc) - aImp*( wlterm )
         fi(i1,i2,i3,qc) = fi(i1,i2,i3,qc) - aImp*( qlterm )
       end if
     end if
   end if
  end if
  if( evalResidual.eq.1 )then
    ! residual in 3D: (NOTE: currently ul is not available when evaluating the residual)
    fe(i1,i2,i3,uc) = fi(i1,i2,i3,uc) -ugv*u0x -vgv*u0y -wgv*u0z - p0x -thermalExpansivity*gravity(0)*qq \
                    + divNuGradu + adCoeff*uDiss23(u,uc)
    fe(i1,i2,i3,vc) = fi(i1,i2,i3,vc) -ugv*v0x -vgv*v0y -wgv*v0z - p0y -thermalExpansivity*gravity(1)*qq \
                    + divNuGradv + adCoeff*uDiss23(u,vc)
    fe(i1,i2,i3,wc) = fi(i1,i2,i3,wc) -ugv*w0x -vgv*w0y -wgv*w0z - p0z -thermalExpansivity*gravity(2)*qq \
                    + divNuGradw + adCoeff*uDiss23(u,wc)
    fe(i1,i2,i3,qc) = fi(i1,i2,i3,qc) -ugv*q0x -vgv*q0y -wgv*q0z +kThermal*(q0xx+q0yy+q0zz)

!write(*,'(" i=",3i3," fi,divNuGradu,fe=",3e10.2)') i1,i2,i3,fi(i1,i2,i3,uc),divNuGradu,fe(i1,i2,i3,uc)
  end if

 #End
endLoopsNoMask()
end if
#endMacro


! ===============================================================================================
!  Fill in the matrix BCs on a face 
! ===============================================================================================
#beginMacro fillMatrixBoundaryConditionsPDE()

if( fillCoefficients.eq.1 )then
if( bc(side,axis).eq.dirichletBoundaryCondition .or.\
    bc(side,axis).eq.noSlipWall.or.\
    bc(side,axis).eq.inflowWithVelocityGiven.or.\
    bc(side,axis).eq.interfaceBoundaryCondition )then
  
 ! Dirichlet BC
 beginLoopsMixedBoundary()

 ! zero out equations for u,v,w: (We cannot zero the T equation if there is a mixed BC)
  zeroMatrixCoefficients( coeff,eqnu,eqnu+nd-1,i1,i2,i3 )

  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
!   opEvalJacobianDerivatives(aj,1)

  ! evaluate the coeff operators 
  getCoeff(identity, iCoeff,aj)
!   getCoeff(x, xCoeff,aj)
!   getCoeff(y, yCoeff,aj)
  #If $DIM == 3
!    getCoeff(z, zCoeff,aj)
  #End

  do n=0,nd-1
   setCoeff5(cmpu+n,eqnu+n,coeff,iCoeff,,,,)
  end do 
  ! T is done below 
  ! ********* fix for mixed *************
  ! setCoeff5(cmpq,eqnq,coeff,iCoeff,,,,)

 endLoops()


else if( bc(side,axis).eq.outflow )then

 if( outflowOption.eq.1 )then
  ! Neumann BC at outflow if outflowOption==1
  beginLoopsMixedBoundary()
   getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
   do n=0,nd-1
     fillMatrixNeumann(coeff, cmpu+n,eqnu+n, i1,i2,i3, an,0.,1. )
   end do
  endLoops()
 end if

else if( bc(side,axis).eq.slipWall )then

  ! SLIP-WALL

  ! NOTE: Here we assume the matrix already includes the interior equations on the boundary 

  ! NOTE: what about corners ???

  ! boundary values use:
  !    n.u = f
  !    tau.(Lu) = tau.g   (tangential component of the equations on the boundary
  ! To avoid a zero pivot we combine the above equations as
  !     (n.u) n + ( tau.(Lu) ) tau = n f + tau g 
  !
  ! OR:  ( tau.(Lu) ) tau = Lu - (n.(Lu)) n 
  !    (n.u) n + Lu - (n.(Lu)) n = n f +  g - (n.g) n 
  !       


 getCoeff(identity, iCoeff,aj)

 beginLoopsMixedBoundary()

  getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
  beginMatrixLoops(m1,m2,m3)

  ! Form the matrix for "n.u"  -->  nDot(mc3(m1,m2,m3,c)) = iCoeff(m1,m2,m3)*an(c-cmpu)
  !nDot=0
  !nDot(mc3(0,0,0,cmpu))=an(0)
  !nDot(mc3(0,0,0,cmpv))=an(1)
  !nDot(mc3(0,0,0,cmpw))=an(2)

  ! Form the matrix for "n.Lu"
#If $DIM == 2 
  nDotL(0)=an(0)*coeff(mce2(m1,m2,m3,cmpu,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce2(m1,m2,m3,cmpu,eqnv),i1,i2,i3)
  nDotL(1)=an(0)*coeff(mce2(m1,m2,m3,cmpv,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce2(m1,m2,m3,cmpv,eqnv),i1,i2,i3)
#Else
  nDotL(0)=an(0)*coeff(mce3(m1,m2,m3,cmpu,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce3(m1,m2,m3,cmpu,eqnv),i1,i2,i3)+\
           an(2)*coeff(mce3(m1,m2,m3,cmpu,eqnw),i1,i2,i3)
  nDotL(1)=an(0)*coeff(mce3(m1,m2,m3,cmpv,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce3(m1,m2,m3,cmpv,eqnv),i1,i2,i3)+\
           an(2)*coeff(mce3(m1,m2,m3,cmpv,eqnw),i1,i2,i3)
  nDotL(2)=an(0)*coeff(mce3(m1,m2,m3,cmpw,eqnu),i1,i2,i3)+\
           an(1)*coeff(mce3(m1,m2,m3,cmpw,eqnv),i1,i2,i3)+\
           an(2)*coeff(mce3(m1,m2,m3,cmpw,eqnw),i1,i2,i3)
#End

  ! form the matrix for  Lu + [ (n.u) - (n.(Lu)) ] n 

  !  eqnu:  (n1*u1+n2*u2+n3*u3)*n1 + L1(u) - nDotL*n1    
  !  eqnv:  (n1*u1+n2*u2+n3*u3)*n2 + L2(u) - nDotL*n2
  !  eqnw:  (n1*u1+n2*u2+n3*u3)*n3 + L3(u) - nDotL*n3
  !  nDotL = L1(u)*n1 + L2(u)*n2 + L3(u)*n3 
  do e=eqnu,eqnu+nd-1
  do c=cmpu,cmpu+nd-1
   coeff(MCE(m1,m2,m3,c,e),i1,i2,i3)= coeff(MCE(m1,m2,m3,c,e),i1,i2,i3) + \
       an(e-eqnu)*(iCoeff(MA(m1,m2,m3))*an(c-cmpu)-nDotL(c-cmpu))
  end do
  end do

 endMatrixLoops()

 ! fill ghost pt eqns with a vector symmetry condition:
 fillMatrixVectorSymmetry(coeff, cmpu,eqnu, i1,i2,i3, an )

 ! T is done below
 ! ************* fix for mixed ****************
 ! fillMatrixNeumann(coeff, cmpq,eqnq, i1,i2,i3, an, 0.,1. )

 endLoops()


else if( bc(side,axis).gt.0 )then
  write(*,'("insimp:BC: ERROR unknown bc=",i4)') bc(side,axis)
end if

 orderOfExtrap=orderOfExtrapolation
 if( bc(side,axis).eq.outflow .and. orderOfExtrapolationForOutflow.gt.0 )then
   orderOfExtrap=orderOfExtrapolationForOutflow
 end if
 if( orderOfExtrap.lt.1 .or. orderOfExtrap.gt.maxOrderOfExtrapolation )then
  write(*,'("insimp:BC:ERROR: requesting orderOfExtrap=",i6)') orderOfExtrap
  stop 5502
 end if

if( bc(side,axis).eq.dirichletBoundaryCondition .or.\
    bc(side,axis).eq.noSlipWall.or.\
    bc(side,axis).eq.inflowWithVelocityGiven .or.\
    (bc(side,axis).eq.outflow .and. outflowOption.eq.0) .or.\
    bc(side,axis).eq.interfaceBoundaryCondition )then

 ! === extrapolation ===

 beginLoopsMixedBoundary()

  do n=0,nd-1
   c=cmpu+n
   e=eqnu+n
   fillMatrixExtrapolation(coeff,c,e,i1,i2,i3,orderOfExtrap,1)
   !* fillMatrixExtrapolation(coeff,c,e,i1,i2,i3,orderOfExtrap,2)
  end do

  !* fillMatrixExtrapolation(coeff,cmpq,eqnq,i1,i2,i3,orderOfExtrap,1)
  !* fillMatrixExtrapolation(coeff,cmpq,eqnq,i1,i2,i3,orderOfExtrap,2)

 endLoops()
end if


 ! --- Assign the BCs for T ---
 a0 = mixedCoeff(tc,side,axis,grid)
 a1 = mixedNormalCoeff(tc,side,axis,grid)
 ! write(*,'(" insimpvp: T BC: (a0,a1)=(",f3.1,",",f3.1,") for side,axis,grid=",3i3)') a0,a1,side,axis,grid
 if( bc(side,axis).eq.dirichletBoundaryCondition .or.\
     bc(side,axis).eq.noSlipWall.or.\
     bc(side,axis).eq.slipWall.or.\
     bc(side,axis).eq.inflowWithVelocityGiven.or.\
     bc(side,axis).eq.outflow.or.\
     bc(side,axis).eq.interfaceBoundaryCondition )then

   if( bc(side,axis).eq.outflow )then
    ! outflow is Neumann
    a0=0.
    a1=1.
   end if

  if( a1.ne.0. )then
   ! Mixed BC 
   beginLoopsMixedBoundary()
    getNormalForCurvilinearGrid(side,axis,i1,i2,i3)
    fillMatrixNeumann(coeff, cmpq,eqnq, i1,i2,i3, an,a0,a1 )
   endLoops()
  else
   ! Dirichlet + extrap ghost line values


   getCoeff(identity, iCoeff,aj)
   beginLoopsMixedBoundary()
    zeroMatrixCoefficients( coeff,eqnq,eqnq,i1,i2,i3 )
    setCoeff5(cmpq,eqnq,coeff,a0*iCoeff,,,,)
    fillMatrixExtrapolation(coeff,cmpq,eqnq,i1,i2,i3,orderOfExtrap,1)
   endLoops()
  end if

!  else if( bc(side,axis).eq.outflow )then
!    ! === extrapolation ===
!   beginLoopsMixedBoundary()
!    fillMatrixExtrapolation(coeff,cmpq,eqnq,i1,i2,i3,orderOfExtrap,1)
!   endLoops()

 else if( bc(side,axis).gt.0 )then
  write(*,'("insimpvp:BC:T: ERROR unknown bc=",i4)') bc(side,axis) 
  stop 9167
 end if 

end if ! end if( fillCoefficients.eq.1 )
#endMacro


! ===============================================================================================
!  Compute the residual for BCs
! ===============================================================================================
#beginMacro getBoundaryResidualPDE()
if( evalResidualForBoundaryConditions.eq.1 )then

if( bc(side,axis).eq.dirichletBoundaryCondition .or.\
    bc(side,axis).eq.noSlipWall.or.\
    bc(side,axis).eq.inflowWithVelocityGiven.or.\
    bc(side,axis).eq.interfaceBoundaryCondition )then
  
 ! Dirichlet BC
 beginLoops()

  do n=0,nd-1
   ! fe(i1,i2,i3,uc+n)=0.  
   fe(i1,i2,i3,uc+n)=fi(i1,i2,i3,uc+n)-u(i1,i2,i3,uc+n)
  end do 

  ! ********* fix for mixed *************
  fe(i1,i2,i3,tc)=fi(i1,i2,i3,tc)-u(i1,i2,i3,tc)

 endLoops()

else if( bc(side,axis).eq.outflow )then


else if( bc(side,axis).eq.slipWall )then
  ! SLIP-WALL
 
  ! boundary values use:
  !    n.u = f
  !    tau.(Lu) = tau.g   (tangential component of the equations on the boundary
  ! To avoid a zero pivot we combine the above equations as
  !     (n.u) n + ( tau.(Lu) ) tau = n f + tau g 
  !
  ! OR:  ( tau.(Lu) ) tau = Lu - (n.(Lu)) n 
  !    (n.u) n + Lu - (n.(Lu)) n = n f +  g - (n.g) n 
  !       

 beginLoops()

  do n=0,nd-1
   fe(i1,i2,i3,uc+n)=0.  ! *** do this for now 
  end do 

 endLoops()

else if( bc(side,axis).gt.0 )then

  write(*,'("insimp:residual:BC: ERROR unknown bc=",i4)') bc(side,axis)

end if

end if ! end if( evalResidualForBoundaryConditions.eq.1 )
#endMacro



! ******************* Here we now define the subroutine insImpVP ****************************
INSIMP(insImpVP,INSVP)

