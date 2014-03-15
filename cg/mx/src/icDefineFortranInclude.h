!
! define initial condition options (see Maxwell.h :  enum InitialConditionEnum)
!
      integer defaultInitialCondition,\
      planeWaveInitialCondition,\
      gaussianPlaneWave,\
      gaussianPulseInitialCondition,\
      squareEigenfunctionInitialCondition, \
      annulusEigenfunctionInitialCondition,\
      zeroInitialCondition,\
      planeWaveScatteredFieldInitialCondition,\
      planeMaterialInterfaceInitialCondition,\
      gaussianIntegralInitialCondition,\
      twilightZoneInitialConditions,\
      userDefinedInitialConditionOption

      parameter( defaultInitialCondition              =0,\
	     planeWaveInitialCondition                =1,\
	     gaussianPlaneWave                        =2,\
	     gaussianPulseInitialCondition            =3,\
	     squareEigenfunctionInitialCondition      =4,\
	     annulusEigenfunctionInitialCondition     =5,\
	     zeroInitialCondition                     =6,\
	     planeWaveScatteredFieldInitialCondition  =7,\
	     planeMaterialInterfaceInitialCondition   =8,\
	     gaussianIntegralInitialCondition         =9,\
		 twilightZoneInitialConditions        =10,\
             userDefinedInitialConditionOption        =11)
