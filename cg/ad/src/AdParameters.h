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

virtual int 
updateUserDefinedCoefficients(GenericGraphicsInterface & gi);

virtual
int userDefinedDeformingSurface( DeformingBodyMotion & deformingBody,
				 real t1, real t2, real t3, 
				 GridFunction & cgf1,
				 GridFunction & cgf2,
				 GridFunction & cgf3,
				 int option );

virtual
void userDefinedDeformingSurfaceCleanup( DeformingBodyMotion & deformingBody );

virtual
int userDefinedDeformingSurfaceSetup( DeformingBodyMotion & deformingBody );


// virtual int
// setUserDefinedParameters();

};

#endif
