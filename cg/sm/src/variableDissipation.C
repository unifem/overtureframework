#include "Cgsm.h"


// ========================================================================
//  Create the variable dissipation
// ========================================================================
int Cgsm::
buildVariableDissipation()
{
  printF("*** buildVariableDissipation...\n");

  if( variableDissipation == NULL )
    variableDissipation = new realCompositeGridFunction(cg);
  
  realCompositeGridFunction & v = *variableDissipation;
  

  v=0.;
  // Set v=1 at interpolation points
  int grid;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const intArray & ip = cg.interpolationPoint[grid];
    realArray & vg = v[grid];
    
    Range R = cg.numberOfInterpolationPoints(grid);
    const int i3=cg[grid].gridIndexRange(0,2);
    if( cg.numberOfDimensions()==2 )
    {
      vg(ip(R,0),ip(R,1),i3)=1.;
    }
    else
    {
      vg(ip(R,0),ip(R,1),ip(R,2))=1.;
    }

  }
  
  // Smooth The dissipation coefficient -- keep the value 1. at interpolation points
  Index I1,I2,I3;
  const int numberOfSmooths=10;
  for( int it=0; it<numberOfSmooths; it++ )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      const intArray & mask = mg.mask();
      realArray & vg = v[grid];
      
      getIndex(mg.gridIndexRange(),I1,I2,I3);

      real omega=1.;
      real omo=1.-omega, omb4=omega/4., omb6=omega/6.;
      if( cg.numberOfDimensions()==2 )
      {
	where( mask(I1,I2,I3)>0 )
	{
	  vg(I1,I2,I3) = omo*vg(I1,I2,I3) + omb4*(vg(I1+1,I2,I3) + vg(I1-1,I2,I3) 
                                                + vg(I1,I2+1,I3) + vg(I1,I2-1,I3));
	}
      }
      else
      {
	where( mask(I1,I2,I3)>0 )
	{
	  vg(I1,I2,I3) = omo*vg(I1,I2,I3) + omb6*(vg(I1+1,I2,I3) + vg(I1-1,I2,I3) 
                                                + vg(I1,I2+1,I3) + vg(I1,I2-1,I3)
                                                + vg(I1,I2,I3+1) + vg(I1,I2,I3-1) );
	}
      }

    }
  } // end smooth
  
  return 0;
}

