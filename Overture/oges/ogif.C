#include "Oges.h"
#include "SparseRep.h"

void Oges::
integrate(realCompositeGridFunction & integrationWeights, 
	  realCompositeGridFunction & f , 
	  real & volumeIntegral, real & surfaceIntegral )
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
  //   realCompsoiteGridFunction integrationWeights : scaled integration weights
  //       computed by Oges and scaled by scaleIntegrationWeights
  //   realCompsoiteGridFunction f   : function to integrate
  //
  // Output -
  //   real volumeIntegral : volume integral of f
  //   real surfaceIntegral : integral of f over all surfaces
  //
  //======================================================================

  real area0=0.;
  real sarea0=0.;
  
  volumeIntegral=0.;
  surfaceIntegral=0.;
  int grid;
  Index I1,I2,I3;
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;

    I1=Range(c.dimension()(Start,axis1),c.dimension()(End,axis1));
    I2=Range(c.dimension()(Start,axis2),c.dimension()(End,axis2));
    I3=Range(c.dimension()(Start,axis3),c.dimension()(End,axis3));
    where( classifyX(I1,I2,I3)==SparseRepForMGF::interior || classifyX(I1,I2,I3)==SparseRepForMGF::boundary )
    {
      volumeIntegral+=sum(f[grid](I1,I2,I3)*integrationWeights[grid](I1,I2,I3));
      area0+=sum(integrationWeights[grid](I1,I2,I3));
    }
    where( classifyX(I1,I2,I3)==SparseRepForMGF::ghost1 )
    {
      surfaceIntegral+=sum(f[grid](I1,I2,I3)*integrationWeights[grid](I1,I2,I3));
      sarea0+=sum(integrationWeights[grid](I1,I2,I3));
    }
  }
  if( Oges::debug & 4 )
    printf(" ogif: Integrating a function on a overlapping grid:\n"
           " area(volume)          =%16.10e \n"
           " surface area          =%16.10e \n"
           " volume integral of f  =%16.10e \n"
           " surface integral of f =%16.10e \n",
	   area0,sarea0,volumeIntegral,surfaceIntegral);
  
}
