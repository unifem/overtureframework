#ifndef FLUID_PISTON_H
#define FLUID_PISTON_H

#include "Overture.h"

// This class knows how to solve problems related to the motion of
// a solid piston next to a fluid

class FluidPiston
{
public:

FluidPiston();
~FluidPiston();


static int 
fluidSolidRiemannProblem( const real solid[], const real fluid[], real fsr[] );

};


#endif
  
