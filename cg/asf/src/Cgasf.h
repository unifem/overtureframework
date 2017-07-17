#ifndef CGASF_H
#define CGASF_H

#include "DomainSolver.h"


// CG equation-domain solver for the All-Speed Navier-Stokes Equations

class Cgasf : public DomainSolver
{
public:

Cgasf(CompositeGrid & cg, GenericGraphicsInterface *ps=NULL, Ogshow *show=NULL, const int & plotOption=1 );


virtual ~Cgasf();


virtual void 
addAllSpeedImplicitForcing( realMappedGridFunction & u0, const real t, const real deltaT, int grid );

virtual
void addForcing(realMappedGridFunction & dvdt, const realMappedGridFunction & u, int iparam[], real rparam[],
		realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
                realMappedGridFunction *referenceFrameVelocity=NULL );

virtual void
addForcingToPressureEquation( const int & grid,
			      MappedGrid & c, 
			      realMappedGridFunction & f,  
			      realMappedGridFunction & gridVelocity, 
			      const real & t );

virtual void 
allSpeedImplicitTimeStep( GridFunction & gf0,  
			  real & t, 
			  real & dt0, 
			  int & numberOfSubSteps,
			  const real & nextTimeToPrint );

virtual int
applyBoundaryConditions(GridFunction & cgf,
                        const int & option =-1,
                        int grid_= -1,
                        GridFunction *puOld=NULL, 
                        const real & dt =-1. );
virtual int 
applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
			realMappedGridFunction & gridVelocity,
			const int & grid,
			const int & option=-1,
			realMappedGridFunction *puOld=NULL, 
			realMappedGridFunction *pGridVelocityOld=NULL,
			const real & dt=-1.);

virtual void 
assignTestProblem( GridFunction & cgf );

// virtual int
// applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & rhs, 
// 					       realMappedGridFunction & uL,
// 					       realMappedGridFunction & gridVelocity,
// 					       real t,
// 					       int scalarSystem,
// 					       int grid );

virtual int 
buildTimeSteppingDialog(DialogData & dialog );

// virtual void getUt( GridFunction & cgf, 
// 		    const real & t, 
// 		    RealCompositeGridFunction & ut, 
// 		    real tForce );

virtual void 
computeSource(const Index & I1,
	      const Index & I2, 
	      const Index & I3, 
	      const realArray & v, 
	      const realArray & s1, 
	      const realArray & s2, 
	      const realArray & gam, 
	      const realArray & cp, 
	      const realArray & r );

virtual void 
formAllSpeedPressureEquation( GridFunction & gf0, real t, real deltaT,
			      const bool & formSteadyEquation =false );

// virtual int 
// formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
// 			       const real & dt0, 
// 			       int scalarSystem, 
// 			       realMappedGridFunction & uL,
// 			       const int & grid );

// Return the list of interface data needed by a given interface:
virtual int
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const;

virtual int
getUt(const realMappedGridFunction & v, 
	  const realMappedGridFunction & gridVelocity, 
	  realMappedGridFunction & dvdt, 
	  int iparam[], real rparam[],
	  realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
	  MappedGrid *pmg2=NULL,
	  const realMappedGridFunction *pGridVelocity2= NULL);


virtual void
getTimeSteppingEigenvalue(MappedGrid & mg, 
 			       realMappedGridFunction & u, 
 			       realMappedGridFunction & gridVelocity,  
 			       real & reLambda,
 			       real & imLambda, 
 			       const int & grid);
virtual int
getTimeSteppingOption(const aString & answer,
		      DialogData & dialog );

void 
gridAccelerationBC(const int & grid,
		   const real & t0,
		   GridFunction & gf0, 
		   realCompositeGridFunction & f0,
		   int side,
		   int axis );
// virtual void 
// gridAccelerationBC(const int & grid,
// 		   const real & t0,
// 		   MappedGrid & c,
// 		   realMappedGridFunction & u ,
// 		   realMappedGridFunction & f ,
// 		   realMappedGridFunction & gridVelocity ,
// 		   realSerialArray & normal,
// 		   const Index & I1,
// 		   const Index & I2,
// 		   const Index & I3,
// 		   const Index & I1g,
// 		   const Index & I2g,
// 		   const Index & I3g,
// 		   int side,
//		   int axis );

virtual int
initializeSolution();

virtual int
interfaceRightHandSide( InterfaceOptionsEnum option, 
                        int interfaceDataOptions,
                        GridFaceDescriptor & info,
                        GridFaceDescriptor & gfd, 
			int gfIndex, real t, bool saveTimeHistory = false );

virtual real 
maxMachNumber( realMappedGridFunction & u );

virtual void 
printTimeStepInfo( const int & step, const real & t, const real & cpuTime );

virtual int 
setupPde(aString & reactionName,bool restartChosen, IntegerArray & originalBoundaryCondition);

virtual void
solveForAllSpeedPressure( real t, real deltaT, const real & dtRatio );

virtual void 
solveForTimeIndependentVariables( GridFunction & cgf, bool updateSolutionDependentEquations=false );

virtual int 
updateGeometryArrays(GridFunction & cgf);

// virtual int
// updateStateVariables(GridFunction & cgf);

virtual int
updateToMatchGrid(CompositeGrid & cg);

virtual int
setupGridFunctions();

virtual int
setPlotTitle(const real &t, const real &dt);

virtual void
saveShowFileComments( Ogshow &show );
virtual void writeParameterSummary( FILE * file );

protected:

std::vector<real> gridMachNumber;

int numberOfImplicitSolves;
bool refactorImplicitMatrix;

private:


};

#endif
