#ifndef INS_PARAMETERS
#define INS_PARAMETERS

#include "Parameters.h"
// Here are the run time and PDE parameters
class InsParameters : public Parameters
{
public:

enum BoundaryConditions 
{
  inflowWithVelocityGiven=2,
  inflowWithPressureAndTangentialVelocityGiven=3,
  outflow=5,
  convectiveOutflow=14,  
  tractionFree=15,
  inflowOutflow=30
};

enum PDEModel
{
  standardModel=0,
  BoussinesqModel,      // add a Temperature equation and buoyancy
  viscoPlasticModel,    // add a Temperature equation and nonlinear "viscosity"
  twoPhaseFlowModel,    // two-phase flow 
  numberOfPDEModels
};

enum ImplicitVariation
{
  implicitViscous=0,               // viscous terms are implicit 
  implicitAdvectionAndViscous,     // viscous and advection terms are implicit
  implicitFullLinearized           // build full lineared implicit operator
};


enum InsImplicitMatrixOptionsEnum  // option for insImplicitMatrix
{
  buildMatrix=0, 
  evalRightHandSide,
  evalResidual,
  evalResidualForBoundaryConditions
};

enum DiscretizationOptions // compact or non-compact
{
  standardFiniteDifference=0,
  compactDifference
};

enum AdvectionOptions  // options to treat the advection term
{
  centeredAdvection=0,
  upwindAdvection,
  bwenoAdvection
};

static aString PDEModelName[InsParameters::numberOfPDEModels+1];

InsParameters(const int & numberOfDimensions0=3);
~InsParameters();

virtual int
chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg);

virtual int 
displayPdeParameters(FILE *file = stdout );

// compute the normal force on a boundary (for moving grid problems)
virtual int 
getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar,
                bool includeViscosity = true ); 

int 
getModelVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, 
		   const int component );

int
getModelVariables( const aString & name, const realMappedGridFunction & uIn, realMappedGridFunction & vIn, 
		   const int grid,
		   const int component,
                   const real t );
int 
getTurbulenceModelVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, 
                             const int component );

int
getTurbulenceModelVariables( const aString & name, const realMappedGridFunction & uIn, realMappedGridFunction & vIn, 
                             const int grid,
                             const int component,
                             const real t );


int 
getTwoPhaseFlowVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, const int component );

int
getTwoPhaseFlowVariables( const aString & name, const realMappedGridFunction & uIn, realMappedGridFunction & vIn, 
                          const int grid,
                          const int component,
                          const real t );
virtual 
int
getDerivedFunction( const aString & name, const realMappedGridFunction & u, 
                    realMappedGridFunction & v, const int grid,
                    const int component, const real t, Parameters & parameters);

virtual
int getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, RealArray & ua, 
				const Index & I1, const Index &I2, const Index &I3, int numberOfTimeDerivatives = 0 );

// Some known solutions include rigid-body motions
virtual
int getUserDefinedKnownSolutionRigidBody( int body, real t, 
					  RealArray & xCM      = Overture::nullRealArray(), 
					  RealArray & vCM      = Overture::nullRealArray(),
					  RealArray & aCM      = Overture::nullRealArray(),
					  RealArray & omega    = Overture::nullRealArray(), 
					  RealArray & omegaDot = Overture::nullRealArray() );
int 
getViscoPlasticVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, const int component );

int
getViscoPlasticVariables( const aString & name, const realMappedGridFunction & uIn, realMappedGridFunction & vIn, 
                          const int grid,
                          const int component,
                          const real t );


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

virtual 
bool isMixedBC( int bc );

virtual
int numberOfGhostPointsNeeded() const;  // number of ghost points needed by this method.

virtual
int numberOfGhostPointsNeededForImplicitMatrix() const;  // number of ghost points needed for implicit matrix

virtual
int getComponents( IntegerArray &components );

virtual 
bool saveLinearizedSolution();  // save the linearized solution for implicit methods.

virtual 
int setDefaultDataForABoundaryCondition(const int & side,
					const int & axis,
					const int & grid,
					CompositeGrid & cg);

virtual
int updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg);



};

#endif
