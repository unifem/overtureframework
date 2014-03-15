#include "DomainSolver.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "gridFunctionNorms.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{getBounds}} 
int DomainSolver::
getBounds(const realCompositeGridFunction & u, 
	  RealArray & uMin,
	  RealArray & uMax, 
	  real & uvMax)
// =========================================================================
// /Description:
//    Compute min and max of all components plus max of all values
// /u (input):
// /uMin (output) : array of minimum values.
// /uMax (output) : array of maximum values.
// /uvMax (output) : maximum of the absolute values of all components.
//\end{CompositeGridSolverInclude.tex}  
// =========================================================================
{
  GridFunctionNorms::getBounds(u,uMin,uMax);

  uvMax=max(max(fabs(uMin)),max(fabs(uMax)));
  return 0;

#undef UMIN
#undef UMAX

}

// =============================================================================================================
// /Description:
//    Compute a new gridIndexRange, dimension
//             and boundaryCondition array that will be valid for the local grid on a processor.
// 
//    Set the gid to match the ends of the local array.
//    Set the bc(side,axis) to internalGhostBC (-1 by default) for internal boundaries between processors
//
// NOTES: In parallel we cannot assume the rsxy array is defined on all ghost points -- it will not
// be set on the extra ghost points put at the far ends of the array. -- i.e. internal boundary ghost 
// points will be set but not external
// =============================================================================================================
void
getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                     IntegerArray & gidLocal, 
                                     IntegerArray & dimensionLocal, 
                                     IntegerArray & bcLocal)
{

#if 1

  //kkc 101102 just call the common code in ParallelGridUtility.h
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions(a,gidLocal,dimensionLocal,bcLocal);

#else
  // kkc old implementation replicated in ParallelUtility::getLocalIndexBoundsAndBoundaryConditions
  MappedGrid & mg = *a.getMappedGrid();
  
  const IntegerArray & dimension = mg.dimension();
  const IntegerArray & gid = mg.gridIndexRange();
  const IntegerArray & bc = mg.boundaryCondition();
  
  gidLocal = gid;
  bcLocal = bc;
  dimensionLocal=dimension;
  
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
//      printf(" axis=%i gidLocal(0,axis)=%i a.getLocalBase(axis)=%i  dimension(0,axis)=%i\n",axis,gidLocal(0,axis),
//                        a.getLocalBase(axis),dimension(0,axis));
//      printf(" axis=%i gidLocal(1,axis)=%i a.getLocalBound(axis)=%i dimension(0,axis)=%i\n",axis,gidLocal(1,axis),
//                        a.getLocalBound(axis),dimension(1,axis));
    if( a.getLocalBase(axis) == a.getBase(axis) ) 
    {
      assert( dimension(0,axis)==a.getLocalBase(axis) );
      gidLocal(0,axis) = gid(0,axis); 
      dimensionLocal(0,axis) = dimension(0,axis); 
    }
    else
    {
      gidLocal(0,axis) = a.getLocalBase(axis)+a.getGhostBoundaryWidth(axis);
      dimensionLocal(0,axis) = a.getLocalBase(axis); 
      // for internal ghost mark as periodic since these behave in the same was as periodic
      // ** we cannot mark as "0" since the mask may be non-zero at these points and assignBC will 
      // access points out of bounds
      bcLocal(0,axis) = -1; // bc(0,axis)>=0 ? 0 : -1;
    }
    
    if( a.getLocalBound(axis) == a.getBound(axis) ) 
    {
      assert( dimension(1,axis) == a.getLocalBound(axis) );
      
      gidLocal(1,axis) = gid(1,axis); 
      dimensionLocal(1,axis) = dimension(1,axis); 
    }
    else
    {
      gidLocal(1,axis) = a.getLocalBound(axis)-a.getGhostBoundaryWidth(axis);
      dimensionLocal(1,axis) = a.getLocalBound(axis);
      // for internal ghost mark as periodic since these behave in the same was as periodic
      bcLocal(1,axis) = -1; // bc(1,axis)>=0 ? 0 : -1;
    }
    
  }
#endif

}
