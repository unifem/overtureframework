//                                                -*- c++ -*-
//
// DeformingGrid:
// ==============
//
//     Handles a single deforming component grid
//     "Kinematics"= the non-physics aspects (=just grids)
//
//
//    UNDER REVISION currently

#ifndef DEFORMING_GRID_H
#define DEFORMING_GRID_H

#include "Overture.h"
#include "GenericGraphicsInterface.h"

//..Forward definitions
class HyperbolicMapping;
class GenericGraphicsInterface;
//class DeformingGridGenerationInformation;

class DeformingGrid {
public:
  //..Constructor / Destructor
  DeformingGrid(int numberOfTimeLevels,
		GenericGraphicsInterface *pGIDebug00 =NULL,
		int debug00                          =0);

  ~DeformingGrid();

  //..Services 
  //-----from old "DeformingComponentGridInformation"
  //  int storeNewTimeLevel( const real newTime, HyperbolicMapping & mapping);

  //void getNewMapping( const real time0,    //OBSOLETE
  //		      Mapping & surface,
  //		      HyperbolicMapping * &pNewMapping );

  void getNewMapping( const real time0,                 // without surface.
		      HyperbolicMapping * &pNewMapping );

  void getCurrentMap( real & time0, HyperbolicMapping*  &pCurrentMapping );
  
  void getTimeLevelMapPointer( const int offset, real & time0, 
			       HyperbolicMapping*   &pMap);


  void getCurrentGridArray( real & time0, realArray*  &pCurrentGrid );
  void getTimeLevelGridArrayPointer( const int offset, 
  				     real & time0, 
  				     realArray*  &pGrid);
  

  void regenerateGrid( HyperbolicMapping *pHyper,
		       const int grid, 
		       CompositeGrid & cg);

  //
  //------GRID VELOCITY & ACCELERATION
  //

  //..User interface
  void getVelocity( const real time0, 
		   const int grid, 
		   CompositeGrid & cg,
		   realArray & gridVelocity);

  void getAcceleration( const real time0, 
			const int grid, 
			CompositeGrid & cg,
			realArray & gridVelocity);
  


  //..Internal routines
  //....Compute vel. of gridpoints in THREE conseq.(in time) mappings
  void computeGridVelocity( real vTime,
			real t0, realArray const & map0,
			real t1, realArray const & map1,
			real t2, realArray const & map2,
			realArray & gridVelocity,
		        Index & I1, Index & I2, Index & I3);


  
  //....Compute vel. of gridpoints in TWO conseq.(in time) mappings
  void computeGridVelocity( real vTime,
			real t0, realArray const & map0,
			real t1, realArray const & map1,
			realArray & gridVelocity,
		        Index & I1, Index & I2, Index & I3);

  //....Compute ACCEL. of gridpoints in THREE conseq.(in time) mappings
  void computeGridAcceleration( real vTime,
			real t0, realArray const & map0,
			real t1, realArray const & map1,
			real t2, realArray const & map2,
			realArray & gridVelocity);
  
  //..Compute ACCEL. of gridpoints in TWO conseq.(in time) mappings
  void computeGridAcceleration( real vTime,
			real t0, realArray const & map0,
			real t1, realArray const & map1,
			realArray & gridVelocity);


  //.............................................................................

  int grid;                 // the Component grid number of this DeformingGrid
  
  enum RemapType { 
    hyperbolicRemap,
    lagrangianRemap,
    userDefinedRemap
  };

  //REGENERATION PARAMETERS -- moot, use params in pHyper on entry
  RemapType  remapType;
  //DeformingGridGenerationInformation *pGridGenerationInformation;

  //private:
  //TIME HISTORY of grid points
  int maximumNumberOfTimeLevels;
  int currentNumberOfTimeLevels;          
  int iMostRecentTimeLevel;
  real *timeHistory;
  HyperbolicMapping *mappingHistoryList;  // type should be more general
                                          // --> just  _Mapping_  

  realArray  *gridHistoryList; // keep also ghostbdries

  //protected:

  int numberOfDimensions;  // 2 or 3 dimensional problem
  int current;             // position of current solution in arrays???
  int numberOfSteps;       // number of time steps taken
  int numberSaved;         // number of different times we have saved data
  int maximumNumberToSave; // save at most this many previous time values


  RealArray time;          // arrays of times for which we know data.

  const Range R;           // R=Range(0,2) 

  // How many components & properties for those grids
  int                                 numberOfTimeLevelsStored;
  //int                                 numberOfComponentGrids;
  //int                                 numberOfMyGrids; 

  //DEBUG DATA
  int debug;
  GenericGraphicsInterface *pGIDebug;
  MappingInformation       *pMapInfoDebug;

  //DEBUG CODE
  void getPastLevelGrid(   const int grid00,   CompositeGrid & cg,
			   int iLevel, realArray & gridVelocity);

  void simpleGetVelocity( const real vTime, 
			  const int grid00, 
			  CompositeGrid & cg,
			  realArray & gridVelocity);

  void simpleGetVelocityAndPoints( const real vTime, 
			  const int grid00, 
			  CompositeGrid & cg,
			  realArray & gridVelocity,
			  realArray & xpoints1, realArray & xpoints2); //debug

  int mappingSerialNumber;
  void createMappingName( real time00, aString & newMappingName);


};

#endif
