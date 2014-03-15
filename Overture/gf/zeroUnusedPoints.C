#include "Overture.h"
#include "SparseRep.h"

int
zeroUnusedPoints(realMappedGridFunction & u,
                 IntegerDistributedArray & classify)
// =================================================================
// /Description:
//    Zero out the unused points on the grid function u.
//   This is a bit tricky since we need to zero out ghost points as well
//   but the mask array is not defined at ghost points (this will get fixed sometime).
//   To get arround this for now we use the classify array from a sparse matrix.
// /u (input) : grid function, assumed to be of the form u(I1,I2,I3,c0,c1) (i.e. at
//    most two component dimensions in positions 3 and 4. 
// /classify (input) : classify array from a sparse coefficient grid function.
// =================================================================
{
  MappedGrid & mg = *u.getMappedGrid();
  
  Index I1,I2,I3;
  getIndex(mg.dimension,I1,I2,I3);
  for( int c1=u.getComponentBase(1); c1<=u.getComponentBound(1); c1++ )
  for( int c0=u.getComponentBase(0); c0<=u.getComponentBound(0); c0++ )
  {
    where( classify(I1,I2,I3) == SparseRepForMGF::unused )
    {
      u(I1,I2,I3,c0,c1)=0.; 
    }
  }
  return 0;
}


int
zeroUnusedPoints(realGridCollectionFunction & u, realGridCollectionFunction & coeff )
// =================================================================
// /Description:
//    Zero out the unused points on the grid function u.
//   This is a bit tricky since we need to zero out ghost points as well
//   but the mask array is not defined at ghost points (this will get fixed sometime).
//   To get arround this for now we use the classify array from a sparse matrix.
// /u (input) : grid function, assumed to be of the form u(I1,I2,I3,c0,c1) (i.e. at
//    most two component dimensions in positions 3 and 4. 
// /coeff (input) : a Coefficient matrix from which the classify array will be extracted.
// =================================================================
{
  assert( coeff.getIsACoefficientMatrix() );
  for( int grid=0; grid<u.numberOfComponentGrids(); grid++ )
  {
    zeroUnusedPoints(u[grid],coeff[grid].sparse->classify);
  }
  return 0;
}
