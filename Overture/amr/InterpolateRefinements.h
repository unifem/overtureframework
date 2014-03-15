#ifndef INTERPOLATE_REFINEMENTS_H
#define INTERPOLATE_REFINEMENTS_H

#include "Overture.h"
#include "Interpolate.h"

class ListOfParentChildSiblingInfo;  // forward declaration.

class InterpolateRefinements
{
 public:

  enum
  {
    allLevels=-1
  };
   
  InterpolateRefinements(int numberOfDimensions);
  ~InterpolateRefinements();
  
  // interpolate all values on a new adaptive grid from values on an old (different) adaptive grid.
  int interpolateRefinements( const realGridCollectionFunction & uOld, 
			      realGridCollectionFunction & u,
			      int baseLevel = 1 );

  // interpolate ghost boundaries of refinements from grids at same level or below:
  int interpolateRefinementBoundaries( realGridCollectionFunction & u,
				       int levelToInterpolate = allLevels,
				       const Range & C = nullRange  );

  int interpolateRefinementBoundaries( ListOfParentChildSiblingInfo & listOfPCSInfo,
				       realGridCollectionFunction & u,
				       int levelToInterpolate = allLevels,
				       const Range & C0 = nullRange );
  
  // interpolate coarse grid points covered by finer grids:
  int interpolateCoarseFromFine( realGridCollectionFunction & u,
				 int levelToInterpolate= allLevels,
				 const Range & C = nullRange  );
  
 int interpolateCoarseFromFine(ListOfParentChildSiblingInfo & listOfPCSInfo, 
                               realGridCollectionFunction & u,
			       int levelToInterpolate= allLevels,
			       const Range & C = nullRange  );

  int get( const GenericDataBase & dir, const aString & name);

  int put( GenericDataBase & dir, const aString & name) const;

  // utility routines for Boxes:  
  static int getIndex( const BOX & box, Index Iv[3] );
  static int getIndex( const BOX & box, int side, int axis, Index Iv[3]);
  static Box intersects( const Box & box1, const Box & box2 );
  static Box buildBox(Index Iv[3] );
  static Box buildBaseBox( MappedGrid & mg );
  // Build a box for the portion of the array u(Iv) that lives on a given processor
  static Box buildBox(realArray & u, Index Iv[3] , int processor);

  int setOrderOfInterpolation( int order );
  int setNumberOfGhostLines( int numberOfGhostLines );
  

  void printStatistics( FILE *file=NULL ) const;
  
  int debug;
  static FILE *debugFile;  // this allows other classes to write to the debugFile
  
 protected:
  int numberOfDimensions;

  Interpolate interp;
  InterpolateParameters interpParams;
  IntegerArray refinementRatio;
  int numberOfGhostLines;

  real timeForCoarseFromFine;
  real timeForRefinementBoundaries;
  real timeForRefinements;
  real timeForBoundaryCoarseFromFine;
  
  bool boxWasAdjustedInPeriodicDirection(BOX & box, GridCollection & gc, int baseGrid, int level,
					 int & periodicDirection, int & periodShift  );

  void openDebugFile();
  int myid;

};

#endif
