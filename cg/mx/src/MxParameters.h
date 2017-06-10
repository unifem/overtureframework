#ifndef MX_PARAMETERS
#define MX_PARAMETERS

// Parameters for the advection-diffusion solver

#include "Parameters.h"
// Here are the run time and PDE parameters
class MxParameters : public Parameters
{
public:


MxParameters(const int & numberOfDimensions0=3);
~MxParameters();

virtual int
setParameters(const int & numberOfDimensions0=2, 
	      const aString & reactionName =nullString);

};

#endif
