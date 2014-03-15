#include "Oges.h"

void integrate( CompositeGrid & cg, 
                Oges & solver,
                realCompositeGridFunction & integrationWeights, 
                realCompositeGridFunction & f, 
                real & volumeIntegral, 
                real & surfaceIntegral )
{

  //======================================================================
  //     Integrate a Function on an Overlapping Grid
  //     -------------------------------------------
  //
  // Purpose -
  //  Use the integration coefficients computed from Oges and scaled by
  //  the function scaleIntegrationWeights to integrate a
  //  grid function defined on an overlapping grid
  //
  // Input -
  //   realCompositeGridFunction integrationWeights : scaled integration weights
  //       computed by Oges and scaled by scaleIntegrationWeights
  //   realCompositeGridFunction f   : function to integrate
  //
  // Output -
  //   real volumeIntegral : volume integral of f
  //   real surfaceIntegral : integral of f over all surfaces
  //
  //======================================================================

  volumeIntegral=0.;
  surfaceIntegral=0.;
  int grid;
  Index I1,I2,I3;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    I1=Range(cg[grid].dimension()(Start,axis1),cg[grid].dimension()(End,axis1));
    I2=Range(cg[grid].dimension()(Start,axis2),cg[grid].dimension()(End,axis2));
    I3=Range(cg[grid].dimension()(Start,axis3),cg[grid].dimension()(End,axis3));
    where( solver.classify[grid](I1,I2,I3)==Oges::interior || 
           solver.classify[grid](I1,I2,I3)==Oges::boundary )
    {
      volumeIntegral+=sum(f[grid](I1,I2,I3)*integrationWeights[grid](I1,I2,I3));
    }
    where( solver.classify[grid](I1,I2,I3)==Oges::ghost1 )
    {
      surfaceIntegral+=sum(f[grid](I1,I2,I3)*integrationWeights[grid](I1,I2,I3));
    }
  }
  
}

