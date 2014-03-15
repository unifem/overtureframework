#ifndef CAN_INTERPOLATE_H
#define CAN_INTERPOLATE_H

#include "Overture.h"

namespace CanInterpolate
{


// This struct holds the data needed to make a query to canInterpolate:
struct CanInterpolateQueryData
{
CanInterpolateQueryData(){ rv[0]=rv[1]=rv[2]=0.; id=i=grid=donor=0; } //
real rv[3]; // location in donor grid  -- note: put doubles first for memory alignment

int id;     // index to an array of these objects -- used for testing --> can eventually be removed
int i;      // index into the interpolation array's 
int grid;   // receptor grid
int donor;  // donor grid

CanInterpolateQueryData& operator=(const CanInterpolateQueryData& x);

};

// here is the info that we send back from the query to canInterpolate
struct CanInterpolateResultData
{
CanInterpolateResultData(){ id=width=0; il[0]=il[1]=il[2]=0; } //
int id;     // id to match the one in a CanInterpolateQueryData object --> can eventually be removed
int width;  // width of valid interpolation (0=cannot interpolate)
int il[3];  // interpolation location

CanInterpolateResultData& operator=(const CanInterpolateResultData& x);

};


// parallel canInterpolate function -- query a list of points from different grids and donors.
int canInterpolate( CompositeGrid & cg, 
		    int numberToCheck, 
		    CanInterpolateQueryData *cid, 
		    CanInterpolateResultData *cir,
                    const int numberOfValidGhost=0 );


// *OLD VERSION* --this has a memory leak
int canInterpolateOld( CompositeGrid & cg, 
		    int numberToCheck, 
		    CanInterpolateQueryData *cid, 
		    CanInterpolateResultData *cir,
                    const int numberOfValidGhost=0 );


// used by canInterpolate to transfer interp data to processors where it is needed 
int 
transferInterpDataForAMR(CompositeGrid & cg, 
                         intSerialArray *ipLocal, intSerialArray *ilLocal, realSerialArray *ciLocal);


// query if a point can interpolate
Logical cgCanInterpolate(
  const Integer&      k10,
  const Integer&      k20,
  const RealArray&    r,
  const IntegerArray& ok,
  const IntegerArray& useBackupRules,
  const Logical       checkForOneSided,
  const CompositeGrid & cg,
  const IntegerArray & mask,
  int *pValidRange=NULL );

void 
getInterpolationStencil(const Integer&      k10,
			const Integer&      k20,
			const RealArray&    r,
			const IntegerArray& interpolationStencil,
			const IntegerArray& useBackupRules,
                        const CompositeGrid & cg,
                        int *pValidRange=NULL );


};



#endif
