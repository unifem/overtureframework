! -- arrays for variable material properties --
      integer constantMaterialProperties
      integer piecewiseConstantMaterialProperties
      integer variableMaterialProperties,ndMatProp
      parameter( constantMaterialProperties=0 )
      parameter( piecewiseConstantMaterialProperties=1 )
      parameter( variableMaterialProperties=2 )
      integer materialFormat
      integer matIndex(nd1a:nd1b,nd2a:nd2b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,0:*)
