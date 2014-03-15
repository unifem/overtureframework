!
! Compute artificial dissipation to various orders for cgins
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


!================================================================
!  Add on the artificial dissipation
!================================================================
#beginMacro addArtficialDissipation(ADTYPE,DIM,ORDER,GRIDTYPE)

#If #ADTYPE == "AD2" || #ADTYPE == "AD24" || #ADTYPE == "AD4"
#Else
  stop 99
#End

#If #DIM == "2"
 if( turbulenceModel.eq.noTurbulenceModel .or. turbulenceModel.eq.largeEddySimulation )then
  defineArtificialDissipationMacro(ADTYPE,DIM,INS)
  loopse4($getArtificialDissipationCoeff(ADTYPE,DIM,INS),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),)
 else if( turbulenceModel.eq.spalartAllmaras )then
  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
  loopse4($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
          ut(i1,i2,i3,nc)=ut(i1,i2,i3,nc) artificialDissipationTM(nc))
 else if( turbulenceModel.eq.kEpsilon )then
  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
  loopse6($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
          ut(i1,i2,i3,kc)=ut(i1,i2,i3,kc) artificialDissipationTM(kc),\
          ut(i1,i2,i3,ec)=ut(i1,i2,i3,ec) artificialDissipationTM(ec),)
 else
   write(*,'("insad:artificialDiss: unexpected turbulence model=",i4)') turbulenceModel
   stop 442
 end if

#Elif #DIM == "3"

 if( turbulenceModel.eq.noTurbulenceModel .or. turbulenceModel.eq.largeEddySimulation )then
  defineArtificialDissipationMacro(ADTYPE,DIM,INS)
  loopse4($getArtificialDissipationCoeff(ADTYPE,DIM,INS),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc) artificialDissipation(wc))
 else if( turbulenceModel.eq.spalartAllmaras )then
  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
  loopse6($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc) artificialDissipation(wc),\
          ut(i1,i2,i3,nc)=ut(i1,i2,i3,nc) artificialDissipationTM(nc),)
 else if( turbulenceModel.eq.kEpsilon )then
  defineArtificialDissipationMacro(ADTYPE,DIM,SPAL)
  loopse6($getArtificialDissipationCoeff(ADTYPE,DIM,SPAL),\
          ut(i1,i2,i3,uc)=ut(i1,i2,i3,uc) artificialDissipation(uc),\
          ut(i1,i2,i3,vc)=ut(i1,i2,i3,vc) artificialDissipation(vc),\
          ut(i1,i2,i3,wc)=ut(i1,i2,i3,wc) artificialDissipation(wc),\
          ut(i1,i2,i3,kc)=ut(i1,i2,i3,kc) artificialDissipationTM(kc),\
          ut(i1,i2,i3,ec)=ut(i1,i2,i3,ec) artificialDissipationTM(ec))
 else
   write(*,'("insad:artificialDiss: unexpected turbulence model=",i4)') turbulenceModel
   stop 443
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

! Not used anymore:
#beginMacro addDissipationByDimension( GRIDTYPE )
  if( nd.eq.2 )then
    addDissipationByOrder( 2,GRIDTYPE )
  else if( nd.eq.3 )then
    addDissipationByOrder( 3,GRIDTYPE )
  else
    stop 66
  end if
#endMacro



! =================================================================================================
! Macro to define artificial dissipation functions
! =================================================================================================
#beginMacro INSAD_MACRO(NAME,DIM,GRIDTYPE)
      subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                       mask,xy,rsxy,radiusInverse,  u,uu, ut,uti,gv,dw,  \
                       ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, ierr )
!======================================================================
!   Compute the artificial dissipation for cgins.
! 
! Notes:
!   - this routine is the template for insad2dr.f, inad2dc.f, insad3dr.f and insad3dc.f
!   - these routines are called from insdt.bf
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
      
      ! -- arrays for variable material properties --
      integer constantMaterialProperties
      integer piecewiseConstantMaterialProperties
      integer variableMaterialProperties
      parameter( constantMaterialProperties=0,\
                 piecewiseConstantMaterialProperties=1,\
                 variableMaterialProperties=2 )
      integer materialFormat,ndMatProp
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

!     ---- local variables -----
      integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
      integer gridIsImplicit,implicitOption,implicitMethod,debug,isAxisymmetric,use2ndOrderAD,use4thOrderAD
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


      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,twoPhaseFlowModel=3 )

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
      ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  \
                 +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

!    --- 2nd order 3D artificial diffusion ---
      ad23(c)=adc \
          *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)    \
           +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  \
           +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))
                       
!     ---fourth-order artificial diffusion in 2D
      ad4(c)=adc               \
          *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  \
               -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)  \
           +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  \
               +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
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
        (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)    \
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
      ! write(*,'("Inside NAME ...")') 

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
      pdeModel           =ipar(20)  ! **new**
      vsc                =ipar(21)
      ! rc               =ipar(22)
      debug              =ipar(23)
      materialFormat     =ipar(24)

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
        write(*,'("NAME:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if
      if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
        write(*,'("NAME:ERROR gridType=",i6)') gridType
        stop 2
      end if
      if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
        write(*,'("NAME:ERROR uc,vc,ws=",3i6)') uc,vc,wc
        stop 4
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

      adc=adcPassiveScalar ! coefficient of linear artificial diffusion
      cd22=ad22/(nd**2)
      cd42=ad42/(nd**2)

      cd22n=ad22n/nd
      cd42n=ad42n/nd

!     **********************************
!     ****** artificial diffusion ******  
!     **********************************

      addDissipationByOrder( DIM,GRIDTYPE )

      return
      end
#endMacro

! macro to output the different AD files
#beginMacro buildFile(NAME,DIM,GRIDTYPE)
#beginFile src/NAME.f
 INSAD_MACRO(NAME,DIM,GRIDTYPE)
#endFile
#endMacro


      buildFile(insad2dr,2,rectangular)
      buildFile(insad2dc,2,curvilinear)

      buildFile(insad3dr,3,rectangular)
      buildFile(insad3dc,3,curvilinear)

