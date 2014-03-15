      ! forcingOptions -- these should match ForcingEnum in Maxwell.h 
      integer noForcing,magneticSinusoidalPointSource,gaussianSource,twilightZoneForcing,planeWaveBoundaryForcing,\
	gaussianChargeSource, userDefinedForcingOption
      parameter(noForcing                =0,\
           magneticSinusoidalPointSource =1,\
           gaussianSource                =2,\
           twilightZoneForcing           =3,\
           planeWaveBoundaryForcing      =4,\
	   gaussianChargeSource          =5,\
           userDefinedForcingOption      =6 )
