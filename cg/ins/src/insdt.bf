!
! Compute du/dt for the incompressible NS on rectangular AND curvilinear grids
!


! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
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


! ====================================================
! SOLVER: INS, SPAL, KE
! ====================================================
#beginMacro insdtFunctions(SOLVER)
 if( orderOfAccuracy.eq.2 )then
  if( nd.eq.2 )then
    call insdt ## SOLVER ## 2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
  else 
    call insdt ## SOLVER ## 3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
  end if
! #If #SOLVER ne "VP" && #SOLVER ne "VD"
#If #SOLVER ne "VD"
 else if( orderOfAccuracy.eq.4 )then
  if( nd.eq.2 )then
    call insdt ## SOLVER ## 2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
  else 
    call insdt ## SOLVER ## 3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
  end if
#End
 else
   stop 1111
 end if
#endMacro


! ==========================================================
!  Advect a passive scalar -- kernel
! ==========================================================
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

! ==============================================================
!  Advect a passive scalar -- build loops for different cases:
!     DIM,ORDER,GRIDTYPE
! ==============================================================
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

! =============================================================================
! Evaluate the variable viscosity and its derivatives
! ============================================================================
#beginMacro getVariableViscosity(DIM)
 nuT = u(i1,i2,i3,vsc)
 nuTx=UX(vsc)
 nuTy=UY(vsc)
 #If #DIM == "3" 
  nuTz=UZ(vsc)
 #End
#endMacro

! ================================================================================
! Compute the first derivatives of the thermal conductivity
!
! DIM : 2 or 3 (number of space dimensions)
! ORDER : 2 or 4 (order of accuracy)
! GRIDTYPE : rectangular or curvilinear
! ================================================================================
#beginMacro getThermalConductivityDerivatives(K,DIM,ORDER,GRIDTYPE)

 ! --- For now we only do second-order accurate---
 #If #GRIDTYPE eq "rectangular"
  ! --- rectangular grid ---
  #If #ORDER eq "2"
    Kx = (K(i1+1,i2,i3)-K(i1-1,i2,i3))/(2.*dx(0))
    Ky = (K(i1,i2+1,i3)-K(i1,i2-1,i3))/(2.*dx(1))
    #If #DIM == "3" 
      Kz = (K(i1,i2,i3+1)-K(i1,i2,i3-1))/(2.*dx(2))
    #End
  #Elif #ORDER eq "4"
    Kx = (8.*(K(i1+1,i2,i3)-K(i1-1,i2,i3))-(K(i1+2,i2,i3)-K(i1-2,i2,i3)))*h41(0)
    Ky = (8.*(K(i1,i2+1,i3)-K(i1,i2-1,i3))-(K(i1,i2+2,i3)-K(i1,i2-2,i3)))*h41(1)
    #If #DIM == "3" 
      Kz = (8.*(K(i1,i2,i3+1)-K(i1,i2,i3-1))-(K(i1,i2,i3+2)-K(i1,i2,i3-2)))*h41(2)
    #End
  #Else
    stop 2003
  #End

 #Elif #GRIDTYPE eq "curvilinear"
    ! --- curvilinear grid ---
    #If #ORDER eq "2"
      Kr = (K(i1+1,i2,i3)-K(i1-1,i2,i3))/(2.*dr(0))
      Ks = (K(i1,i2+1,i3)-K(i1,i2-1,i3))/(2.*dr(1))
      #If #DIM == "3"
        Kt = (K(i1,i2,i3+1)-K(i1,i2,i3-1))/(2.*dr(2))
      #End
    #Elif #ORDER eq "4"
      Kr = (8.*(K(i1+1,i2,i3)-K(i1-1,i2,i3))-(K(i1+2,i2,i3)-K(i1-2,i2,i3)))*d14(0)
      Ks = (8.*(K(i1,i2+1,i3)-K(i1,i2-1,i3))-(K(i1,i2+2,i3)-K(i1,i2-2,i3)))*d14(1)
      #If #DIM == "3"
        Kt = (8.*(K(i1,i2,i3+1)-K(i1,i2,i3-1))-(K(i1,i2,i3+2)-K(i1,i2,i3-2)))*d14(2)
      #End
    #Else
      stop 2005
    #End

    #If #DIM == "2"
      Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks
      Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks
    #Elif #DIM == "3" 
      Kx = rx(i1,i2,i3)*Kr+sx(i1,i2,i3)*Ks+tx(i1,i2,i3)*Kt
      Ky = ry(i1,i2,i3)*Kr+sy(i1,i2,i3)*Ks+ty(i1,i2,i3)*Kt
      Kz = rz(i1,i2,i3)*Kr+sz(i1,i2,i3)*Ks+tz(i1,i2,i3)*Kt
    #Else
      stop 2006
    #End

 #Else
   stop 1007
 #End
#endMacro



! ==========================================================
!  Boussinseq approximation -- kernel
!
!  Add the Boussinseq (buoyancy) term to the momentum equations and
!  evaluate the Temperature equation.
!
! DIM : 2 or 3 (number of space dimensions)
! ORDER : 2 or 4 (order of accuracy)
! GRIDTYPE : rectangular or curvilinear
! IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
! TMODEL :  NONE, LES (turbulence model)
! VARMAT : CONST, PIECEWISE (piece-wise constant) , VAR (variable material properties)
! ==========================================================
#beginMacro boussinesqKernel(DIM,ORDER,GRIDTYPE,IMPEXP,TMODEL,VARMAT)

 defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

 ! Scaling factor for nuT for turbulent diffusivity: (We could also use the Prandtl number here)
 kThermalLES = kThermal/nu 

 #If #TMODEL == "LES"
  if( t.le.0 )then
    write(*,'(" --- insdt:evaluate LES with variable diffusivity Temperature equation t=",e10.2,"---")') t
  end if
 #End

 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
    ! add on gravity terms to the momentum equations:
    ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)-gravity(0)*thermalExpansivity*u(i1,i2,i3,tc)
    ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)-gravity(1)*thermalExpansivity*u(i1,i2,i3,tc)
    #If #DIM == "3" 
      ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)-gravity(2)*thermalExpansivity*u(i1,i2,i3,tc)
    #End

  ! --- Temperature Equation ---

  ! Define the diffusion term: 
 #If #TMODEL == "NONE"

  #If #VARMAT == "CONST"
    ! constant thermal diffusivity
    #defineMacro TLAP(tc) (kThermal*ULAP(tc))
  #Elif #VARMAT == "PIECEWISE"
    ! Do this for now -- finish me - missing a term - use a conservative approx.  ****************************************************
    ! Kx = (thermalKpc(i1+1,i2,i3)-thermalKpc(i1-1,i2,i3))/(2.*dx(0))
    ! Ky = (thermalKpc(i1,i2+1,i3)-thermalKpc(i1,i2-1,i3))/(2.*dx(1))
    ! Compute Kx, Ky, Kz:
    getThermalConductivityDerivatives(thermalKpc,DIM,ORDER,GRIDTYPE)
    #defineMacro TLAP(tc) ( (thermalKpc(i1,i2,i3)*ULAP(tc) + Kx*UX(tc)+Ky*UY(tc) )/(rhopc(i1,i2,i3)*Cppc(i1,i2,i3)) )
  #Elif #VARMAT == "VAR"
    ! Kx = (thermalKv(i1+1,i2,i3)-thermalKv(i1-1,i2,i3))/(2.*dx(0))
    ! Ky = (thermalKv(i1,i2+1,i3)-thermalKv(i1,i2-1,i3))/(2.*dx(1))

    ! Compute Kx, Ky, Kz:
    getThermalConductivityDerivatives(thermalKv,DIM,ORDER,GRIDTYPE)
    #defineMacro TLAP(tc) ( (thermalKv(i1,i2,i3)*ULAP(tc) + Kx*UX(tc)+Ky*UY(tc) )/(rhov(i1,i2,i3)*Cpv(i1,i2,i3)) )
  #Else
    ! Error -- unknown varmat options
    stop 2505
  #End

 #Elif #TMODEL == "LES"

  ! variable (turbulent) thermal diffusivity
  #If #DIM == "2"
    #defineMacro TLAP(tc) ( (nuT*ULAP(tc) +nuTx*(UX(tc)) +nuTy*(UY(tc)) )*kThermalLES )
  #Else
    #defineMacro TLAP(tc) ( (nuT*ULAP(tc) +nuTx*(UX(tc)) +nuTy*(UY(tc)) +nuTz*(UZ(tc)) )*kThermalLES )
  #End

  ! Evaluate nuT and its derivatives: 
  #If #IMPEXP ne "EXPLICIT_ONLY"
    getVariableViscosity(DIM)
  #End

 #Else
   stop 9099
 #End


#If #IMPEXP == "EXPLICIT"

  #If #DIM == "2"
    ut(i1,i2,i3,tc)= -UU(uc)*UX(tc)-UU(vc)*UY(tc) +TLAP(tc) +adcBoussinesq*delta ## ORDER DIM(tc)
  #Else
    ut(i1,i2,i3,tc)= -UU(uc)*UX(tc)-UU(vc)*UY(tc)-UU(wc)*UZ(tc) +TLAP(tc) +adcBoussinesq*delta ## ORDER DIM(tc)
  #End

  if( isAxisymmetric.eq.1 )then
    ! -- add on axisymmetric corrections ---
    #If #TMODEL ne "NONE"
      ! finish me for axisymmetric and variable (turbulent) thermal diffusivity
      stop 8567
    #End
    #If #VARMAT ne "CONST"
      ! finish me for axisymmetric and variable material properties
      stop 8568
    #End

    ri=radiusInverse(i1,i2,i3)
    if( ri.ne.0. )then
      ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( UY(tc)*ri )
    else
      ut(i1,i2,i3,tc)=ut(i1,i2,i3,tc)+kThermal*( UYY(tc) )
    end if  
  end if

#Elif #IMPEXP == "EXPLICIT_ONLY" || #IMPEXP == "BOTH"

  #If #DIM == "2"
    ut(i1,i2,i3,tc)= -UU(uc)*UX(tc)-UU(vc)*UY(tc)+adcBoussinesq*delta ## ORDER DIM(tc)
  #Else
    ut(i1,i2,i3,tc)= -UU(uc)*UX(tc)-UU(vc)*UY(tc)-UU(wc)*UZ(tc)+adcBoussinesq*delta ## ORDER DIM(tc)
  #End

#End
#If #IMPEXP == "BOTH"
 ! include implicit terms - diffusion

 uti(i1,i2,i3,tc)= TLAP(tc)

 if( isAxisymmetric.eq.1 )then
   #If #VARMAT ne "CONST"
     ! finish me for axisymmetric and variable material properties
     stop 8569
   #End
   ri=radiusInverse(i1,i2,i3)
   if( ri.ne.0. )then
     uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( UY(tc)*ri )
   else
     uti(i1,i2,i3,tc)=uti(i1,i2,i3,tc)+kThermal*( UYY(tc) )
   end if
 end if
#End

  end if
 end do
 end do
 end do

#endMacro

! ==============================================================
!  Boussinesq Model -- build loops for different cases
!
! IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
! TMODEL :  NONE, LES (turbulence model)
! VARMAT : CONST, PIECEWISE (piece-wise constant) , VAR (variable material properties)
!
! ==============================================================
#beginMacro boussinesqLoopsMacro(IMPEXP,TMODEL,VARMAT)

 if( gridType.eq.rectangular )then
  if( orderOfAccuracy.eq.2 )then
    if( nd.eq.2 )then
      boussinesqKernel(2,2,rectangular,IMPEXP,TMODEL,VARMAT)
    else 
      boussinesqKernel(3,2,rectangular,IMPEXP,TMODEL,VARMAT)
    end if
  else if( orderOfAccuracy.eq.4 )then
    if( nd.eq.2 )then
      boussinesqKernel(2,4,rectangular,IMPEXP,TMODEL,VARMAT)
    else 
      boussinesqKernel(3,4,rectangular,IMPEXP,TMODEL,VARMAT)
    end if
  else
   stop 2281
  end if

 else if( gridType.eq.curvilinear )then

  if( orderOfAccuracy.eq.2 )then
    if( nd.eq.2 )then
      boussinesqKernel(2,2,curvilinear,IMPEXP,TMODEL,VARMAT)
    else 
      boussinesqKernel(3,2,curvilinear,IMPEXP,TMODEL,VARMAT)
    end if
  else if( orderOfAccuracy.eq.4 )then
    if( nd.eq.2 )then
      boussinesqKernel(2,4,curvilinear,IMPEXP,TMODEL,VARMAT)
    else 
      boussinesqKernel(3,4,curvilinear,IMPEXP,TMODEL,VARMAT)
    end if
  else
   stop 2282
  end if

 else
   stop 2717
 end if

#endMacro


! ==============================================================
!  Boussinesq Model -- add on gravity terms, eval T equation
!
! TMODEL :  NONE, LES (turbulence model)
! VARMAT : CONST, PIECEWISE (piece-wise constant) , VAR (variable material properties)
! ==============================================================
#beginMacro boussinesqMacro(TMODEL,VARMAT)
 if( gridIsImplicit.eq.0 )then ! explicit
    boussinesqLoopsMacro(EXPLICIT,TMODEL,VARMAT)
 else ! ***** implicit *******
  if( implicitOption .eq.computeImplicitTermsSeparately )then
    boussinesqLoopsMacro(BOTH,TMODEL,VARMAT)
  else if( implicitOption.eq.doNotComputeImplicitTerms )then
    boussinesqLoopsMacro(EXPLICIT_ONLY,TMODEL,VARMAT)
  else
   write(*,*)'insdt:boussinesq Unknown implicitOption=',implicitOption
   stop 4135
  end if  ! end implicitOption

 end if
#endMacro



      subroutine insdt(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                       mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  \
                       ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, ierr )
!======================================================================
!   Compute du/dt for the incompressible NS on rectangular grids
!     OPTIMIZED version for rectangular grids.
! nd : number of space dimensions
!
! gv : gridVelocity for moving grids
! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
! dw : distance to the wall for some turbulence models
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uti(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)
      
      ! -- arrays for variable material properties --
      integer constantMaterialProperties
      integer piecewiseConstantMaterialProperties
      integer variableMaterialProperties
      parameter( constantMaterialProperties=0,
     &           piecewiseConstantMaterialProperties=1,
     &           variableMaterialProperties=2 )
      integer materialFormat,ndMatProp
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

!     ---- local variables -----
      integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
      integer gridIsImplicit,implicitOption,implicitMethod,debug,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD
      integer pc,uc,vc,wc,sc,nc,kc,ec,tc,vsc,grid,m,advectPassiveScalar
      real nu,dt,nuPassiveScalar,adcPassiveScalar,t
      real gravity(0:2), thermalExpansivity, adcBoussinesq,kThermal,kThermalLES
      real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real yy,ri

      integer gridType
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

      integer upwindOrder
      integer advectionOption, centeredAdvection,upwindAdvection,bwenoAdvection
      parameter( centeredAdvection=0, upwindAdvection=1, bwenoAdvection=2 )
      real agu(0:5,0:5) ! for holdings upwind approximations to (a.grad)u

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

      real rhopc,rhov,   Cppc, Cpv, thermalKpc, thermalKv, Kx, Ky, Kz, Kr, Ks, Kt

      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'

      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

!     --- begin statement functions
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

!     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
      defineDifferenceOrder4Components1(u,RX)


!    --- 2nd order 2D artificial diffusion ---
      ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &           +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

!    --- 2nd order 3D artificial diffusion ---
      ad23(c)=adc
     &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) 
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                       
!     ---fourth-order artificial diffusion in 2D
      ad4(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   
     &      -12.*u(i1,i2,i3,c) ) 
!     ---fourth-order artificial diffusion in 3D
      ad43(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  
     &      -18.*u(i1,i2,i3,c) )

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


#Include "selfAdjointArtificialDiffusion.h"

      ! -- statement functions for variable material properties
      ! (rho,Cp,k) for materialFormat=piecewiseConstantMaterialProperties
      rhopc(i1,i2,i3)      = matValpc( 0, matIndex(i1,i2,i3))
      Cppc(i1,i2,i3)       = matValpc( 1, matIndex(i1,i2,i3))
      thermalKpc(i1,i2,i3) = matValpc( 2, matIndex(i1,i2,i3))

      ! (rho,Cp,k) for materialFormat=variableMaterialProperties
      rhov(i1,i2,i3)      = matVal(i1,i2,i3,0)
      Cpv(i1,i2,i3)       = matVal(i1,i2,i3,1)
      thermalKv(i1,i2,i3) = matVal(i1,i2,i3,2)

!     --- end statement functions

      ierr=0
      ! write(*,'("Inside insdt: gridType=",i2)') gridType

      pc                 =ipar(0)
      uc                 =ipar(1)
      vc                 =ipar(2)
      wc                 =ipar(3)
      nc                 =ipar(4)
      sc                 =ipar(5)
      tc                 =ipar(6) 
      grid               =ipar(7)
      orderOfAccuracy    =ipar(8)
      gridIsMoving       =ipar(9)
      useWhereMask       =ipar(10)
      gridIsImplicit     =ipar(11)
      implicitMethod     =ipar(12)
      implicitOption     =ipar(13)
      isAxisymmetric     =ipar(14)
      use2ndOrderAD      =ipar(15)
      use4thOrderAD      =ipar(16)
      advectPassiveScalar=ipar(17)
      gridType           =ipar(18)
      turbulenceModel    =ipar(19)
      pdeModel           =ipar(20) 
      vsc                =ipar(21)
      ! rc               =ipar(22)
      debug              =ipar(23)
      materialFormat     =ipar(24)
      advectionOption    =ipar(25)  ! *new* 2017/01/27
      upwindOrder        =ipar(26)

      dr(0)             =rpar(0)
      dr(1)             =rpar(1)
      dr(2)             =rpar(2)
      dx(0)             =rpar(3)
      dx(1)             =rpar(4)
      dx(2)             =rpar(5)
      nu                =rpar(6)
      ad21              =rpar(7)
      ad22              =rpar(8)
      ad41              =rpar(9)
      ad42              =rpar(10)
      nuPassiveScalar   =rpar(11)
      adcPassiveScalar  =rpar(12)
      ad21n             =rpar(13)
      ad22n             =rpar(14)
      ad41n             =rpar(15)
      ad42n             =rpar(16)

      gravity(0)        =rpar(18)
      gravity(1)        =rpar(19)
      gravity(2)        =rpar(20)
      thermalExpansivity=rpar(21)
      adcBoussinesq     =rpar(22) ! coefficient of artificial diffusion for Boussinesq T equation 
      kThermal          =rpar(23)
      t                 =rpar(24)

      kc=nc
      ec=kc+1

      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("insdt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if
      if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
        write(*,'("insdt:ERROR gridType=",i6)') gridType
        stop 2
      end if
      if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
        write(*,'("insdt:ERROR uc,vc,ws=",3i6)') uc,vc,wc
        stop 4
      end if

!      write(*,'("insdt: turbulenceModel=",2i6)') turbulenceModel
!      write(*,'("insdt: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc

      if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. kc.gt.1000) )then
        write(*,'("insdt:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc
        stop 5
      end if

      if( advectionOption.ne.centeredAdvection .and. t.le.0. )then
        write(*,'(" insdt: advectionOption=",i2," (0=Centered,1=Upwind,2=Bweno)")') advectionOption
        write(*,'(" insdt: upwindOrder=",i2, " (-1=default)")') upwindOrder
      end if
      ! --- Output rho, Cp and kThermal t=0 for testing ---
      if( materialFormat.ne.0 .and. t.le.0 .and. (nd1b-nd1a)*(nd2b-nd2a).lt. 1000 )then

       write(*,'("insdt: variable material properties rho,Cp,kThermal for T")')
       write(*,'("insdt: rho:")')
       i3=nd3a
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,9000) (rhopc(i1,i2,i3),i1=nd1a,nd1b)
         else
          write(*,9000) (rhov(i1,i2,i3),i1=nd1a,nd1b)
         end if
       end do 
       write(*,'("insdt: Cp:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,9000) (Cppc(i1,i2,i3),i1=nd1a,nd1b)
         else
          write(*,9000) (Cpv(i1,i2,i3),i1=nd1a,nd1b)
         end if
       end do 
       write(*,'("insdt: thermalConductivity:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,9000) (thermalKpc(i1,i2,i3),i1=nd1a,nd1b)
         else
          write(*,9000) (thermalKv(i1,i2,i3),i1=nd1a,nd1b)
         end if
       end do 
 9000  format(100(f5.1))

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

       ! write(*,'(" insdt: k-epsilon: nc,kc,ec=",3i3)') nc,kc,ec

        call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )
        !  write(*,'(" insdt: cMu,cEps1,cEps2,sigmaEpsI,sigmaKI=",5f8.3)') cMu,cEps1,cEps2,sigmaEpsI,sigmaKI

      else if( turbulenceModel.eq.largeEddySimulation )then
       
      else if( turbulenceModel.ne.noTurbulenceModel )then
        write(*,'(" insdt:ERROR: turbulenceModel=",i4," not expected")') turbulenceModel
        stop 88
      end if

      adc=adcPassiveScalar ! coefficient of linear artificial diffusion
      cd22=ad22/(nd**2)
      cd42=ad42/(nd**2)

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

!     *********************************      
!     ********MAIN LOOPS***************      
!     *********************************      

      if( (turbulenceModel.eq.noTurbulenceModel .and. pdeModel.eq.viscoPlasticModel) .or. \
           turbulenceModel.eq.largeEddySimulation )then
        ! ins + visco-plastic model, or LES
        if( debug.gt.2 )then
          write(*,'(" insdt: compute du/dt for generic viscosity (VP), t=",e10.2)') t 
        endif
        insdtFunctions(VP)

      else if( turbulenceModel.eq.noTurbulenceModel )then

        insdtFunctions(INS)

      else if( turbulenceModel.eq.spalartAllmaras )then

        insdtFunctions(SPAL)

      else if( turbulenceModel.eq.kEpsilon )then

        insdtFunctions(KE)

      else
        write(*,'("Unknown turbulence model")') 
        stop 68
      end if


!     *********************************
!     ******** passive scalar *********
!     *********************************

      if( advectPassiveScalar.eq.1 )then
        passiveScalarMacro()
      end if


!     *********************************
!     ******** Boussinesq Model *******
!     *********************************

      ! write(*,'("insdt: pdeModel=",i2," kThermal=",e10.2)') pdeModel,kThermal
      ! ' 

      if( pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel )then
        if( tc.lt.0 )then
          write(*,'("insdt:Boussinesq:ERROR: tc<0 !")')
          stop 8868
        end if
        if( turbulenceModel.eq.noTurbulenceModel )then

          if( materialFormat.eq.constantMaterialProperties )then
            ! const thermal diffusivity:
            boussinesqMacro(NONE,CONST)
          else if( materialFormat.eq.piecewiseConstantMaterialProperties )then
            ! piece-wise constant material properties
            ! write(*,'(" insdt: piece-wise constant material property Heat Eqn...")')
            boussinesqMacro(NONE,PIECEWISE)
          else if( materialFormat.eq.variableMaterialProperties )then
            ! variable material property 
            ! write(*,'(" insdt: variable material property Heat Eqn...")')
            boussinesqMacro(NONE,VAR)
          end if


        else if( turbulenceModel.eq.largeEddySimulation )then
          ! variable thermal diffusivity:
          boussinesqMacro(LES,CONST)
        else
          write(*,'("insdt: Solving T equation : Unknown turbulence model=",i6)') turbulenceModel
          stop 4005
        end if
      end if

!     **********************************
!     ****** artificial diffusion ******  
!     **********************************

      if( use2ndOrderAD.eq.1 .or. use4thOrderAD.eq.1 )then
       if( nd.eq.2 )then
        ! -- 2D --
        if( gridType.eq.rectangular )then
          call insad2dr(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,\
                        nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,\
                        uu, ut,uti,gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, \
                        ipar, rpar, ierr )
        else if( gridType.eq.curvilinear )then
          call insad2dc(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,\
                        nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,\
                        uu, ut,uti,gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, \
                        ipar, rpar, ierr )
        else
          stop 77
        end if
       else
        ! -- 3D --
        if( gridType.eq.rectangular )then
          call insad3dr(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,\
                        nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,\
                        uu, ut,uti,gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, \
                        ipar, rpar, ierr )
        else if( gridType.eq.curvilinear )then
          call insad3dc(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,\
                        nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse,  u,\
                        uu, ut,uti,gv,dw,  ndMatProp,matIndex,matValpc,matVal, bc, \
                        ipar, rpar, ierr )
        else
          stop 77
        end if
       end if
      end if
      return
      end




! ..................................................................................


      subroutine insArtificialDiffusion(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,rsxy,  u,v,  ipar, rpar, ierr )
!======================================================================
!   Add on the artificial diffusion in a semi-implicit way
!
!  Approximately add on: 
!       v = u + dt*AD( v )
! by iterating
!       v(0) = u
!       for k=0,1,..
!         v(k+1) = u + dt*AD( v(k) )   --- but do this implicitly
!
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ierr

      integer ipar(0:*)
      real rpar(0:*)
      
!     ---- local variables -----
      integer c,i1,i2,i3,orderOfAccuracy,gridType,useWhereMask,numberOfIterations
      integer gridIsImplicit,use2ndOrderAD,use4thOrderAD,use6thOrderAD
      integer pc,uc,vc,wc,sc,grid,m,nc,advectPassiveScalar
      real dt,dr(0:2),dx(0:2),adcPassiveScalar
      real ad21,ad22,ad41,ad42,ad61,ad62,cd22,cd42,cd62,adc
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      ! declare variables for difference approximations
      ! include 'declareDiffOrder2f.h'

      declareDifferenceOrder2(u,RX)


!      real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
!      real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
!      real udmzC,udzmC,udmzzC,udzmzC,udzzmC
!      real admzR,adzmR,admzzR,adzmzR,adzzmR
!      real admzC,adzmC,admzzC,adzmzC,adzzmC
!     real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
!     real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
!     real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f
!     real dr2i,ds2i,dt2i,dri,dsi,dti

!     --- begin statement functions
      real ad2Coeff,ad2,ad23Coeff,ad23,ad4Coeff,ad4,ad43Coeff,ad43
      real ad2rCoeff,ad23rCoeff,ad4rCoeff,ad43rCoeff
      real ad2nd,ad23nd,ad4nd,ad43nd

!      real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
!      real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA
!***  include 'insDeriv.h'

! .............. begin statement functions
      integer kd
      real rx,ry,rz,sx,sy,sz,tx,ty,tz

!.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

!     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)

!    --- 2nd order 2D artificial diffusion ---
      ad2Coeff()=(ad21 + cd22* 
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))  
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad2rCoeff()=(ad21 + cd22* 
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))  
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &           +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
      ad2nd(c)=adc*(u(i1+1,i2,i3,c)                 +u(i1-1,i2,i3,c)  ! no diagonal term
     &             +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

!    --- 2nd order 3D artificial diffusion ---
      ad23Coeff()=(ad21 + cd22*   
     &    ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc)) 
     &     +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))  
     &     +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,i3,wc)) ) )
      ad23rCoeff()=(ad21 + cd22*   
     &    ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc)) 
     &     +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,vc))  
     &     +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,wc)) ) )
      ad23(c)=adc
     &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) 
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
      ad23nd(c)=adc
     &    *(u(i1+1,i2,i3,c)                   +u(i1-1,i2,i3,c)  
     &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) 
     &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                       
!     ---fourth-order artificial diffusion in 2D
      ad4Coeff()=(ad41 + cd42*    
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))    
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad4rCoeff()=(ad41 + cd42*    
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))    
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
      ad4(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   
     &      -12.*u(i1,i2,i3,c) ) 
      ad4nd(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   
     &                         ) 
!     ---fourth-order artificial diffusion in 3D
      ad43Coeff()=
     &   (ad41 + cd42*    
     &    ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))    
     &     +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))    
     &     +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,i3,wc)) ) )
      ad43rCoeff()=
     &   (ad41 + cd42*    
     &    ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))    
     &     +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,vc))    
     &     +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,wc)) ) )
      ad43(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  
     &      -18.*u(i1,i2,i3,c) )
      ad43nd(c)=adc
     &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   
     &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   
     &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   
     &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
     &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   
     &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  
     &                         )

!**** Include "selfAdjointArtificialDiffusion.h"

!     --- end statement functions

      ierr=0
      ! write(*,*) 'Inside insdt'

      pc                 =ipar(0)
      uc                 =ipar(1)
      vc                 =ipar(2)
      wc                 =ipar(3)
      sc                 =ipar(4)
      grid               =ipar(5)
      orderOfAccuracy    =ipar(6)
      gridType           =ipar(7)
      useWhereMask       =ipar(8)
      gridIsImplicit     =ipar(9)
      use2ndOrderAD      =ipar(10)
      use4thOrderAD      =ipar(11)
      use6thOrderAD      =ipar(12)
      advectPassiveScalar=ipar(13)
      numberOfIterations =ipar(14)

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      ad21              =rpar(6)
      ad22              =rpar(7)
      ad41              =rpar(8)
      ad42              =rpar(9)
      ad61              =rpar(10)
      ad62              =rpar(11)
      adcPassiveScalar  =rpar(12) 

      dt                = rpar(11) ! ************* add this **************

!$$$      dxi=1./dx
!$$$      dyi=1./dy
!$$$      dzi=1./dz
!$$$      dx2i=1./(2.*dx)
!$$$      dy2i=1./(2.*dy)
!$$$      dz2i=1./(2.*dz)
!$$$      dxsqi=1./(dx*dx)
!$$$      dysqi=1./(dy*dy)
!$$$      dzsqi=1./(dz*dz)
!$$$
!$$$      if( orderOfAccuracy.eq.4 )then
!$$$        dx12i=1./(12.*dx)
!$$$        dy12i=1./(12.*dy)
!$$$        dz12i=1./(12.*dz)
!$$$        dxsq12i=1./(12.*dx**2)
!$$$        dysq12i=1./(12.*dy**2)
!$$$        dzsq12i=1./(12.*dz**2)
!$$$      end if


!     **********************************
!     ****** artificial diffusion ******  
!     **********************************


      cd22n=ad22n/nd
      cd42n=ad42n/nd

      if( use2ndOrderAD.eq.1 .and. 
     &     (ad21.gt.0. .or. ad22.gt.0.) ) then

!      *******************************************
!      ****** 2nd-order artificial diffusion *****
!      *******************************************

       cd22=ad22/(nd**2)
       if( nd.eq.1 )then
         stop 1
       else if( nd.eq.2 )then

!       non-self-adjoint form:
!       loopse4(adc=ad2Coeff(), \
!               ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad2(uc),\
!               ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad2(vc),)

!        self-adjoint form:
!        loopse4($defineArtificialDiffusionCoefficients(2,R,), \
!                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad2f(i1,i2,i3,uc),\
!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad2f(i1,i2,i3,vc),)

         ! -- Here is a aprtially implicit version ---
         if( gridType.eq.rectangular )then     
          loopse4(adc=dt*ad2rCoeff(), \
            v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad2nd(uc))/(1.+4.*adc),\
            v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad2nd(vc))/(1.+4.*adc),)
         else
          loopse4(adc=dt*ad2Coeff(), \
            v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad2nd(uc))/(1.+4.*adc),\
            v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad2nd(vc))/(1.+4.*adc),)
         end if
         
       else ! 3D
!        loopse4(adc=ad23Coeff(), \
!                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad23(uc),\
!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad23(vc),\
!                ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)+ad23(wc) )

!        loopse4($defineArtificialDiffusionCoefficients(3,R,), \
!                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad3f(i1,i2,i3,uc),\
!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad3f(i1,i2,i3,vc),\
!                ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)+ad3f(i1,i2,i3,wc) )

         stop 2
       end if

      end if

      if( use4thOrderAD.eq.1 .and. 
     &     (ad41.gt.0. .or. ad42.gt.0.) ) then

       cd42=ad42/(nd**2)
       if( nd.eq.1 )then
         stop 1
       else if( nd.eq.2 )then

         if( gridType.eq.rectangular )then     
          loopse4(adc=dt*ad4rCoeff(), \
            v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad4nd(uc))/(1.+12.*adc),\
            v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad4nd(vc))/(1.+12.*adc),)
         else
          loopse4(adc=dt*ad4Coeff(), \
            v(i1,i2,i3,uc)=(u(i1,i2,i3,uc)+ad4nd(uc))/(1.+12.*adc),\
            v(i1,i2,i3,vc)=(u(i1,i2,i3,vc)+ad4nd(vc))/(1.+12.*adc),)
         end if

       else ! 3D
         stop 2
!        loopse4(adc=ad43Coeff(), \
!                ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc)+ad43(uc),\
!                ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc)+ad43(vc),\
!                ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc)+ad43(wc) )

       end if

      end if

!     *************************************************
!     *********Advect a passive scalar ****************
!     *************************************************
      if( advectPassiveScalar.eq.1 )then
       adc=adcPassiveScalar ! coefficient of linear artificial diffusion
!       if( nd.eq.1 )then
!         if( orderOfAccuracy.eq.2 )then
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux2(sc)+nuPassiveScalar*uxx2(sc),,,)
!         else
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux4(sc)+nuPassiveScalar*uxx4(sc),,,)
!         end if
!
!       else if( nd.eq.2 )then
!         if( orderOfAccuracy.eq.2 )then
!          loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux2(sc)-uu(vc)*uy2(sc)+nuPassiveScalar*lap2d2(sc)+ad2(sc),,,)
!         else ! order==4
!          loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux4(sc)-uu(vc)*uy4(sc)+nuPassiveScalar*lap2d4(sc)+ad4(sc),,,)
!         end if
!       else ! nd==3
!        if( orderOfAccuracy.eq.2 )then
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux2(sc)-uu(vc)*uy2(sc)-uu(wc)*uz2(sc)\
!                                    +nuPassiveScalar*lap3d2(sc)+ad23(sc),,,)
!        else
!           loopse4(ut(i1,i2,i3,sc)= -uu(uc)*ux4(sc)-uu(vc)*uy4(sc)-uu(wc)*uz4(sc)\
!                                    +nuPassiveScalar*lap3d4(sc)+ad43(sc),,,)
!        end if
!       end if ! end nd

      end if ! advectPassiveScalar


      return
      end
