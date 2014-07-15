#ifndef BOUNDARY_DATA_H
#define BOUNDARY_DATA_H

#include "Overture.h"
#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

// Here is a little class used to hold the boundary data for boundary conditions
class BoundaryData
{
public:

BoundaryData();

~BoundaryData();

BoundaryData(const BoundaryData & x);

BoundaryData & operator=(const BoundaryData & x);

typedef RealArray *BoundaryDataArray[2][3];
BoundaryDataArray boundaryData;

enum VariableCoefficientBoundaryConditionEnum
{
  variableCoefficientTemperatureBC=1,                                       // bit 0 
  variableCoefficientOutflowBC    = variableCoefficientTemperatureBC << 1   // bit 1 
};
    
// Return the bit flag which indicates which variable coefficient boundary conditions are defined.
int 
hasVariableCoefficientBoundaryCondition(int side, int axis) const{ 
  return pHasVariableCoefficientBoundaryCondition[(side)+2*(axis)]; }

RealArray& 
getVariableCoefficientBoundaryConditionArray( VariableCoefficientBoundaryConditionEnum option, int side, int axis );

// Here is a dbase where we can save additional parameters
DataBase dbase;

protected:

// This is a bit flag indicating which variable coefficient boundary conditions exist on a face:
int pHasVariableCoefficientBoundaryCondition[6];

};

#endif

