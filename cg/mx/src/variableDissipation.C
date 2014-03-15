#include "Maxwell.h"
#include "ParallelUtility.h"
#include "CompositeGridOperators.h"

// ========================================================================
//  Create the variable dissipation
// ========================================================================
int Maxwell::
buildVariableDissipation()
{
  printF("*** buildVariableDissipation...\n");

  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;

  if( variableDissipation == NULL )
    variableDissipation = new realCompositeGridFunction(cg);
  
  realCompositeGridFunction & v = *variableDissipation;
  assert( cgop!=NULL );
  v.setOperators( *cgop );
  
  Index I1,I2,I3;

  // Set v=1 at interpolation points and zero otherwise
  int grid;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
//     const intArray & ip = cg.interpolationPoint[grid];
//     realArray & vg = v[grid];
    
//     Range R = cg.numberOfInterpolationPoints(grid);
//     const int i3=cg[grid].gridIndexRange(0,2);
//     if( cg.numberOfDimensions()==2 )
//     {
//       vg(ip(R,0),ip(R,1),i3)=1.;
//     }
//     else
//     {
//       vg(ip(R,0),ip(R,1),ip(R,2))=1.;
//     }

    MappedGrid & mg = cg[grid];
    const intArray & mask = mg.mask();
    realArray & vg = v[grid];

    #ifdef USE_PPP
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      realSerialArray vLocal; getLocalArrayWithGhostBoundaries(vg,vLocal);    
    #else
      const intSerialArray & maskLocal = mask;
      realSerialArray & vLocal =vg; 
    #endif

    getIndex(mg.dimension(),I1,I2,I3);
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
    if( !ok ) continue;

    vLocal=0.;
    where( maskLocal(I1,I2,I3)<0 )
    {
      vLocal(I1,I2,I3)=1.;
    }
      

  }
  
  // Smooth The dissipation coefficient -- keep the value 1. at interpolation points

  real omega=1.;
  real omo=1.-omega, omb4=omega/4., omb6=omega/6.;

  for( int it=0; it<numberOfVariableDissipationSmooths; it++ )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      const intArray & mask = mg.mask();
      realArray & vg = v[grid];
      
      #ifdef USE_PPP
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(vg,vLocal);    
      #else
        const intSerialArray & maskLocal = mask;
        realSerialArray & vLocal =vg; 
      #endif

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3);
      if( !ok ) continue;
      
      if( cg.numberOfDimensions()==2 )
      {
	where( maskLocal(I1,I2,I3)>0 )
	{
	  vLocal(I1,I2,I3) = omo*vLocal(I1,I2,I3) + omb4*(vLocal(I1+1,I2,I3) + vLocal(I1-1,I2,I3) 
                                                        + vLocal(I1,I2+1,I3) + vLocal(I1,I2-1,I3));
	}
      }
      else
      {
	where( maskLocal(I1,I2,I3)>0 )
	{
	  vLocal(I1,I2,I3) = omo*vLocal(I1,I2,I3) + omb6*(vLocal(I1+1,I2,I3) + vLocal(I1-1,I2,I3) 
                                                        + vLocal(I1,I2+1,I3) + vLocal(I1,I2-1,I3)
                                                        + vLocal(I1,I2,I3+1) + vLocal(I1,I2,I3-1) );
	}
      }

    } // end for grid 
    real t=0.;
    v.applyBoundaryCondition(0,BCTypes::evenSymmetry,BCTypes::allBoundaries,0.,t);
    v.finishBoundaryConditions();

  } // end smooth
  
  return 0;
}

