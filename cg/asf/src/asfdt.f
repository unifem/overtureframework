! This file automatically generated from asfdt.bf with bpp.
c
c Compute du/dt for the compressible all-speed Navier-Stokes
c


c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX



c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX

c **********************************************************************
c  This file contains some commonly used macros.
c **********************************************************************


c Define macros for the derivatives based on the dimension, order of accuracy and grid-type


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================











c Define the artificial diffusion coefficients
c gt should be R or C (gridType is Rectangular or Curvilinear)
c tb should be blank or SA  (SA=Spalart-Allamras turbulence model)

c Define macros for the derivatives based on the dimension, order of accuracy and grid-type


c =============================================================
c Compute derivatives of u,v,w 
c =============================================================

c =============================================================
c Compute Spalart-Allmaras quantities
c   This macro assumes u0x,u0y, ... are defined
c =============================================================

c =============================================================
c Compute k-epsilon quantities
c   This macro assumes u0x,u0y, ... are defined
c =============================================================

c====================================================================
c This macro will build the statements that form the body of the loop
c
c IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
c OPTION: ASF : ASF equations
c         ASFSPAL - ASF + Spalart-Allmaras turbulence model
c         ASF-BL - ASF + Baldwin Lomax turbulence model
c SCALAR: NONE
c         PASSIVE - include equations for a passive scalar
c AXISYMMETRIC : YES or NO
c====================================================================

c***************************************************************
c  Define the equations for EXPLICIT time stepping
c
c SOLVER: ASF, ASFSPAL, ASFBL, ASFKE
c ORDER: 2,4
c DIM: 2,3
c GRIDTYPE: rectangular, curvilinear
c
c***************************************************************


c$$$#beginMacro fillByOrder(SOLVER,DIM,GRIDTYPE)
c$$$if( orderOfAccuracy.eq.2 )then
c$$$ fillEquations(SOLVER,DIM,2,GRIDTYPE)
c$$$else if( orderOfAccuracy.eq.4 )then
c$$$ fillEquations(SOLVER,DIM,4,GRIDTYPE)
c$$$else
c$$$ stop 88
c$$$end if
c$$$#endMacro
c$$$
c$$$#beginMacro fillByDimension(SOLVER,GRIDTYPE)
c$$$if( nd.eq.2 )then
c$$$ fillByOrder(SOLVER,2,GRIDTYPE)
c$$$else if( nd.eq.3 )then
c$$$ fillByOrder(SOLVER,3,GRIDTYPE)
c$$$else
c$$$ stop 99
c$$$end if
c$$$#endMacro

c====================================================================================
c
c SOLVER: ASF, ASFSPAL, ASFBL, ASFKE
c
c====================================================================================



c================================================================
c  Add on the artificial dissipation
c================================================================






c======================================================================================
c Define the subroutine to compute du/dt
c
c SOLVER: ASF, ASFSPAL, ASFBL, ASFKE
c
c======================================================================================

c 
c : empty version for linking when we don't want an option
c



c Here we create the files
c#beginFile asfdtASF2dOrder2Null.f
c ASFDT_NULL(ASF,asfdtASF2dOrder2,2,2)
c#endFile
c      buildFile(ASF,asfdtASF2dOrder4,2,4)
c#beginFile asfdtASF3dOrder2Null.f
c ASFDT_NULL(ASF,asfdtASF3dOrder2,3,2)
c#endFile
c      buildFile(ASF,asfdtASF3dOrder4,3,4)

c      buildFile(ASFSPAL,asfdtSPAL2dOrder2,2,2)
c      buildFile(ASFSPAL,asfdtSPAL2dOrder4,2,4)
c      buildFile(ASFSPAL,asfdtSPAL3dOrder2,3,2)
c      buildFile(ASFSPAL,asfdtSPAL3dOrder4,3,4)

c      buildFile(ASFKE,asfdtKE2dOrder2,2,2)
c      buildFile(ASFKE,asfdtKE2dOrder4,2,4)
c      buildFile(ASFKE,asfdtKE3dOrder2,3,2)
c      buildFile(ASFKE,asfdtKE3dOrder4,3,4)

c      ! Visco-plastic case
c      buildFile(ASFVP,asfdtVP2dOrder2,2,2)
c      buildFile(ASFVP,asfdtVP3dOrder2,3,2)

c ====================================================
c SOLVER: ASF, SPAL, KE
c ====================================================


c ==========================================================
c  Advect a passive scalar -- kernel
c ==========================================================

c ==============================================================
c  Advect a passive scalar -- build loops for different cases:
c     DIM,ORDER,GRIDTYPE
c ==============================================================


      subroutine asfdt(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, ut,uti,rL, pL, gv,dw, 
     &  bc, ipar, rpar, ierr )
c======================================================================
c   Compute du/dt for the all-speed compressible NS
c 
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uti(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real pL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)

c     ---- local variables -----
      integer orderOfAccuracy

c     integer c,i1,i2,i3,kd,kd3,orderOfAccuracy,gridIsMoving,useWhereMask
c     integer gridIsImplicit,implicitOption,implicitMethod,
c    & isAxisymmetric,use2ndOrderAD,use4thOrderAD
c     integer pc,uc,vc,wc,sc,nc,kc,ec,tc,grid,m,advectPassiveScalar
c     real nu,dt,nuPassiveScalar,adcPassiveScalar
c     real gravity(0:2), thermalExpansivity, adcBoussinesq,kThermal
c     real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
c     real ad21,ad22,ad41,ad42,cd22,cd42,adc
c     real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
c     real yy,yEps

      integer gridType
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      integer pdeModel,BoussinesqModel,viscoPlasticModel
      parameter( BoussinesqModel=1,viscoPlasticModel=2 )

c      real cdmz,cdpz,cdzm,cdzp,cdmzz,cdpzz,cdzmz,cdzpz,cdzzm,cdzzp,cdDiag,cdm,cdp
c      real uxmzzR,uymzzR,uzmzzR,uxzmzR,uyzmzR,uzzmzR,uxzzmR,uyzzmR,uzzzmR
c      real udmzC,udzmC,udmzzC,udzmzC,udzzmC
c      real admzR,adzmR,admzzR,adzmzR,adzzmR
c      real admzC,adzmC,admzzC,adzmzC,adzzmC
c      real admzRSA,adzmRSA,admzzRSA,adzmzRSA,adzzmRSA
c      real admzCSA,adzmCSA,admzzCSA,adzmzCSA,adzzmCSA
c      real adE0,adE1,ade2,adE3d0,adE3d1,adE3d2,ad2f,ad3f

c     real delta22,delta23,delta42,delta43

c     real adCoeff2,adCoeff4

c     real ad2,ad23,ad4,ad43
c     real adSelfAdjoint2dR,adSelfAdjoint3dR,adSelfAdjoint2dC,adSelfAdjoint3dC
c     real adSelfAdjoint2dRSA,adSelfAdjoint3dRSA,adSelfAdjoint2dCSA,adSelfAdjoint3dCSA

c     real rx,ry,rz,sx,sy,sz,tx,ty,tz
c     real dr(0:2), dx(0:2)

      ! for SPAL TM
c      real n0,n0x,n0y,n0z
c      real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
c      real chi,chi3,fnu1,fnu2,s,r,g,fw,dKappaSq,nSqBydSq,dd
c      real nuT,nuTx,nuTy,nuTz,nuTd

      ! for k-epsilon
c      real k0,k0x,k0y,k0z, e0,e0x,e0y,e0z
c      real nuP,prod
c      real cMu,cEps1,cEps2,sigmaEpsI,sigmaKI


      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'

c     declareDifferenceOrder2(u,RX)
c     declareDifferenceOrder4(u,RX)

c     --- begin statement functions
c     rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
c     ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
c     rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
c     sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
c     sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
c     sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
c     tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
c     ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
c     tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

c     The next macro call will define the difference approximation statement functions
c     defineDifferenceOrder2Components1(u,RX)
c     defineDifferenceOrder4Components1(u,RX)


c    --- 2nd order 2D artificial diffusion ---
c     ad2(c)=adc*(u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
c    &           +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))

c    --- 2nd order 3D artificial diffusion ---
c     ad23(c)=adc
c    &    *(u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  
c    &     +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c) 
c    &     +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c))

c     ---fourth-order artificial diffusion in 2D
c     ad4(c)=adc
c    &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)    
c    &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)    
c    &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)    
c    &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   
c    &      -12.*u(i1,i2,i3,c) ) 
c     ---fourth-order artificial diffusion in 3D
c     ad43(c)=adc
c    &    *(   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   
c    &         -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   
c    &         -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)   
c    &     +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   
c    &         +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)   
c    &         +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))  
c    &      -18.*u(i1,i2,i3,c) )

c    --- For 2nd order 2D artificial diffusion ---
c     delta22(c)=    (u(i1+1,i2,i3,c)-4.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)  c                  +u(i1,i2+1,i3,c)                 +u(i1,i2-1,i3,c))
c    --- For 2nd order 3D artificial diffusion ---
c     delta23(c)= c       (u(i1+1,i2,i3,c)-6.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c)   c       +u(i1,i2+1,i3,c)                   +u(i1,i2-1,i3,c)  c       +u(i1,i2,i3+1,c)                   +u(i1,i2,i3-1,c)) 
c     ---For fourth-order artificial diffusion in 2D
c     delta42(c)= c       (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   c           -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   c       +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   c           +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  c        -12.*u(i1,i2,i3,c) ) 
c     ---For fourth-order artificial diffusion in 3D
c     delta43(c)= c       (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  c           -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)  c           -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  c       +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  c           +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)  c           +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) c        -18.*u(i1,i2,i3,c) )


c #Include "selfAdjointArtificialDiffusion.h"

c     --- end statement functions

      ierr=0
      ! write(*,'("Inside asfdt: gridType=",i2)') gridType

      pdeModel           =ipar(0)
      turbulenceModel    =ipar(1)
      orderOfAccuracy    =ipar(2)


      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("asfdt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if

c     *********************************      
c     ********MAIN LOOPS***************      
c     *********************************      

      if( turbulenceModel.eq.noTurbulenceModel .and. 
     & pdeModel.eq.viscoPlasticModel )then
        ! asf + visco-plastic model
c**        asfdtFunctions(VP)
        stop 123

      else if( turbulenceModel.eq.noTurbulenceModel )then

         if( orderOfAccuracy.eq.2 )then
          if( nd.eq.2 )then
            call asfdtASF2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, ut,uti,rL,
     & pL, gv,dw,  bc, ipar, rpar, ierr )
          else
            call asfdtASF3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, ut,uti,rL,
     & pL, gv,dw,  bc, ipar, rpar, ierr )
          end if
         else if( orderOfAccuracy.eq.4 )then
          stop 555
          if( nd.eq.2 )then
c    call asfdtASF2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c           mask,xy,rsxy,  u,uu, ut,uti,rL,pL, gv,dw,  bc, ipar, rpar, ierr )
          else
c    call asfdtASF3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c           mask,xy,rsxy,  u,uu, ut,uti,rL,pL, gv,dw,  bc, ipar, rpar, ierr )
          end if
         else
           stop 1111
         end if

      else if( turbulenceModel.eq.spalartAllmaras )then

c**        asfdtFunctions(SPAL)
        stop 456

      else if( turbulenceModel.eq.kEpsilon )then

c**        asfdtFunctions(KE)
        stop 789

      else
        write(*,'("Unknown turbulence model")')
        stop 68
      end if


c     *********************************
c     ******** passive scalar *********
c     *********************************

c*      if( advectPassiveScalar.eq.1 )then
c*        passiveScalarMacro()
c*      end if


c     **********************************
c     ****** artificial diffusion ******  
c     **********************************

c*      if( use2ndOrderAD.eq.1 .or. use4thOrderAD.eq.1 )then
c*        if( gridType.eq.rectangular )then
c*          addDissipationByDimension( rectangular )
c*        else if( gridType.eq.curvilinear )then
c*          addDissipationByDimension( curvilinear )
c*        else
c*          stop 77
c*        end if
c*      end if


      return
      end


