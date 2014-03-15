
c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

#Include "commonMacros.h"

#beginMacro beginLoops()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).ne.0 )then
#endMacro

#beginMacro endLoops()
  end if
 end do
 end do
 end do
#endMacro

c **************************************************************
c   Macro to compute the LES viscosity 
c **************************************************************
#beginMacro computeLESNuT(u,nc,DIM,ORDER,GRIDTYPE)

 defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

 beginLoops()

  ! evaluate some derivatives (from macros defined in commonMacros.h
  u0x = UX(uc)
  u0y = UY(uc)
  v0x = UX(vc)
  v0y = UY(vc)
  #If #DIM == "3"
    u0z = UZ(uc)
    v0z = UZ(vc)
    w0x = UX(wc)  
    w0y = UX(wc)  
    w0z = UX(wc)  

  #End

  if( lesOption.le.0 )then
    ! constant viscosity: 
    u(i1,i2,i3,nc)=nu 
  else if( lesOption.le.1 )then
    ! test: nuT = nu + .1*( x^2 + y^ 2 )
    ! WARNING: the xy array is currently only valid for TZ flow.
    ! u(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,0)**2 + xy(i1,i2,i3,1)**2 )

    ! test: 
    ! u(i1,i2,i3,nc)=nu + nu*( u0x*u0x )
    u(i1,i2,i3,nc)=nu + lesPar1*nu*( u0x**2 + u0y**2 + v0x**2 + v0y**2 )
  end if

 endLoops()

#endMacro


! ================================================================================
! Define the LES Coefficient of Viscosity
!
!=================================================================================
#beginMacro GET_LES_VISCOSITY()
 subroutine getLargeEddySimulationViscosity(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
       mask,rsxy,xy,  u, v, dw, bc, boundaryCondition, ipar, rpar, pdb, ierr )
!======================================================================
!
! nd : number of space dimensions
!
! n1a,n1b,n2a,n2b,n3a,n3b : 
! u : current solution
! v : save results in v(i1,i2,i3,nc). v and u may be the same
!
! dw: distance to wall
!======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr

 integer ipar(0:*)
 real rpar(0:*)
 double precision pdb  ! pointer to data base
 
 !     ---- local variables -----
 integer m,n,c,i1,i2,i3,orderOfAccuracy,useWhereMask,i1p,i2p,i3p
 integer pc,uc,vc,wc,tc,nc,vsc,grid,side,gridType
 integer twilightZoneFlow,lesOption
 integer indexRange(0:1,0:2),is1,is2,is3,kd
 real nu,dx(0:2),dr(0:2),t

 real lesPar1

 real u0,u0x,u0y,u0z
 real v0,v0x,v0y,v0z
 real w0,w0x,w0y,w0z

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

 character *50 name
 integer ok,getInt,getReal

 integer \
     noSlipWall,\
     outflow,\
     convectiveOutflow,\
     tractionFree,\
     inflowWithPandTV,\
     dirichletBoundaryCondition,\
     symmetry,\
     axisymmetric
 parameter( noSlipWall=1,outflow=5,convectiveOutflow=14,tractionFree=15,\
  inflowWithPandTV=3,dirichletBoundaryCondition=12,symmetry=11,axisymmetric=13 )

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer interpolate,dirichlet,neumann,extrapolate
 parameter( interpolate=0, dirichlet=1, neumann=2, extrapolate=3 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

!     --- begin statement functions

 real rxi
 real uu, ux2,uy2,uz2,ux2c,uy2c,ux3c,uy3c,uz3c
 real rx,ry,rz,sx,sy,sz,tx,ty,tz

 declareDifferenceOrder2(u,RX)

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

 rxi(m,n) = rsxy(i1,i2,i3,m,n)
 uu(c)    = u(i1,i2,i3,c)

 ux2(c)   = ux22r(i1,i2,i3,c)
 uy2(c)   = uy22r(i1,i2,i3,c)
 uz2(c)   = uz23r(i1,i2,i3,c)

 ux2c(m) = ux22(i1,i2,i3,m)
 uy2c(m) = uy22(i1,i2,i3,m)

 ux3c(m) = ux23(i1,i2,i3,m)
 uy3c(m) = uy23(i1,i2,i3,m)
 uz3c(m) = uz23(i1,i2,i3,m)


 ierr=0

 nc                =ipar(0)
 grid              =ipar(1)
 gridType          =ipar(2)
 orderOfAccuracy   =ipar(3)
 useWhereMask      =ipar(4)
 turbulenceModel   =ipar(5)
 twilightZoneFlow  =ipar(6)
 pdeModel          =ipar(7)

 dx(0)             =rpar(0)
 dx(1)             =rpar(1)
 dx(2)             =rpar(2)
 dr(0)             =rpar(3)
 dr(1)             =rpar(4)
 dr(2)             =rpar(5)
 t                 =rpar(6)

 ok = getInt(pdb,'uc',uc)  
 if( ok.eq.0 )then
   write(*,'("*** getLargeEddySimulationViscosity: ERROR: uc NOT FOUND")') 
 end if
 ok = getInt(pdb,'vc',vc)  
 if( ok.eq.0 )then
   write(*,'("*** getLargeEddySimulationViscosity: ERROR: vc NOT FOUND")') 
 end if
 ok = getInt(pdb,'wc',wc)  
 if( ok.eq.0 )then
   write(*,'("*** getLargeEddySimulationViscosity: ERROR: wc NOT FOUND")') 
 end if

 ok = getReal(pdb,'nu',nu)  
 if( ok.eq.0 )then
   write(*,'("*** getLargeEddySimulationViscosity: ERROR: nu NOT FOUND")') 
 end if

 ! Access parameters that are defined in the command file with the commands 'define real/integer parameter ...'
 ! Note that these parameters go into the data-base under the PdeParameters directory
 lesOption=-1  ! default value if not found
 ok = getInt(pdb,'PdeParameters/lesOption',lesOption)

 lesPar1=.01 ! default value if not found
 ok = getReal(pdb,'PdeParameters/lesPar1',lesPar1)
 
 if( t.eq.0 )then
   write(*,'("getLargeEddySimulationViscosity: option=",i4," lesPar1=",e10.2)') lesOption,lesPar1
   write(*,'("  Info:  option=0 : nuT = nu (constant)")')
   write(*,'("         option=1 : nuT = nu + lesPar1*nu*( u0x**2 + u0y**2 + v0x**2 + v0y**2 )")')
 end if

 if ( turbulenceModel.ne.largeEddySimulation ) then
   stop 5001
 end if

 if( t.le.0. )then
   write(*,'("getLargeEddySimulationViscosity: nu=",e10.2,", set nuT at t=",e10.2)') nu,t
 endif

 ! if( orderOfAccuracy.ne.2 )then
  ! write(*,'("getLargeEddySimulationViscosity: finish me for orderOfAccuracy=",i4)') orderOfAccuracy
  ! stop 502
 ! endif

 ! computeLESNuT(u,nc,DIM,ORDER,GRIDTYPE)
 if( nd.eq.2 )then

   if( gridType.eq.rectangular )then
    computeLESNuT(v,nc,2,2,rectangular)
   else if( gridType.eq.curvilinear )then
    computeLESNuT(v,nc,2,2,curvilinear)
   else
     stop 510
   end if

 else if( nd.eq.3 )then

   if( gridType.eq.rectangular )then
    computeLESNuT(v,nc,3,2,rectangular)
   else if( gridType.eq.curvilinear )then
    computeLESNuT(v,nc,3,2,curvilinear)
   else
     stop 520
   end if

 else
  stop 590
 endif


 return
 end
#endMacro


      GET_LES_VISCOSITY()
