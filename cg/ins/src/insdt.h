!  -*- mode: F90 -*-
! ===============================================================================
! This file is included by
!     insdtINS.bf 
!     insdtKE.bf
!     insdtSPAL.bf
!     insdtVP.bf
! ==============================================================================

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




! Define the artificial diffusion coefficients
! gt should be R or C (gridType is Rectangular or Curvilinear)
! tb should be blank or SA  (SA=Spalart-Allamras turbulence model)
#beginMacro defineArtificialDiffusionCoefficients(dim,gt,tb)
  #If #dim == "2" 
    cdmz=admz ## gt ## tb(i1  ,i2  ,i3)
    cdpz=admz ## gt ## tb(i1+1,i2  ,i3)
    cdzm=adzm ## gt ## tb(i1  ,i2  ,i3)
    cdzp=adzm ## gt ## tb(i1  ,i2+1,i3)
    cdDiag=cdmz+cdpz+cdzm+cdzp
    ! write(*,'(1x,''insdt:i1,i2,cdmz,cdzm='',2i3,2f9.3)') i1,i2,cdmz,cdzm
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

! Define macros for the derivatives based on the dimension, order of accuracy and grid-type
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


! =============================================================
! Compute derivatives of u,v,w 
! =============================================================
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





!***************************************************************
!  Define the equations for EXPLICIT time stepping
!
! ORDER: 2,4
! DIM: 2,3
! GRIDTYPE: rectangular, curvilinear
!
!***************************************************************
#beginMacro fillEquations(DIM,ORDER,GRIDTYPE)

defineDerivativeMacros(DIM,ORDER,GRIDTYPE)

if( isAxisymmetric.eq.0 )then

 if( gridIsImplicit.eq.0 )then
  ! --- explicit time-stepping ---

  if( advectionOption.eq.centeredAdvection )then
    loopse1($buildEquations(EXPLICIT,NONE,DIM,ORDER,GRIDTYPE,NO))

  else if( advectionOption.eq.upwindAdvection )then  
    ! --- upwind ---
    loopse1($buildEquationsUpwind(EXPLICIT,NONE,DIM,ORDER,GRIDTYPE,NO,UPWIND))

  else if( advectionOption.eq.bwenoAdvection )then  
    ! --- bweno ---
    loopse1($buildEquationsUpwind(EXPLICIT,NONE,DIM,ORDER,GRIDTYPE,NO,BWENO))

  else
    write(*,'(" unknown advectionOption")')
    stop 1010
  end if

 else ! gridIsImplicit
  ! ---- implicit time-stepping ---
  if( advectionOption.eq.centeredAdvection )then
   if( implicitOption .eq.computeImplicitTermsSeparately )then
     loopse1($buildEquations(BOTH,NONE,DIM,ORDER,GRIDTYPE,NO))
   else if( implicitOption.eq.doNotComputeImplicitTerms )then
     loopse1($buildEquations(EXPLICIT_ONLY,NONE,DIM,ORDER,GRIDTYPE,NO))
   else
    write(*,*)'insdt: Unknown implicitOption=',implicitOption
    stop 5
   end if  ! end implicitOption

  else if( advectionOption.eq.upwindAdvection )then  
    ! --- upwind ---
   if( implicitOption .eq.computeImplicitTermsSeparately )then
     loopse1($buildEquationsUpwind(BOTH,NONE,DIM,ORDER,GRIDTYPE,NO,UPWIND))
   else if( implicitOption.eq.doNotComputeImplicitTerms )then
     loopse1($buildEquationsUpwind(EXPLICIT_ONLY,NONE,DIM,ORDER,GRIDTYPE,NO,UPWIND))
   else
    write(*,*)'insdt: Unknown implicitOption=',implicitOption
    stop 6
   end if  ! end implicitOption

  else if( advectionOption.eq.bwenoAdvection )then  
    ! --- bweno ---
   if( implicitOption .eq.computeImplicitTermsSeparately )then
     loopse1($buildEquationsUpwind(BOTH,NONE,DIM,ORDER,GRIDTYPE,NO,BWENO))
   else if( implicitOption.eq.doNotComputeImplicitTerms )then
     loopse1($buildEquationsUpwind(EXPLICIT_ONLY,NONE,DIM,ORDER,GRIDTYPE,NO,BWENO))
   else
    write(*,*)'insdt: Unknown implicitOption=',implicitOption
    stop 7
   end if  ! end implicitOption

  else
    write(*,'(" unknown advectionOption")')
    stop 1010
  end if

 end if

else if( isAxisymmetric.eq.1 )then

 if( advectionOption.ne.centeredAdvection )then
   write(*,*) 'insdt.h : finish me for axisymmetric'
   stop 2020
 end if

 #If (#DIM == "2") 
 ! **** axisymmetric case ****
 if( gridIsImplicit.eq.0 )then
  ! explicit

  loopse1($buildEquations(EXPLICIT,NONE,DIM,ORDER,GRIDTYPE,YES))

 else ! gridIsImplicit
  ! ***** implicit *******

  if( implicitOption .eq.computeImplicitTermsSeparately )then
    loopse1($buildEquations(BOTH,NONE,DIM,ORDER,GRIDTYPE,YES))
  else if( implicitOption.eq.doNotComputeImplicitTerms )then
    loopse1($buildEquations(EXPLICIT_ONLY,NONE,DIM,ORDER,GRIDTYPE,YES))
  else
   write(*,*)'insdt: Unknown implicitOption=',implicitOption
   stop 5
  end if  ! end implicitOption

 end if
 #End

else
  stop 88733
end if 

#endMacro



!====================================================================================
!
!====================================================================================
#beginMacro assignEquations(DIM,ORDER)
if( gridType.eq.rectangular )then
 fillEquations(DIM,ORDER,rectangular)
else if( gridType.eq.curvilinear )then
 fillEquations(DIM,ORDER,curvilinear)
else
  stop 77
end if
#endMacro


!======================================================================================
! Define the subroutine to compute du/dt
!
!======================================================================================
#beginMacro INSDT(NAME,DIM,ORDER)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
         mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
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
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

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
 
 !     ---- local variables -----
 integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
 integer gridIsImplicit,implicitOption,implicitMethod,isAxisymmetric,use2ndOrderAD,use4thOrderAD
 integer rc,pc,uc,vc,wc,sc,nc,kc,ec,tc,grid,m,advectPassiveScalar,vsc
 real nu,dt,nuPassiveScalar,adcPassiveScalar
 real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
 real ad21,ad22,ad41,ad42,cd22,cd42,adc
 real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
 real yy,ri


 integer materialFormat
 real t

 integer gridType
 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,largeEddySimulation
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

 integer upwindOrder,debug
 integer advectionOption, centeredAdvection,upwindAdvection,bwenoAdvection
 parameter( centeredAdvection=0, upwindAdvection=1, bwenoAdvection=2 )
 real au,agu(0:5,0:5) ! for holdings upwind approximations to (a.grad)u

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

 ! for visco-plastic
 ! real nuVP,etaVP,yieldStressVP,exponentVP,epsVP
 ! real eDotNorm,exp0
 ! real u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
 ! real v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
 ! real w0xx,w0xy,w0xz,w0yy,w0yz,w0zz

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

!     The next macro call will define the difference approximation statement functions
#If #ORDER == "2"
 defineDifferenceOrder2Components1(u,RX)
#End
#If #ORDER == "4"
 defineDifferenceOrder4Components1(u,RX)
#End

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

!     --- end statement functions

 ierr=0
 ! write(*,'("Inside insdt: gridType=",i2)') gridType

 pc                 =ipar(0)
 uc                 =ipar(1)
 vc                 =ipar(2)
 wc                 =ipar(3)
 nc                 =ipar(4)
 sc                 =ipar(5)
 tc                 =ipar(6)  ! **new**
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
 rc                 =ipar(22)
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

!       gravity(0)        =rpar(18)
!      gravity(1)        =rpar(19)
!      gravity(2)        =rpar(20)
!      thermalExpansivity=rpar(21)
!      adcBoussinesq     =rpar(22) ! coefficient of artificial diffusion for Boussinesq T equation 
!      kThermal          =rpar(23)
 t                 =rpar(24)

! nuVP              =rpar(24)  ! for visco-plastic
 ! etaVP             =rpar(25)
 ! yieldStressVP     =rpar(26)
 ! exponentVP        =rpar(27)
 ! epsVP             =rpar(28)

! write(*,'(" insdt: eta,yield,exp,eps=",4e10.2)') etaVP,yieldStressVP,exponentVP,epsVP

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


! ** these are needed by self-adjoint terms **fix**
 dxi=1./dx(0)
 dyi=1./dx(1)
 dzi=1./dx(2)
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
   ! do nothing
 else if( turbulenceModel.ne.noTurbulenceModel )then
   stop 88
 end if

 adc=adcPassiveScalar ! coefficient of linear artificial diffusion
 cd22=ad22/(nd**2)
 cd42=ad42/(nd**2)

!     *********************************      
!     ********MAIN LOOPS***************      
!     *********************************      
 assignEquations(DIM,ORDER)

 return
 end
#endMacro

! 
! : empty version for linking when we do not want an option
!
#beginMacro INSDT_NULL(NAME,DIM,ORDER)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
         mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
!======================================================================
!       EMPTY VERSION for Linking without this Capability
!
!   Compute du/dt for the incompressible NS on rectangular grids
!     OPTIMIZED version for rectangular grids.
! nd : number of space dimensions
!
! gv : gridVelocity for moving grids
! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
! dw : distance to the wall for some turbulence models
!======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

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

 write(*,'("ERROR: NULL version of subroutine NAME called")')
 write(*,'(" You may have to turn on an option in the Makefile.")')

 stop 1080

 return
 end
#endMacro


#beginMacro buildFile(NAME,DIM,ORDER)
#beginFile src/NAME.f
 INSDT(NAME,DIM,ORDER)
#endFile
#beginFile src/NAME ## Null.f
 INSDT_NULL(NAME,DIM,ORDER)
#endFile
#endMacro


