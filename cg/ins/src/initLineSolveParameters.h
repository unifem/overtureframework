c Here are the statements we use to initialize the main subroutines below
#beginMacro INITIALIZE(SOLVER)
      pc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      grid              =ipar(4)
      orderOfAccuracy   =ipar(5)
      gridIsMoving      =ipar(6)
      useWhereMask      =ipar(7)
      gridIsImplicit    =ipar(8)
      implicitMethod    =ipar(9)
      implicitOption    =ipar(10)
      isAxisymmetric    =ipar(11)
      use2ndOrderAD     =ipar(12)
      use4thOrderAD     =ipar(13)
      gridType          =ipar(14)

      computeMatrix     =ipar(15)
      computeRHS        =ipar(16)
      computeMatrixBC   =ipar(17)
      fc                =ipar(18)
      fcu=fc
      fcv=fc+1
      fcw=fc+2
      fcn=fc+nd
      fct=fc+nd
      orderOfExtrapolation=ipar(19)
      ibc               = ipar(20)

      option            = ipar(21)
      nc                = ipar(22)
      turbulenceModel   = ipar(23)
      twilightZoneFlow  = ipar(24)

      useSelfAdjointDiffusion=ipar(25)
      fourthOrder       = ipar(26)
      pdeModel          = ipar(27)
      tc                = ipar(28)
      numberOfComponents= ipar(29)
      systemComponent   = ipar(30) ! form the tridiagonal system for this component 

      gid(0,0)          = ipar(31)
      gid(1,0)          = ipar(32)
      gid(0,1)          = ipar(33)
      gid(1,1)          = ipar(34)
      gid(0,2)          = ipar(35)
      gid(1,2)          = ipar(36)

      vsc               = ipar(37)

      dx(0)            =rpar(0)
      dx(1)            =rpar(1)
      dx(2)            =rpar(2)
      nu                =rpar(3)
      ad21              =rpar(4)
      ad22              =rpar(5)
      ad41              =rpar(6)
      ad42              =rpar(7)

      dr(0)             =rpar(8)
      dr(1)             =rpar(9)
      dr(2)             =rpar(10)
      cfl               =rpar(11)
      ad21n             =rpar(12)
      ad22n             =rpar(13)
      ad41n             =rpar(14)
      ad42n             =rpar(15)
      kThermal          =rpar(16)
      
      thermalExpansivity=rpar(17)
      gravity(0)        =rpar(18)
      gravity(1)        =rpar(19)
      gravity(2)        =rpar(20)


#If #SOLVER == "INSVP"

!*       nuViscoPlastic         =rpar(21)
!*       etaViscoPlastic        =rpar(22)
!*       yieldStressViscoPlastic=rpar(23)
!*       exponentViscoPlastic   =rpar(24)
!*       epsViscoPlastic        =rpar(25)   ! small parameter used to offset the effective strain rate 

      ! here are the names used by the getViscoPlasticViscosity macro -- what should we do about this ? 
!*       etaVP=etaViscoPlastic
!*       yieldStressVP=yieldStressViscoPlastic
!*       exponentVP=exponentViscoPlastic
!*       epsVP=epsViscoPlastic

!*  write(*,'("lineSolveNewINSVP: nuViscoPlastic=",e10.3)') nuViscoPlastic
!*  write(*,'("lineSolveNewINSVP: etaViscoPlastic=",e10.3)') etaViscoPlastic
!*  write(*,'("lineSolveNewINSVP: yieldStressViscoPlastic=",e10.3)') yieldStressViscoPlastic
!*  write(*,'("lineSolveNewINSVP: exponentViscoPlastic=",e10.3)') exponentViscoPlastic
!*  write(*,'("lineSolveNewINSVP: epsViscoPlastic=",e10.3)') epsViscoPlastic

!* -- new way:
!*  double precision pdb
!*  character *50 name
!*  integer ok,getInt,getReal
!*  ! get visco-plastic parameters
!*  nuViscoPlastic=1.   ! default value
!*  etaViscoPlastic=1.
!*  yieldStressViscoPlastic=10.
!*  exponentViscoPlastic=10.
!*  epsViscoPlastic=1.e-10   ! small parameter used to offset the effective strain rate 
!* 
!*  name ='nuViscoPlastic'
!*  ok = getReal(pdb,name,nuViscoPlastic)
!* 
!*  if( ok.eq.1 )then
!*    write(*,'("*** ut: name=",a10,", num=",e9.3)') name,nuViscoPlastic
!*  else
!*    write(*,'("*** ut: name=",a10,", NOT FOUND")') name
!*  end if

#End

      computeTemperature = 0
      if( pdeModel.eq.BoussinesqModel .or. pdeModel.eq.viscoPlasticModel )then
        computeTemperature=1
      else
        tc=uc ! give this default value to tc so we can always add a gravity term, even if there is no T equation
        thermalExpansivity=0.   ! set to zero to turn off the gravity term 
      end if

      do m=0,2
       dxv2i(m)=1./(2.*dx(m))
       dxvsqi(m)=1./(dx(m)**2)
       drv2i(m)=1./(2.*dr(m))
       drvsqi(m)=1./(dr(m)**2)
      end do

      dx0=dx(0)
      dy=dx(1)
      dz=dx(2)
      dx2i=1./(2.*dx0)
      dy2i=1./(2.*dy)
      dz2i=1./(2.*dz)
      dxsqi=1./(dx0*dx0)
      dysqi=1./(dy*dy)
      dzsqi=1./(dz*dz)

      dr2i=1./(2.*dr(0))
      ds2i=1./(2.*dr(1))
      dt2i=1./(2.*dr(2))
      drsqi=1./(dr(0)**2)
      dssqi=1./(dr(1)**2)
      dtsqi=1./(dr(2)**2)
 
      dxi=1./dx0
      dyi=1./dy
      dzi=1./dz
      dri=1./dr(0)
      dsi=1./dr(1)
      dti=1./dr(2)

      if( orderOfAccuracy.eq.4 )then
        dx12i=1./(12.*dx0)
        dy12i=1./(12.*dy)
        dz12i=1./(12.*dz)
        dxsq12i=1./(12.*dx0**2)
        dysq12i=1./(12.*dy**2)
        dzsq12i=1./(12.*dz**2)
      end if

      cd22=ad22/(nd**2) 
      cd42=ad42/(nd**2)

      cd22n=ad22n/nd     ! for the SA TM model
      cd42n=ad42n/nd

c      write(*,*) 'insLineSolve: use2ndOrderAD,ad21,cd22=',
c     & use2ndOrderAD,ad21,cd22

      dtScale=1./cfl

      if( fourthOrder.eq.1 .and. turbulenceModel.ne.noTurbulenceModel )then
        write(*,'("insLineSolve: ERROR: fourth-order only available for INS")')
        ! " '
        stop 6543
      end if

      if( turbulenceModel.eq.spalartAllmaras )then
        call getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, \
           cv1e3, cd0, cr0)
      else if( turbulenceModel.eq.kEpsilon )then

c**        call getKEpsilonParameters( cMu,cEps1,cEps2,sigmaEpsI,sigmaKI )

      else if( turbulenceModel.ne.noTurbulenceModel )then
        stop 88
      end if


      if( turbulenceModel.eq.baldwinLomax )then
         ! assign constants for baldwin-lomax
         kbl=.4
         alpha=.0168
         a0p=26.
c         ccp=1.6
         ccp=2.6619
         ckleb=0.3
         cwk=.25
c         cwk=1
      end if

      itrip = ipar(50)
      jtrip = ipar(51)
      ktrip = ipar(52)


#endMacro

