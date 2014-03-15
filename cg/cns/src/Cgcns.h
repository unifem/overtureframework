#ifndef CGCNS_H
#define CGCNS_H

#include "DomainSolver.h"


//   Cgcns can be used to solve the compressible Navier-Stokes and reactive Euler equations.

class Cgcns : public DomainSolver
{
public:

Cgcns(CompositeGrid & cg, GenericGraphicsInterface *ps=NULL, Ogshow *show=NULL, const int & plotOption=1 );


virtual ~Cgcns();


virtual int 
applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
			realMappedGridFunction & gridVelocity,
			const int & grid,
			const int & option=-1,
			realMappedGridFunction *puOld=NULL, 
			realMappedGridFunction *pGridVelocityOld=NULL,
			const real & dt=-1.);


virtual int
applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & rhs, 
					       realMappedGridFunction & uL,
					       realMappedGridFunction & gridVelocity,
					       real t,
					       int scalarSystem,
					       int grid );

virtual int 
addConstraintEquation( Parameters &parameters, Oges& solver, 
		       realCompositeGridFunction &coeff, 
		       realCompositeGridFunction &ucur, 
		       realCompositeGridFunction &rhs, const int &numberOfComponents); 

virtual
void addForcing(realMappedGridFunction & dvdt, const realMappedGridFunction & u, int iparam[], real rparam[],
		realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
                realMappedGridFunction *referenceFrameVelocity=NULL );

virtual void 
assignTestProblem( GridFunction & cgf );

virtual void 
buildImplicitSolvers(CompositeGrid &cg);

virtual int 
buildTimeSteppingDialog(DialogData & dialog );

// virtual void getUt( GridFunction & cgf, 
// 		    const real & t, 
// 		    RealCompositeGridFunction & ut, 
// 		    real tForce );

virtual void 
formMatrixForImplicitSolve(const real & dt0,
			   GridFunction & cgf1,
			   GridFunction & cgf0 );
virtual int 
formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
			       const real & dt0, 
			       int scalarSystem, 
			       realMappedGridFunction & uL,
			       const int & grid );

virtual realCompositeGridFunction & 
getAugmentedSolution( GridFunction & gf0, realCompositeGridFunction & v );

// Return the list of interface data needed by a given interface:
virtual int
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const;

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


virtual int
getUt(const realMappedGridFunction & v, 
	  const realMappedGridFunction & gridVelocity, 
	  realMappedGridFunction & dvdt, 
	  int iparam[], real rparam[],
	  realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
	  MappedGrid *pmg2=NULL,
	  const realMappedGridFunction *pGridVelocity2= NULL);


virtual void 
implicitSolve(const real & dt0,
	      GridFunction & cgf1,
              GridFunction & cgf0);

virtual int
initializeSolution();

virtual int
interfaceRightHandSide( InterfaceOptionsEnum option, 
                        int interfaceDataOptions,
                        GridFaceDescriptor & info, 
			GridFaceDescriptor & gfd,
                        int gfIndex, real t );

virtual bool 
isImplicitMatrixSingular( realCompositeGridFunction &uL );

virtual void 
printTimeStepInfo( const int & step, const real & t, const real & cpuTime );

virtual int
project(GridFunction & cgf);

virtual void
saveShowFileComments( Ogshow &show );

virtual int
setPlotTitle(const real &t, const real &dt);

virtual int
setupGridFunctions();

virtual int 
setupPde(aString & reactionName,bool restartChosen, IntegerArray & originalBoundaryCondition);

virtual int 
updateGeometryArrays(GridFunction & cgf);

virtual int
updateStateVariables(GridFunction & cgf, int stage=-1);

virtual int
updateToMatchGrid(CompositeGrid & cg);

virtual int
userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u );

virtual void 
userDefinedInitialConditionsCleanup();

virtual
void writeParameterSummary( FILE *);

protected:


private:


};

#endif
