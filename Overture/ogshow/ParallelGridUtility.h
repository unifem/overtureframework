#ifndef PARALLEL_GRID_UTILITY_H
#define PARALLEL_GRID_UTILITY_H

// ***********************************************************************************************
// ************** Parallel Utilities that know about Grids and Grid Functions *******************
// ***********************************************************************************************

//  These are kept separate from class ParallelUtility so that the Mapping's can use ParallelUtility
//  without needed to know about Grids and Grid Functions

#include "Overture.h"
#include "broadCast.h"
#include "display.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
#include <vector>
#else
#include <list.h>
#include <vector>
#endif

class InterpolationData; // forward declaration

class ParallelGridUtility
{

public:

// redistribute a grid and grid function to a new set of processors
static void redistribute(const realGridCollectionFunction & u, 
			 GridCollection & gcP,
			 realGridCollectionFunction & v, 
			 const Range & Processors );

static void redistribute(const realCompositeGridFunction & u, 
			 CompositeGrid & gcP,
			 realCompositeGridFunction & v, 
			 const Range & Processors );

// redistribute a grid to a new set of processors
static void redistribute(const GridCollection & gc,
			 GridCollection & gcP,
			 const Range & Processors );

static void redistribute(const CompositeGrid & gc,
			 CompositeGrid & gcP,
			 const Range & Processors );


static void getLocalIndexBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
						      IntegerArray & indexRangeLocal, 
						      IntegerArray & dimensionLocal, 
						      IntegerArray & bcLocal,
						      int internalGhostBC = -1); // 101102 kkc added internalGhostBC

static void getLocalBoundaryConditions( const MappedGrid & mg,
					IntegerArray & bcLocal,
					int internalGhostBC = -1 );

static void getLocalBoundaryConditions( const realMappedGridFunction & a, 
					IntegerArray & bcLocal,
					int internalGhostBC = -1 );

static int 
getLocalInterpolationData( CompositeGrid & cg, InterpolationData *&interpData );

static void 
sortLocalInterpolationPoints( CompositeGrid & cg );


};


#endif
