#ifndef GRID_DISTRIBUTION_H
#define GRID_DISTRIBUTION_H

// **********************************************************************
//     This class defines a parallel distribution for Overture Grids
// **********************************************************************

#include "GenericDataBase.h"
#include "OvertureTypes.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
#include <vector>
#else
#include <list.h>
#include <vector>
#endif

class GridDistribution
// This class holds information about the parallel distribution and the work-loads on a grid
{
  public:

GridDistribution(int grid=0, real workLoad=0., int rlevel=0, int mglevel=0 );

  ~GridDistribution();
  GridDistribution(const GridDistribution & x);
  GridDistribution& operator=(const GridDistribution & x);


  int getGrid() const;
  int getProcessorRange( int & pStart, int & pEnd) const;
  int getMultigridLevel() const;
  int getRefinementLevel() const;
  real getWorkLoad() const;
  int getGridPoints( int gridPoints[3] ) const;

  int setGrid( int grid );
  int setGridAndRefinementLevel( int grid, int rlevel );
  int setProcessors(int pStart, int pEnd);
  int setMultigridLevel( int mglevel );
  int setRefinementLevel( int rlevel );

  int setGridPoints( int gridPoints[3] );
  int setWorkLoadAndGridPoints( real workLoad, int gridPoints[3] );
  
  // For sorting by work-load:
  bool operator< ( const GridDistribution & x )const{ return workLoad<x.workLoad; }

  // --- The following functions are not normally called by the casual user: ---

  int determineAGoodNumberOfProcessors( int & numProc, const int minProc, const int maxProc );

  // determine the processor decomposition for a multidimensional distributed array
  void computeParallelArrayDistribution(int *dimProc );

  // utility routine:
  static void computeParallelArrayDistribution( const int nProcs, const int nDims, int *dimVec, int *dimProc );

  // return the desired minimum number of points per dimension on each processor
  static int getMinimumNumberOfPointsPerDimensionPerProcessor(int numberOfDimensions);
  static int setMinimumNumberOfPointsPerDimensionPerProcessor(int numberOfDimensions, int minNumber );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file


  static int minNumberPerDimension[3];  // holds minimum number of points per dimension on each processor

 protected:

   int pStart, pEnd;     // range of processors over which we distribute a grid
   int grid;             // grid number in the GridCollection
   real workLoad;        // estimated work load for this grid 
   int refinementLevel;  // refinement level for this grid
   int multigridLevel;   // multigrid level for this grid 
   int dims[3];          // grid dimensions in the 3 coordinate directions
};

// typedef std::vector<GridDistribution> GridDistributionList;

class GridDistributionList : public std::vector<GridDistribution>
{
  public:
  GridDistributionList();
  ~GridDistributionList();
  
  GridDistributionList( const GridDistributionList & x );
  
  GridDistributionList& operator=(const GridDistributionList & x );
  
  // display the grid distribution info:
  int display( const aString & label, FILE *file=stdout );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

};


#endif
