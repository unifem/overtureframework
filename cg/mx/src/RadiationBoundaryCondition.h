#ifndef RADIATION_BOUNDARY_CONDITION
#define RADIATION_BOUNDARY_CONDITION


#include "Overture.h"

class RadiationKernel;
class OGFunction;


class RadiationBoundaryCondition
{
public:

RadiationBoundaryCondition(int orderOfAccuracy=4);
~RadiationBoundaryCondition();


int initialize( MappedGrid & mg, 
		int side, int axis,
                int nc1=0, int nc2=0, 
		real c_=1., real period_=-1.,  
		int numberOfModes_=-1, 
		int orderOfTimeStepping_=-1, int numberOfPoles_=-1 );

int assignBoundaryConditions( realMappedGridFunction & u, real t, real dt,
			   realMappedGridFunction & u2 );

int setOrderOfAccuracy(int orderOfAccuracy );

static int debug;
static real cpuTime;

OGFunction *tz;

protected:

RadiationKernel *radiationKernel; 

int nc1,nc2,numberOfGridPoints, numberOfModes, orderOfTimeStepping, numberOfPoles;
double period, c, radius; 

int rside,raxis;   // apply BC on this face

int orderOfAccuracy,numberOfDerivatives,numberOfTimeLevels,currentTimeLevel;
real currentTime;
RealArray uSave, uxSave;

};

#endif
