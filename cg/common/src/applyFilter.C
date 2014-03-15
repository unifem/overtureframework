#include "DomainSolver.h"
#include "GridFunctionFilter.h"
#include "Parameters.h"

// ===================================================================================================================
/// \brief This function applies a spatial filter to the solution in gf[gfIndex]. 
///
/// \param gfIndex : use this grid function
/// 
// ==================================================================================================================
int
DomainSolver::
applyFilter(int gfIndex)
{
  const bool applyFilter = parameters.dbase.get<bool >("applyFilter");

  if ( !applyFilter ) return 0;

  // Most of this code comes from the filtering block in sm/advanceSOS.C

  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");

  Range C = parameters.dbase.has_key("componentsToFilter") ? parameters.dbase.get<Range>("componentsToFilter") : numberOfComponents;

  // -- Apply the high-order filter -- 090823 
  #ifdef USE_PPP
   // *wdh* 091123 == this should no longer be needed ==
   // in parallel we need to extrap. interpolation neighbours again to get some points that
   // are invalidated when updateGhostBoundaries is called. (rsise2 example, -N4 -n32)
   // *wdh* 091123 const bool extrapInterpolationNeighbours = true;
  const bool extrapInterpolationNeighbours = !parameters.dbase.get<int>("useNewExtrapInterpNeighbours");
  #else
  const bool extrapInterpolationNeighbours=false;
  #endif
  real time0=getCPU();
  
  GridFunctionFilter *& gridFunctionFilter =parameters.dbase.get<GridFunctionFilter*>("gridFunctionFilter");
  assert( gridFunctionFilter!=NULL );
  GridFunctionFilter & filter = *gridFunctionFilter;
  
  const int filterFrequency = filter.filterFrequency;
  
  // High-order filters may need values assigned at interpolation neighbours and a second ghost line: 
  if( filter.filterType==GridFunctionFilter::explicitFilter &&
      filter.orderOfFilter> orderOfAccuracyInSpace &&
      !parameters.dbase.get<int >("extrapolateInterpolationNeighbours") )
    {
      printF("DomainSolver::applyFilter:ERROR:extrapolateInterpolationNeighbours should be true for this filter\n");
      OV_ABORT("error");
    }
  if( extrapInterpolationNeighbours && filter.filterType==GridFunctionFilter::explicitFilter &&
      filter.orderOfFilter> orderOfAccuracyInSpace )
    {
      // -- Extrapolate interpolation neighbours for the high-order filter and artificial dissipation ---
      //   Do this here since in parallel we can extrap interp. neighbours with no communication BUT
      //   the values may be wrong if afterward we perform an updateGhostBoundaries !
      // -- We could fix this but this would require a re-write to extrap. interp neighbours. 
      printF("DomainSolver::applyFilter: Extrapolate interpolation neighbours before the filter\n");
      extrapolateInterpolationNeighbours( gf[gfIndex], C );
      
    }
    

  if( (numberOfStepsTaken % filterFrequency) ==0  )
    {
      if( debug() & 4 )
	printF("DomainSolver::applyFilter: apply filter at step=%i\n",numberOfStepsTaken);
      
      // apply high order filter to u[gfIndex]
      // kkc !!!!! THIS IS POSSIBLY REALLY BAD, WE ASSUME THAT gfIndex+1 IS NOT CURRENTLY NEEDED, TO SAVE SPACE
      //           IN MOST CASES THIS IS OK (sm, ins, etc), BUT WATCH OUT!!!
      assert(numberOfGridFunctionsToUse>1);
      int next = (gfIndex+1)%numberOfGridFunctionsToUse;
      gridFunctionFilter->applyFilter( gf[gfIndex].u, C, gf[next].u /* work space */ );
    }

  RealArray & timing = parameters.dbase.get<RealArray>("timing");
  timing(parameters.dbase.get<int>("timeForFilter"))+=getCPU()-time0;
    
  return 0;

}
