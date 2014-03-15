#include "Oges.h"

// Here are some useful macros

#define ForBoundary(side,axis)   for( axis=0; axis<og.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

void assignRHS( Oges & og, CompositeGrid & cg, realCompositeGridFunction & f )
{
  //================================================================
  //
  //   Assign the right-hand-side, f
  // 
  //================================================================

  int n,axis,side;
  Range R[3];
  Index I1,I2,I3, Ig1,Ig2,Ig3;
  n=0;   // only 1 component in this example

  for( int grid=0; grid<og.numberOfComponentGrids(); grid++ )
  {
    getIndex( cg[grid].indexRange(),I1,I2,I3 );  // get Index's for interior points

    // assign interior points:
    where( og.classify[grid](I1,I2,I3) > 0 )
      f[grid](I1,I2,I3,n)=1.;

    // loop over boundaries
    ForBoundary(side,axis)
    {
      if( cg[grid].boundaryCondition()(side,axis) > 0 )
      { // In this example bc's are assigned at ghost points
        getGhostIndex(cg[grid].gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points
        where( og.classify[grid](Ig1,Ig2,Ig3) > 0 )
          f[grid](Ig1,Ig2,Ig3,n)=2.;
      }
    }
  }
}
