#ifndef SM_PARAMETERS
#define SM_PARAMETERS

// Parameters for Cgsm, the solid-mechanics solver

#include "Parameters.h"
// Here are the run time and PDE parameters
class SmParameters : public Parameters
{
public:

enum PDEModel
{
  linearElasticity=0,
  nonlinearMechanics,    // for the future 
  numberOfPDEModels
};
static aString PDEModelName[numberOfPDEModels+1];

enum PDEVariation
{
  nonConservative=0,  // Bill's non-conservative
  conservative,       // Daniel's conservative
  godunov,            // Don's Godunov method
  hemp,               // Jeff's version of the Hemp method
  numberOfPDEVariations
};
static aString PDEVariationName[numberOfPDEVariations+1];

enum TimeSteppingMethodSm  // time stepping methods for SM
{
  defaultTimeStepping=0,
  adamsBashforthSymmetricThirdOrder,
  rungeKuttaFourthOrder,
  stoermerTimeStepping, 
  modifiedEquationTimeStepping,
  forwardEuler, 
  improvedEuler,
  adamsBashforth2,
  adamsPredictorCorrector2,
  adamsPredictorCorrector4
};


enum BoundaryConditionEnum  // these should match with bcDefineInclude.h 
{
  interpolation=0,
  displacementBC,
  tractionBC,
  slipWall,       // n.u=0 and tau.sigma.n=0 
  symmetry,
  interfaceBoundaryCondition,   // for the interface between two regions with different properties
  abcEM2,         // absorbing BC, Engquist-Majda order 2 
  abcPML,         // perfectly matched layer
  abc3,           // future absorbing BC
  abc4,           // future absorbing BC
  abc5,           // future absorbing BC
  rbcNonLocal,    // radiation BC, non-local
  rbcLocal,       // radiation BC, local
  dirichletBoundaryCondition,
  numberOfBCNames
};
static aString bcName[numberOfBCNames];

SmParameters(const int & numberOfDimensions0=3);
~SmParameters();

virtual int 
chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg);

virtual int 
displayPdeParameters(FILE *file = stdout );

virtual aString 
getTimeSteppingName() const;

virtual
int getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, RealArray & ua, 
				const Index & I1, const Index &I2, const Index &I3, int numberOfTimeDerivatives = 0 );

virtual int 
initializeTimings();

bool isFirstOrderSystem() const;

bool isSecondOrderSystem() const;

int 
readCoefficients( DialogData & dialog,const aString & answer, const aString & name, RealArray & coeff );

virtual int
saveParametersToShowFile();

virtual int
setParameters(const int & numberOfDimensions0=2, 
 	      const aString & reactionName =nullString);
virtual int 
setPdeParameters(CompositeGrid & cg,
                  const aString & command = nullString,
                  DialogData *interface =NULL );

virtual int 
setTwilightZoneFunction(const TwilightZoneChoice & choice,
                        const int & degreeSpace =2, 
                        const int & degreeTime =1 );

virtual int 
updateToMatchGrid(CompositeGrid & cg, IntegerArray & sharedBoundaryCondition );

virtual
int updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg);


};

#endif
