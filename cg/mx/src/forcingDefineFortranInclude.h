      ! forcingOptions -- these should match ForcingEnum in Maxwell.h 
      integer noForcing,magneticSinusoidalPointSource,gaussianSource,twilightZoneForcing,\
	gaussianChargeSource, userDefinedForcingOption
	integer noBoundaryForcing,planeWaveBoundaryForcing,chirpedPlaneWaveBoundaryForcing
      parameter(noForcing                =0,\
           magneticSinusoidalPointSource =1,\
           gaussianSource                =2,\
           twilightZoneForcing           =3,\
	   gaussianChargeSource          =4,\
           userDefinedForcingOption      =5 )
      ! boundary forcing options when solved directly for the scattered field:
      parameter( noBoundaryForcing              =0,\
		 planeWaveBoundaryForcing       =1,\
                 chirpedPlaneWaveBoundaryForcing=2 )
