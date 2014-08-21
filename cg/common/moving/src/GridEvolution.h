#ifndef GRID_EVOLUTION_H
#define GRID_EVOLUTION_H

// This Class is used to keep a time history of a deforming grid
// so that time derivatives of the grid motion can be computed.

#include "Overture.h"

class GridEvolution
{
public:

GridEvolution();

~GridEvolution();

// add a new grid at time t
int addGrid( const realArray & x, real t );

// Display properties of the class.
int display( FILE *file = stdout ) const;

// get the grid from time t
int getGrid( realArray & x, const real t ) const;

int getNumberOfTimeLevels() const;

int getVelocity( real t, realSerialArray & gridVelocity, 
                 const Index &I1, const Index &I2, const Index &I3 ) const;

int getAcceleration( real t, realSerialArray & gridVelocity, 
                 const Index &I1, const Index &I2, const Index &I3 ) const;

int getAccelerationOrderOfAccuracy() const;
int getVelocityOrderOfAccuracy() const;

int setAccelerationOrderOfAccuracy( int order );
int setVelocityOrderOfAccuracy( int order );

// interactive update
int update(GenericGraphicsInterface & gi );

int get(const GenericDataBase & subDir, const aString & name); 

int put( GenericDataBase & dir, const aString & name) const;

protected:

// For testing we can return a specified velocity and acceleration:
enum SpecifiedMotionEnum
{
  noSpecifiedMotion,
  linearMotion
} specifiedMotion;

int maximumNumberOfTimeLevels;
int numberOfTimeLevels;
int current;

int accelerationOrderOfAccuracy;  // order of accuracy for the acceleration computation
int velocityOrderOfAccuracy;     // order of accurcy for the velocity computation

ListOfRealDistributedArray gridList;
RealArray time;

real specifiedMotionParameters[10];

// on restart, we need to load in the previous grids from the file
// to initialize the object.  b/c gridList makes shallow copies of the grids,
// we have to store the grids we read from file here until we no longer need them
RealDistributedArray* restartGrids;

int remainingRestartGrids;

public:

// debug flag: 
static int debug;

};


#endif
