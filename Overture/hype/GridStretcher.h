#ifndef GRID_STRETCHER_H
#define GRID_STRETCHER_H

#include "Mapping.h"
#include "GenericGraphicsInterface.h"

class DataPointMapping;
class DialogData;
class MappingInformation;
class StretchTransform;

// ==================================================================================
/// \brief Define an interface for stretching grids (used with MappingBuilder).
// ==================================================================================
class GridStretcher
{
  public:

  enum ReturnValueEnum
  {
    answerNotProcessed=0,
    parametersChanged=1,
    gridWasChanged=2
  };

  enum PickingOptionsEnum
  {
    pickToDoNothing,
    pickToStretchInDirection1,
    pickToStretchInDirection2,
    pickToStretchInDirection3,
  };

  GridStretcher(int domainDimension, int rangeDimension);
  ~GridStretcher();

  int buildDialog(DialogData & dialog);

int 
update(aString & answer, DialogData & dialog, MappingInformation & mapInfo, StretchTransform & stretchedMapping );

int 
update(aString & answer, DialogData & dialog, MappingInformation & mapInfo,
       realArray & x,  // source grid points including ghost points
       const IntegerArray & gridIndexRange,     // identifies boundaries
       const IntegerArray & projectIndexRange,  // identifies which points to project for surface grids
       DataPointMapping & dpm,   // the resulting stretched grid
       Mapping *surface=NULL // for projecting onto surface grids
  );


//    int update(aString & answer, DialogData & dialog, MappingInformation & mapInfo,
//  	     realArray & x,  // source grid points including ghost points
//  	     const IntegerArray & gridIndexRange,     // identifies boundaries
//  	     const IntegerArray & projectIndexRange,  // identifies which points to project (surface grids)
//  	     DataPointMapping & dpm,   // the resulting stretched grid
//  	     Mapping *surface=NULL // for projecting onto surface grids
//              );

 protected:

int 
update(aString & answer, DialogData & dialog, MappingInformation & mapInfo,
       realArray *px,  // source grid points including ghost points
       IntegerArray const *pgridIndexRange,     // identifies boundaries
       IntegerArray const *pprojectIndexRange,  // identifies which points to project for surface grids
       DataPointMapping *dpm,   // the resulting stretched grid
       Mapping *surface =NULL , // for projecting onto surface grids
       StretchTransform *stretchedMapping =NULL  );

  int periodicUpdate( realArray & x, const IntegerArray & indexRange, Mapping & map );

int 
applyStretching( StretchTransform & stretchedDPM );

bool
checkForStretchCommands(const aString & answer, GenericGraphicsInterface & gi, DialogData & dialog );

  PickingOptionsEnum pickingOption;

  int domainDimension, rangeDimension;
  int stretchID;
  int numberOfStretch;
  RealArray stretchParams;
  int projectStretchedGridOntoReferenceSurface;
  int gridIsStretched;

  real defaultWeight, defaultExponent;
};


#endif
