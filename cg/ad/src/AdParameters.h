#ifndef AD_PARAMETERS
#define AD_PARAMETERS

// Parameters for the advection-diffusion solver

#include "Parameters.h"
// Here are the run time and PDE parameters
class AdParameters : public Parameters
{
public:

enum BoundaryConditions 
{
  mixedBoundaryCondition=30
};


  AdParameters(const int & numberOfDimensions0=3);
  ~AdParameters();

virtual int
chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg);


virtual int 
displayPdeParameters(FILE *file = stdout );

virtual int 
setDefaultDataForABoundaryCondition(const int & side,
				    const int & axis,
				    const int & grid,
				    CompositeGrid & cg);
virtual int
saveParametersToShowFile();

virtual int
setParameters(const int & numberOfDimensions0=2, 
	      const aString & reactionName =nullString);
virtual int 
setPdeParameters(CompositeGrid & cg, const aString & command = nullString,
                 DialogData *interface =NULL );

virtual int 
setTwilightZoneFunction(const TwilightZoneChoice & choice,
                        const int & degreeSpace =2, 
                        const int & degreeTime =1 );


// virtual int
// setUserDefinedParameters();

};

#endif
