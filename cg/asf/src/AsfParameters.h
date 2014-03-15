#ifndef ASF_PARAMETERS
#define ASF_PARAMETERS

#include "Parameters.h"
// Here are the run time and PDE parameters
class AsfParameters : public Parameters
{
public:

enum BoundaryConditions {
  subSonicInflow=8,
  subSonicOutflow=10,
  convectiveOutflow=14,  
  tractionFree=15
};

enum AlgorithmVariation
{
  defaultAlgorithm=0,
  densityFromGasLawAlgorithm
};


// Here are some standard test problems
enum TestProblems
{
  standard,
  laminarFlame
};

AsfParameters(const int & numberOfDimensions0=3);
~AsfParameters();

// virtual int 
// conservativeToPrimitive(GridFunction & gf, int gridToConvert=-1, int fixupUnsedPoints=false);

virtual int 
displayPdeParameters(FILE *file = stdout );

virtual int 
getDerivedFunction( const aString & name, const realCompositeGridFunction & u,
                    realCompositeGridFunction & v, const int component, 
                    Parameters & parameters);
virtual int
getDerivedFunction( const aString & name, const realMappedGridFunction & u, 
                    realMappedGridFunction & vIn, const int grid,
                    const int component, Parameters & parameters);

// compute the normal force on a boundary (for moving grid problems)
virtual 
int getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar );

// virtual int 
// primitiveToConservative(GridFunction & gf, int gridToConvert=-1, int fixupUnsedPoints=false);

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

  virtual int
  updatePDEparameters();

  virtual
  int getComponents( IntegerArray &components );

  virtual 
  int setDefaultDataForABoundaryCondition(const int & side,
					  const int & axis,
					  const int & grid,
					  CompositeGrid & cg);

virtual 
bool isMixedBC( int bc );

};

#endif
