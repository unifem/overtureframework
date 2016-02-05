#ifndef CNS_PARAMETERS
#define CNS_PARAMETERS

#include "Parameters.h"
// Here are the run time and PDE parameters
class CnsParameters : public Parameters
{
public:

enum BoundaryConditions 
{
  inflowWithVelocityGiven=2,
  outflow=5,
  superSonicInflow=6,
  superSonicOutflow=7,
  subSonicInflow=8,
  subSonicOutflow=10,
  convectiveOutflow=14,  
  tractionFree=15,
  farField=16 // far field BC treats inflow and outflow cases 
};

enum PDE 
{
  compressibleNavierStokes=2,
  compressibleMultiphase=5
};
enum GodunovVariation
{
  fortranVersion,        // Don's fortran version
  cppVersionI,           // Don's C++ version
  cppVersionII,          // dlb's C++ version (to be removed)
  multiComponentVersion, // Jeff Banks version
  multiFluidVersion      // Don's multifluid version
};

// int conservativeGodunovMethod;       // 0=fortran, 1=C++, 2=DLB
enum EquationOfStateEnum
{
  idealGasEOS=0,
  jwlEOS=1,            // EOS for a solid/gas mixture
  mieGruneisenEOS=2,   // EOS for high temperature solids
  userDefinedEOS=3,    // user defined EOS (userDefinedEOS.f)
  stiffenedGasEOS=4,   // stiffened gas
  taitEOS=5            // tait EOS (for water for e.g.)
};
  
enum RiemannSolverEnum
{
  exactRiemannSolver=0,
  roeRiemannSolver=1,
  futureRiemannSolver=2,
  hLLRiemannSolver=3  // *note* this is correct
};

enum PDEVariation   // These options modify the type of PDE.
{
  nonConservative=0,
  conservativeWithArtificialDissipation=1,
  conservativeGodunov=2
};
  

// Here are some standard test problems
enum TestProblems
{
  standard,
  laminarFlame
};

CnsParameters(const int & numberOfDimensions0=3);
~CnsParameters();

virtual 
int
assignParameterValues(const aString & label, RealArray & values,
		      const int & numRead, aString *c, real val[],
		      char *extraName1 /* = 0 */, const int & extraValue1Location /* = 0 */, 
		      char *extraName2 /* = 0 */, const int & extraValue2Location /* = 0 */, 
		      char *extraName3 /* = 0 */, const int & extraValue3Location /* = 0 */ );

virtual 
int buildReactions();

virtual int
chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg);

virtual int 
conservativeToPrimitive(GridFunction & gf, int gridToConvert=-1, int fixupUnsedPoints=false);

virtual int 
displayPdeParameters(FILE *file = stdout );

virtual
int get(const GenericDataBase & dir, const aString & name);

virtual
int getComponents( IntegerArray &components );

virtual int 
getDerivedFunction( const aString & name, const realCompositeGridFunction & u,
		    realCompositeGridFunction & v, const int component, const real t, 
		    Parameters & parameters);
virtual int
getDerivedFunction( const aString & name, const realMappedGridFunction & u, 
		    realMappedGridFunction & v, const int grid,
		    const int component, const real t, Parameters & parameters);

// compute the normal force on a boundary (for moving grid problems)
virtual 
int getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar,
                    bool includeViscosity = true  );

virtual
int getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, RealArray & ua, 
				const Index & I1, const Index &I2, const Index &I3, int numberOfTimeDerivatives = 0 );

virtual bool 
isMixedBC( int bc );

virtual
int numberOfGhostPointsNeeded() const;  // number of ghost points needed by this method.


virtual int 
primitiveToConservative(GridFunction & gf, int gridToConvert=-1, int fixupUnsedPoints=false);

virtual int 
put(GenericDataBase & dir, const aString & name);

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
setUserDefinedParameters();

virtual int
updatePDEparameters();

virtual int 
updateToMatchGrid( CompositeGrid & cg, 
		       IntegerArray & sharedBoundaryCondition = Overture::nullIntArray() );

virtual int 
updateUserDefinedEOS(GenericGraphicsInterface & gi);

virtual int 
updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg);

virtual
bool useConservativeVariables(int grid=-1) const;  // if true we are using a solver that uses conservative variables


};
#endif
