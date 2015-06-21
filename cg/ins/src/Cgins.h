#ifndef CGINS_H
#define CGINS_H

#include "DomainSolver.h"
#include "InsParameters.h"

// CG equation-domain solver for the Incompressible Navier-Stokes Equations

class Cgins : public DomainSolver
{
public:

Cgins(CompositeGrid & cg, 
      GenericGraphicsInterface *ps=NULL, 
      Ogshow *show=NULL, 
      const int & plotOption=1 );

virtual ~Cgins();

virtual
void addForcing(realMappedGridFunction & dvdt, const realMappedGridFunction & u, int iparam[], real rparam[],
		realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
                realMappedGridFunction *referenceFrameVelocity=NULL );

void 
addForcingToPressureEquation( const int & grid,
			      MappedGrid & c, 
			      realMappedGridFunction & f,  
			      realMappedGridFunction & gridVelocity, 
			      const real & t );

// Make adjustments to the pressure coefficient matrix (e.g. for the added mass algorithm)
void adjustPressureCoefficients( CompositeGrid & cg0, GridFunction & cgf );

virtual int 
advanceLineSolve(LineSolve & lineSolve,
		 const int grid, const int direction, 
		 realCompositeGridFunction & u0, 
		 realMappedGridFunction & f, 
		 realMappedGridFunction & residual,
		 const bool refactor,
		 const bool computeTheResidual  =false );

int 
advanceLineSolveOld(LineSolve & lineSolve,
		 const int grid, const int direction, 
		 realCompositeGridFunction & u0, 
		 realMappedGridFunction & f, 
		 realMappedGridFunction & residual,
		 const bool refactor,
		 const bool computeTheResidual  =false );

int 
advanceLineSolveNew(LineSolve & lineSolve,
		 const int grid, const int direction, 
		 realCompositeGridFunction & u0, 
		 realMappedGridFunction & f, 
		 realMappedGridFunction & residual,
		 const bool refactor,
		 const bool computeTheResidual  =false );


virtual int
applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
			realMappedGridFunction & gridVelocity,
			const int & grid,
			const int & option =-1,
			realMappedGridFunction *puOld =NULL,  
			realMappedGridFunction *pGridVelocityOld =NULL,
			const real & dt =-1. );

virtual int
applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & u, 
					       realMappedGridFunction &uL,
                                               realMappedGridFunction & gridVelocity,
					       real t,
					       int scalarSystem,
					       int grid );

void 
applyFourthOrderBoundaryConditions( realMappedGridFunction & u0, real t, int grid,
                                    realMappedGridFunction & gridVelocity );

virtual int 
assignInterfaceBoundaryConditions(GridFunction & cgf, 
				  const int & option=-1,
				  int grid_ =-1,
				  GridFunction *puOld =NULL, 
				  const real & dt=-1.);

int
assignLineSolverBoundaryConditions(const int grid, const int direction, 
				   realMappedGridFunction & u, 
				   realMappedGridFunction & f, 
				   const int numberOfTimeDependentComponents,
                                   bool isPeriodic[3] );

void 
assignPressureRHS( GridFunction & gf0, realCompositeGridFunction & f );

void
assignPressureRHS( const int grid, GridFunction & gf0, realCompositeGridFunction & f0 );

virtual int 
buildTimeSteppingDialog(DialogData & dialog );

int computeAxisymmetricDivergence(realArray & divergence, 
				  Index & I1, Index & I2, Index & I3, MappedGrid & c,
				  const realArray & u0,
				  const realArray & u0x, 
				  const realArray & v0y );

int 
computeTurbulenceQuantities( GridFunction & gf0 );

// int & debug() const { return parameters.debug;}

virtual void 
determineErrors(GridFunction & cgf,
		const aString & label =nullString );

virtual void
determineErrors(realCompositeGridFunction & u,
		realMappedGridFunction **gridVelocity,
		const real & t, 
		const int options,
                RealArray & err,
		const aString & label =nullString );


virtual void 
formMatrixForImplicitSolve(const real & dt0,
			   GridFunction & cgf1,
			   GridFunction & cgf0 );
virtual int
formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
			       const real & dt0, 
			       int scalarSystem,
			       realMappedGridFunction & u0,
			       const realMappedGridFunction & gridVelocity,
                               const int & grid,
                               const int & imp );


// Return the list of interface data needed by a given interface:
virtual int
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const;

virtual int
getResidual( real t, real dt, GridFunction & cgf, realCompositeGridFunction & residual);

virtual int
getTimeSteppingOption(const aString & answer,
		      DialogData & dialog );

// semi-discrete discretization. Lambda is used to determine the time step by requiring
// lambda*dt to be in the stability region of the particular time stepping method we are
// using
virtual void
getTimeSteppingEigenvalue(MappedGrid & mg, 
 			       realMappedGridFunction & u, 
 			       realMappedGridFunction & gridVelocity,  
 			       real & reLambda,
 			       real & imLambda, 
 			       const int & grid);

virtual int
getUt(const realMappedGridFunction & v, 
	  const realMappedGridFunction & gridVelocity, 
	  realMappedGridFunction & dvdt, 
	  int iparam[], real rparam[],
	  realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
	  MappedGrid *pmg2=NULL,
	  const realMappedGridFunction *pGridVelocity2= NULL);

void 
gridAccelerationBC(const int & grid,
		   const real & t0,
		   GridFunction & gf0, 
		   realCompositeGridFunction & f0,
		   int side,
		   int axis );

virtual void 
implicitSolve(const real & dt0,
	      GridFunction & cgf1,
              GridFunction & cgf0);

virtual 
void initializeFactorization();

virtual int
initializeTurbulenceModels(GridFunction & cgf);

int 
insImplicitMatrix(InsParameters::InsImplicitMatrixOptionsEnum option,
                  realMappedGridFunction & coeff,
		  const real & dt0, 
		  const realMappedGridFunction & u0,
		  realMappedGridFunction & fe,
		  realMappedGridFunction & fi,
                  const realMappedGridFunction & gridVelocity,
		  const int & grid);

virtual int
interfaceRightHandSide( InterfaceOptionsEnum option, 
                        int interfaceDataOptions,
                        GridFaceDescriptor & info, 
			GridFaceDescriptor & gfd,
                        int gfIndex, real t );
int
lineSolverBoundaryConditions(const int grid, const int direction, 
			     realMappedGridFunction & u, 
			     realMappedGridFunction & f, 
			     realMappedGridFunction & residual,
                             Index *Iv, Index *Jv, Index *Ixv,
                             const int maxNumberOfSystems, int *uSystem, 
                             const int numberOfTimeDependentComponents,
                             IntegerArray & bc, IntegerArray & extra, IntegerArray & offset,
                             bool & boundaryConditionsAreDifferent, bool isPeriodic[3] );

int
getLineSolverBoundaryConditions(const int grid, const int direction, 
				realMappedGridFunction & u,
				Index *Ixv,
				const int maxNumberOfSystems, int *uSystem, 
				const int numberOfTimeDependentComponents,
				IntegerArray & bc,IntegerArray & numGhost, 
				int & numberOfDifferentLineSolverBoundaryConditions );


virtual int
projectInitialConditionsForMovingGrids(int gfIndex);

// Project the velocity on FSI interfaces
int projectInterfaceVelocity(const real & t, realMappedGridFunction & u, 
			     realMappedGridFunction & gridVelocity,
			     const int & grid,
			     const real & dt =-1. );
virtual void 
outputSolution( realCompositeGridFunction & u, const real & t,
		const aString & label =nullString,
                int printOption = 0 );

virtual void 
printTimeStepInfo( const int & step, const real & t, const real & cpuTime );

virtual int 
insSetup();

virtual int 
setupPde(aString & reactionName,bool restartChosen, IntegerArray & originalBoundaryCondition);

virtual void 
updateDivergenceDamping( CompositeGrid & cg0, const int & geometryHasChanged );

virtual int
updateForNewTimeStep(GridFunction & gf, const real & dt );

virtual int 
updateGeometryArrays(GridFunction & cgf);

virtual void
updatePressureEquation(CompositeGrid & cg0, GridFunction & cgf );

virtual int 
updateForMovingGrids(GridFunction & cgf);

virtual int 
updateStateVariables(GridFunction & cgf, int stage=-1);

virtual void 
updateTimeIndependentVariables(CompositeGrid & cg0, GridFunction & cgf );

virtual int
updateToMatchGrid(CompositeGrid & cg);

virtual int
setSolverParameters(const aString & command = nullString,
                    DialogData *interface =NULL );

virtual int
setupGridFunctions();

virtual void
solveForTimeIndependentVariables( GridFunction & cgf, bool updateSolutionDependentEquations=false );

virtual int 
turbulenceModels(realArray & nuT,
		 MappedGrid & mg,
		 const realArray & u, 
		 const realArray & uu, 
		 const realArray & ut, 
		 const realArray & ux, 
		 const realArray & uy, 
		 const realArray & uz, 
		 const realArray & uxx, 
		 const realArray & uyy, 
		 const realArray & uzz, 
		 const Index & I1, const Index & I2, const Index & I3, 
		 Parameters & parameters,
		 real nu,
		 const int numberOfDimensions,
		 const int grid, const real t );
int 
turbulenceModelBoundaryConditions(const real & t,
				  realMappedGridFunction & u,
				  Parameters & parameters,
				  int grid,
				  RealArray *pBoundaryData[2][3] );

virtual realCompositeGridFunction & 
getAugmentedSolution( GridFunction & gf0, realCompositeGridFunction & v );

virtual void buildImplicitSolvers(CompositeGrid & cg);

virtual int
initializeSolution();

virtual int
setPlotTitle(const real &t, const real &dt);

virtual void
saveShowFileComments( Ogshow &show );

virtual
void writeParameterSummary( FILE *file );

virtual 
int project(GridFunction &cgf);

virtual
int setOgesBoundaryConditions( GridFunction &cgf, IntegerArray & boundaryConditions, RealArray &boundaryConditionData,
                               const int imp );

virtual int
userDefinedBoundaryValues(const real & t, 
                          GridFunction & gf0,
			  const int & grid,
			  int side0 = -1,
			  int axis0 = -1,
			  ForcingTypeEnum forcingType =computeForcing );
virtual int
userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u );

virtual void 
userDefinedInitialConditionsCleanup();


public: // for now 

// protected:

realCompositeGridFunction divDampingWeight;
// realCompositeGridFunction pressureRightHandSide;

// InsParameters parameters;  // overloads base class Parameters

// Oges *poisson;

private:

  // These next variables are for addForcing (optimized TZ evaluation)
  std::vector<real> tzTimeVector1;
  std::vector<real> tzTimeVector2;
  std::vector<realSerialArray*> tzForcingVector;

};

#endif
