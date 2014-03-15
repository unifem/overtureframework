#ifndef GRID_STATISTICS_H
#define GRID_STATISTICS_H

#include "CompositeGrid.h"

/// =========================================================================================================
/// This class can be use to compute and output grid statstics such as cell volumes, areas, arclengths as well as 
/// numbers of grid points, boundary conditions etc.
/// =========================================================================================================
class GridStatistics
{
public:

enum MaskOptionEnum
{
  ignoreMask,
  positiveMask,
  nonZeroMask
};


static int
checkForNegativeVolumes(GridCollection & gc, int numberOfGhost=0, FILE *file=stdout );

static int
checkForNegativeVolumes(MappedGrid & mg, int numberOfGhost=0, FILE *file=stdout, int grid=0 );


static void 
getGridSpacing(MappedGrid & mg, real dsMin[3], real dsAve[3], real dsMax[3], MaskOptionEnum maskOption=ignoreMask );

static void 
getGridSpacingAndNumberOfPoints(MappedGrid & mg, real dsMin[3], real dsAve[3], real dsMax[3],
                                int & numberOfPoints, MaskOptionEnum maskOption=ignoreMask );

static void 
getNumberOfPoints(GridCollection & gc, int & totalNumberOfGridPoints, MaskOptionEnum maskOption=ignoreMask );

static void 
getNumberOfPoints(MappedGrid & mg, int & numberOfPoints, MaskOptionEnum maskOption=ignoreMask );

static void 
printGridStatistics(CompositeGrid & cg, FILE *file=stdout );

static void
printGridStatistics(GridCollection & gc, FILE *file=stdout );

static void 
printGridStatistics(MappedGrid & mg, FILE *file=stdout, int grid=0, int *ipar=NULL, real *rpar=NULL );

private:

// The constructor should never be called
GridStatistics();
~GridStatistics();

};


#endif
